#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "mem.h"
#include "user_interface.h"

#include "smartconfig.h"
#include "user_wifi.h"

#include "../include/espconn.h"

/*
 * wifi配置
 * 	wifi连接成功后初始化,配置SmartConfig,wifi指示灯
 */

LOCAL unsigned char user_smarconfig_flag = 0;	//smarconfig标志位,防止smarconfig出错

//struct mdns_info info;
//
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
//wifi event 回调函数
void wifi_handle_event_cb(System_Event_t *evt) {
	switch (evt->event) {
	case EVENT_STAMODE_CONNECTED:
		os_printf("wifi connect to ssid %s, channel %d\n", evt->event_info.connected.ssid,
				evt->event_info.connected.channel);
		break;
	case EVENT_STAMODE_DISCONNECTED:
		os_printf("wifi disconnect from ssid %s, reason %d\n", evt->event_info.disconnected.ssid,
				evt->event_info.disconnected.reason);
		break;
	case EVENT_STAMODE_AUTHMODE_CHANGE:
		os_printf("wifi change mode: %d -> %d\n", evt->event_info.auth_change.old_mode,
				evt->event_info.auth_change.new_mode);
		break;
	case EVENT_STAMODE_GOT_IP:
//		os_printf("wifi got ip:" IPSTR ",mask:" IPSTR ",gw:" IPSTR, IP2STR(&evt->event_info.got_ip.ip),
//				IP2STR(&evt->event_info.got_ip.mask), IP2STR(&evt->event_info.got_ip.gw));
//		os_printf("\n");
		wifi_status_led_uninstall();
		user_set_led(1);

//		user_mdns_config();

		break;
	case EVENT_SOFTAPMODE_STACONNECTED:
		os_printf("wifi station: " MACSTR "join, AID = %d\n", MAC2STR(evt->event_info.sta_connected.mac),
				evt->event_info.sta_connected.aid);
		break;
	case EVENT_SOFTAPMODE_STADISCONNECTED:
		os_printf("wifi station: " MACSTR "leave, AID = %d\n", MAC2STR(evt->event_info.sta_disconnected.mac),
				evt->event_info.sta_disconnected.aid);
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
		os_printf("smartconfig complete");
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
		os_printf("set wifi mode:station");
	}
	//设置自动连接AP
	if (wifi_station_get_auto_connect() == 0) {
		wifi_station_set_auto_connect(1);
		os_printf("set auto connect AP:true");
	}
	wifi_set_event_handler_cb(wifi_handle_event_cb);
	wifi_status_led_install(GPIO_WIFI_LED_IO_NUM, GPIO_WIFI_LED_IO_MUX, GPIO_WIFI_LED_IO_FUNC);
}

void ICACHE_FLASH_ATTR user_smartconfig(void) {
	//设置为station模式
	if (wifi_get_opmode() != STATION_MODE || wifi_get_opmode_default() != STATION_MODE) {
		wifi_set_opmode(STATION_MODE);
		os_printf("set wifi mode:station");
	}

	if (user_smarconfig_flag != 0) {
		smartconfig_stop();
		user_smarconfig_flag = 0;
	}
	os_printf("smartconfig start");
	smartconfig_start(smartconfig_done);
	user_set_led(0);

	wifi_status_led_install(GPIO_WIFI_LED_IO_NUM, GPIO_WIFI_LED_IO_MUX, GPIO_WIFI_LED_IO_FUNC);
}

void ICACHE_FLASH_ATTR user_smartconfig_stop(void) {
	if (user_smarconfig_flag != 0) {
		smartconfig_stop();
		user_smarconfig_flag = 0;
		os_printf("smartconfig stop");
		wifi_status_led_uninstall();
		user_set_led(1);
	}
}
