#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "mem.h"
#include "user_interface.h"

#include "espconn.h"

#include "user_function.h"
#include "user_setting.h"


/*
 * 通用函数
 * tcp udp接收数据后的处理函数
 */

const char *device_find_request = "Device Report!!";

const char *device_find_response_ok = "I'm clock:";

const char *setting_get_all = "get all setting";
const char *setting_update = "update";

void ICACHE_FLASH_ATTR
user_con_received(void *arg, char *pusrdata, unsigned short length) {
	if (length == 1 && *pusrdata == 127)
		return;

	struct espconn *pesp_conn = arg;

	int i, j, k;
	char DeviceBuffer[40] = { 0 };

	os_sprintf(DeviceBuffer, "result:%s\n", pusrdata);

	if (pesp_conn->type == ESPCONN_TCP) { //tcp
		//espconn_sent(pesp_conn, DeviceBuffer, os_strlen(DeviceBuffer));
	} else if (pesp_conn->type == ESPCONN_UDP) {		//udp
		remot_info *premot = NULL;
		if (espconn_get_connection_info(pesp_conn, &premot, 0) != ESPCONN_OK)
			return;
		pesp_conn->proto.tcp->remote_port = premot->remote_port;	//获取端口
		pesp_conn->proto.tcp->remote_ip[0] = premot->remote_ip[0];	//获取IP地址
		pesp_conn->proto.tcp->remote_ip[1] = premot->remote_ip[1];
		pesp_conn->proto.tcp->remote_ip[2] = premot->remote_ip[2];
		pesp_conn->proto.tcp->remote_ip[3] = premot->remote_ip[3];
		//espconn_sent(pesp_conn, DeviceBuffer, os_strlen(DeviceBuffer));	//发送数据
	}

	if (length == os_strlen(device_find_request) &&
	os_strncmp(pusrdata, device_find_request, os_strlen(device_find_request)) == 0) {

		char Device_mac_buffer[60] = { 0 };
		char hwaddr[6];
		struct ip_info ipconfig;

		wifi_get_ip_info(STATION_IF, &ipconfig);
		wifi_get_macaddr(STATION_IF, hwaddr);

		os_sprintf(DeviceBuffer, "%s" MACSTR "," IPSTR, device_find_response_ok, MAC2STR(hwaddr), IP2STR(&ipconfig.ip));
		os_printf("%s\n", DeviceBuffer);
		espconn_sent(pesp_conn, DeviceBuffer, os_strlen(DeviceBuffer));
	}  else if (length == 1 && *pusrdata == '+') {
//		user_rudder_press(1);
	} else if (length == 1 && *pusrdata == '-') {
//		user_rudder_press(0);
	} else if (length == os_strlen(setting_update)
			&& os_strncmp(pusrdata, setting_update, os_strlen(setting_update)) == 0) {
		os_printf("\nupdate\n");
		os_printf("user bin:%d\n", system_upgrade_userbin_check());
		user_update();
	} else if (length == os_strlen(setting_get_all)
			&& os_strncmp(pusrdata, setting_get_all, os_strlen(setting_get_all)) == 0) {

//		os_sprintf(DeviceBuffer, "%s%03d\n%s%03d\n%s%03d\n%s%03d\n", setting_pwm_max, i, setting_pwm_min, j,
//				setting_pwm_middle, k, setting_pwm_middle_delay, l);
//
//		os_printf("%s", DeviceBuffer);
//		espconn_sent(pesp_conn, DeviceBuffer, os_strlen(DeviceBuffer));
	}

}
