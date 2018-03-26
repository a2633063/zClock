#ifndef __USER_KEY_H__
#define __USER_KEY_H__

#define GPIO_KEY_NUM            1

#define GPIO_KEY_0_IO_MUX     PERIPHS_IO_MUX_GPIO0_U
#define GPIO_KEY_0_IO_NUM     0
#define GPIO_KEY_0_IO_FUNC    FUNC_GPIO0

#define GPIO_KEY_1_IO_MUX     PERIPHS_IO_MUX_SD_DATA3_U
#define GPIO_KEY_1_IO_NUM     10
#define GPIO_KEY_1_IO_FUNC    FUNC_GPIO10

void user_key_init(void);
#endif
