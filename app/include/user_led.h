#ifndef __USER_LED_H__
#define __USER_LED_H__


#define GPIO_LED_0_IO_MUX     PERIPHS_IO_MUX_GPIO2_U
#define GPIO_LED_0_IO_NUM     2
#define GPIO_LED_0_IO_FUNC    FUNC_GPIO2

#define GPIO_LED_1_IO_MUX     PERIPHS_IO_MUX_MTDO_U
#define GPIO_LED_1_IO_NUM     15
#define GPIO_LED_1_IO_FUNC    FUNC_GPIO15

void user_led_init(void);
unsigned char user_get_led();
void user_set_led(unsigned char level);
#endif
