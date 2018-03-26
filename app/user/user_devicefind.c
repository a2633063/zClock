/******************************************************************************
 * Copyright 2013-2014 Espressif Systems (Wuxi)
 *
 * FileName: user_devicefind.c
 *
 * Description: Find your hardware's information while working any mode.
 *
 * Modification history:
 *     2014/3/12, v1.0 create this file.
 *******************************************************************************/
#include "ets_sys.h"
#include "os_type.h"
#include "osapi.h"
#include "mem.h"
#include "user_interface.h"

#include "espconn.h"
#include "user_devicefind.h"

#include "user_function.h"


/*---------------------------------------------------------------------------*/
LOCAL struct espconn ptrespconn;

/******************************************************************************
 * FunctionName : user_devicefind_init
 * Description  : the espconn struct parame init
 * Parameters   : none
 * Returns      : none
 *******************************************************************************/
void ICACHE_FLASH_ATTR
user_devicefind_init(int port) {
	ptrespconn.type = ESPCONN_UDP;
	ptrespconn.proto.udp = (esp_udp *) os_zalloc(sizeof(esp_udp));
	ptrespconn.proto.udp->local_port = port;
	espconn_regist_recvcb(&ptrespconn, user_con_received);
//	espconn_regist_recvcb(&ptrespconn, user_devicefind_recv);
	espconn_create(&ptrespconn);
}
