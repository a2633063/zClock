#ifndef __USER_WIFI_H__
#define __USER_WIFI_H__


#define GPIO_WIFI_LED_IO_MUX     PERIPHS_IO_MUX_GPIO2_U
#define GPIO_WIFI_LED_IO_NUM     2
#define GPIO_WIFI_LED_IO_FUNC    FUNC_GPIO2


void user_wifi_init(void);
void user_smartconfig(void);
void user_smartconfig_stop(void);
#endif
