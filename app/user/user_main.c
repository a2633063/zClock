/******************************************************************************
 * Copyright 2013-2014 Espressif Systems (Wuxi)
 *
 * FileName: user_main.c
 *
 * Description: entry file of user application
 *
 * Modification history:
 *     2014/1/1, v1.0 create this file.
 *******************************************************************************/
#include "ets_sys.h"
#include "osapi.h"

#include "user_interface.h"

#include "user_key.h"
#include "user_led.h"
#include "user_wifi.h"
#include "user_sntp.h"
#include "user_tm1628.h"
#include "user_devicefind.h"


void user_rf_pre_init(void) {
//	system_set_os_print(0);	//关闭log打印功能
}

/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
 *******************************************************************************/
void user_init(void) {
	int i, j;
	uint32 x;

	uart_init(115200, 115200);
	os_printf("\n\nStart user%d.bin\n", system_upgrade_userbin_check() + 1);
	os_printf("SDK version:%s\n", system_get_sdk_version());


	user_wifi_init();
	user_key_init();
	user_led_init();
	user_tm1628_init();

	user_sntp_init();

	//UDP初始化,监听端口12345,当接收到特定字符串时,返回本设备IP及MAC地址
	user_devicefind_init(12345);



}

