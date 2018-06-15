#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "mem.h"
#include "user_interface.h"

#include "smartconfig.h"
#include "user_wifi.h"

#include "../include/espconn.h"
static os_timer_t wifi_reconnect_timer;
static uint8 wifi_reconnect_count = 0;
/*
 * wifi配置
 * 	wifi连接成功后初始化,配置SmartConfig,wifi指示灯
 */

LOCAL unsigned char user_smarconfig_flag = 0;	//smarconfig标志位,防止smarconfig出错

//mDNS
//struct mdns_info info;
//void user_mdns_config() {
//	struct ip_info ipconfig;
//	wifi_get_ip_info(STATION_IF, &ipconfig);
//	info->host_name = "espressif";
//	info->ipAddr = ipconfig.ip.addr; //ESP8266 station IP
//	info->server_name = "iot";
//	info->server_port = 8080;
//	info->txt_data[0] = "version = now";
//	info->txt_data[1] = "user1 = data1";
//	info->txt_data[2] = "user2 = data2";
//	espconn_mdns_init(&info);
//}
//wifi连接失败重连定时器
void ICACHE_FLASH_ATTR user_wifi_timer_reconnect_func(void *arg) {
	wifi_reconnect_count = 0;
	user_set_led(0);
	wifi_status_led_install(GPIO_WIFI_LED_IO_NUM, GPIO_WIFI_LED_IO_MUX, GPIO_WIFI_LED_IO_FUNC);
	os_printf("wifi reconnect timer: wifi state: %d\n", wifi_station_get_connect_status());
	wifi_station_connect();
}

//wifi event 回调函数
void wifi_handle_event_cb(System_Event_t *evt) {
	switch (evt->event) {
	case EVENT_STAMODE_CONNECTED:	//wifi连接
		os_printf("wifi connect to ssid %s, channel %d\n", evt->event_info.connected.ssid, evt->event_info.connected.channel);
		wifi_reconnect_count=0;
		os_timer_disarm(&wifi_reconnect_timer);
		wifi_status_led_uninstall();
		user_set_led(0);
		break;
	case EVENT_STAMODE_DISCONNECTED:	//wifi断开
		wifi_reconnect_count++;
		os_printf("wifi disconnect from ssid %s, reason %d,count: %d\n", evt->event_info.disconnected.ssid, evt->event_info.disconnected.reason,wifi_reconnect_count);
		if (wifi_reconnect_count > 19) {
			wifi_reconnect_count=0;
			os_printf("Reconnect after 60 seconds\n");
			wifi_station_disconnect();
			wifi_status_led_uninstall();
			user_set_led(1);
			os_timer_disarm(&wifi_reconnect_timer);
			os_timer_setfn(&wifi_reconnect_timer, (os_timer_func_t *) user_wifi_timer_reconnect_func, NULL);
			os_timer_arm(&wifi_reconnect_timer, 60000, 0);	//60s后重连
		}
		break;
	case EVENT_STAMODE_AUTHMODE_CHANGE:
		os_printf("wifi change mode: %d -> %d\n", evt->event_info.auth_change.old_mode, evt->event_info.auth_change.new_mode);
		break;
	case EVENT_STAMODE_GOT_IP:	//获取到IP
//		os_printf("wifi got ip:" IPSTR ",mask:" IPSTR ",gw:" IPSTR, IP2STR(&evt->event_info.got_ip.ip),
//				IP2STR(&evt->event_info.got_ip.mask), IP2STR(&evt->event_info.got_ip.gw));
//		os_printf("\n");

		user_set_led(1);
//		user_mdns_config();
		break;

	default:
		break;
	}
}

//smartconfig过程回调函数
void ICACHE_FLASH_ATTR
smartconfig_done(sc_status status, void *pdata) {
	switch (status) {
	case SC_STATUS_WAIT:
		os_printf("1:SC_STATUS_WAIT\n");
		user_smarconfig_flag = 1;
		break;
	case SC_STATUS_FIND_CHANNEL:
		os_printf("2:SC_STATUS_FIND_CHANNEL\n");
		user_smarconfig_flag = 1;
		break;
	case SC_STATUS_GETTING_SSID_PSWD:
		os_printf("3:SC_STATUS_GETTING_SSID_PSWD\n");
		sc_type *type = pdata;
		if (*type == SC_TYPE_ESPTOUCH) {
			os_printf("SC_TYPE:SC_TYPE_ESPTOUCH\n");
		} else {
			os_printf("SC_TYPE:SC_TYPE_AIRKISS\n");
		}
		break;
	case SC_STATUS_LINK:
		os_printf("4:SC_STATUS_LINK\n");
		struct station_config *sta_conf = pdata;
		wifi_station_set_config(sta_conf);
		wifi_station_disconnect();
		wifi_station_connect();
		break;
	case SC_STATUS_LINK_OVER:
		os_printf("5:SC_STATUS_LINK_OVER\n");
		if (pdata != NULL) {
			uint8 phone_ip[4] = { 0 };
			memcpy(phone_ip, (uint8*) pdata, 4);
			os_printf("Phone ip: %d.%d.%d.%d\n", phone_ip[0], phone_ip[1], phone_ip[2], phone_ip[3]);
		}
		smartconfig_stop();
		os_printf("smartconfig complete\n");
		user_smarconfig_flag = 0;
		wifi_status_led_uninstall();
		user_set_led(1);
		break;
	}
}

void ICACHE_FLASH_ATTR user_wifi_init(void) {

	//设置为station模式
	if (wifi_get_opmode() != STATION_MODE || wifi_get_opmode_default() != STATION_MODE) {
		wifi_set_opmode(STATION_MODE);
	}
	os_printf("set wifi mode:station\n");

	//失能自动连接AP
	if (wifi_station_get_auto_connect() == 0) {
		wifi_station_set_auto_connect(1);	//失能上电自动连接已记录的 AP,必须在system_init_done_cb中手动连接wifi
	}
	os_printf("set auto connect AP\n");

	//设置连接失败后自动重连ap,使用定时器每隔15s尝试连接一次
	wifi_station_set_reconnect_policy(true);

	wifi_set_event_handler_cb(wifi_handle_event_cb);
	wifi_status_led_install(GPIO_WIFI_LED_IO_NUM, GPIO_WIFI_LED_IO_MUX, GPIO_WIFI_LED_IO_FUNC);
}

void ICACHE_FLASH_ATTR user_smartconfig(void) {
	//设置为station模式
	if (wifi_get_opmode() != STATION_MODE || wifi_get_opmode_default() != STATION_MODE) {
		wifi_set_opmode(STATION_MODE);
		os_printf("set wifi mode:station\n");
	}

	if (user_smarconfig_flag != 0) {
		smartconfig_stop();
		user_smarconfig_flag = 0;
	}
	os_printf("smartconfig start\n");
	smartconfig_start(smartconfig_done);
	user_set_led(0);

	wifi_status_led_install(GPIO_WIFI_LED_IO_NUM, GPIO_WIFI_LED_IO_MUX, GPIO_WIFI_LED_IO_FUNC);
}

void ICACHE_FLASH_ATTR user_smartconfig_stop(void) {
	if (user_smarconfig_flag != 0) {
		smartconfig_stop();
		user_smarconfig_flag = 0;
		os_printf("smartconfig stop\n");
		wifi_status_led_uninstall();
		user_set_led(1);
	}
}
