/******************************************************************************
 * Copyright 2013-2014 Espressif Systems (Wuxi)
 *
 * FileName: user_webserver.c
 *
 * Description: The web server mode configration.
 *              Check your hardware connection with the host while use this mode.
 * Modification history:
 *     2014/3/12, v1.0 create this file.
 *******************************************************************************/
#include "ets_sys.h"
#include "os_type.h"
#include "osapi.h"
#include "mem.h"
#include "user_interface.h"

#include "espconn.h"
#include "user_webserver.h"

#include "upgrade.h"

#include "user_devicefind.h"

#include "user_function.h"

uint8 *web_http =
		"HTTP/1.1 200 OK\n"
				"Content-Length: 159\n\n"
				"<html><body><a href=\"pwm_delay=123\"><button type=\"button\">Click Me!</button></a><a href=\"+\"><button type=\"button\">Click Me2!</button></a></body></html>\n";
/******************************************************************************
 * FunctionName : webserver_recv
 * Description  : Processing the received data from the server
 * Parameters   : arg -- Additional argument to pass to the callback function
 *                pusrdata -- The received data (or NULL when the connection has been closed!)
 *                length -- The length of received data
 * Returns      : none
 *******************************************************************************/
LOCAL void ICACHE_FLASH_ATTR
webserver_recv(void *arg, char *pusrdata, unsigned short length) {
	struct espconn *pesp_conn = arg;
	char *pParseBuffer = NULL;
	int i, j, k;
	char DeviceBuffer[70] = { 0 };

//	struct espconn *ptrespconn = arg;
	os_printf("tcp get:%s\n", pusrdata);

	os_printf("len:%u\n", length);

	if (os_strncmp(pusrdata, "GET /favicon", 11) == 0) {
		espconn_sent(pesp_conn, web_http, os_strlen(web_http));
	}
	if (os_strncmp(pusrdata, "GET /", 5) == 0) {
		os_printf("We have a GET request.\n");
		espconn_sent(pesp_conn, web_http, os_strlen(web_http));

		pusrdata += 5;
		if (length >= 1 && *pusrdata == '+') {
			user_rudder_press(1);
		} else if (length >= 1 && *pusrdata == '-') {
			user_rudder_press(0);
		} else if (length >= os_strlen(setting_update)
				&& os_strncmp(pusrdata, setting_update, os_strlen(setting_update)) == 0) {

			os_printf("\nupdate\n");
			os_printf("user bin:%d\n", system_upgrade_userbin_check());
			user_update();
		} else if (length >= os_strlen(setting_get_all)
				&& os_strncmp(pusrdata, setting_get_all, os_strlen(setting_get_all)) == 0) {

//			os_sprintf(DeviceBuffer, "%s%03d\n%s%03d\n%s%03d\n%s%03d\n", setting_pwm_max, i, setting_pwm_min, j,
//					setting_pwm_middle, k, setting_pwm_middle_delay, rudder_middle_delay);
//
//			os_printf("%s", DeviceBuffer);
//			espconn_sent(pesp_conn, DeviceBuffer, os_strlen(DeviceBuffer));
		}  else {
			os_printf("tcp get:%s\n", pusrdata);
		}
	}

}

/******************************************************************************
 * FunctionName : webserver_recon
 * Description  : the connection has been err, reconnection
 * Parameters   : arg -- Additional argument to pass to the callback function
 * Returns      : none
 *******************************************************************************/
LOCAL ICACHE_FLASH_ATTR
void webserver_recon(void *arg, sint8 err) {
	struct espconn *pesp_conn = arg;

	os_printf("webserver's %d.%d.%d.%d:%d err %d reconnect\n", pesp_conn->proto.tcp->remote_ip[0],
			pesp_conn->proto.tcp->remote_ip[1], pesp_conn->proto.tcp->remote_ip[2], pesp_conn->proto.tcp->remote_ip[3],
			pesp_conn->proto.tcp->remote_port, err);
}

/******************************************************************************
 * FunctionName : webserver_recon
 * Description  : the connection has been err, reconnection
 * Parameters   : arg -- Additional argument to pass to the callback function
 * Returns      : none
 *******************************************************************************/
LOCAL ICACHE_FLASH_ATTR
void webserver_discon(void *arg) {
	struct espconn *pesp_conn = arg;

	os_printf("webserver's %d.%d.%d.%d:%d disconnect\n", pesp_conn->proto.tcp->remote_ip[0],
			pesp_conn->proto.tcp->remote_ip[1], pesp_conn->proto.tcp->remote_ip[2], pesp_conn->proto.tcp->remote_ip[3],
			pesp_conn->proto.tcp->remote_port);
}

/******************************************************************************
 * FunctionName : user_accept_listen
 * Description  : server listened a connection successfully
 * Parameters   : arg -- Additional argument to pass to the callback function
 * Returns      : none
 *******************************************************************************/
LOCAL void ICACHE_FLASH_ATTR
webserver_listen(void *arg) {
	struct espconn *pesp_conn = arg;

	espconn_regist_recvcb(pesp_conn, webserver_recv);
	espconn_regist_reconcb(pesp_conn, webserver_recon);
	espconn_regist_disconcb(pesp_conn, webserver_discon);
}

/******************************************************************************
 * FunctionName : user_webserver_init
 * Description  : parameter initialize as a server
 * Parameters   : port -- server port
 * Returns      : none
 *******************************************************************************/
void ICACHE_FLASH_ATTR
user_webserver_init(uint32 port) {
	LOCAL struct espconn esp_conn;
	LOCAL esp_tcp esptcp;

	esp_conn.type = ESPCONN_TCP;
	esp_conn.state = ESPCONN_NONE;
	esp_conn.proto.tcp = &esptcp;
	esp_conn.proto.tcp->local_port = port;
	espconn_regist_connectcb(&esp_conn, webserver_listen);

	espconn_accept(&esp_conn);

}
