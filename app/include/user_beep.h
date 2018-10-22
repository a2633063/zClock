#ifndef __USER_BEEP_H__
#define __USER_BEEP_H__

#define GPIO_BEEP_0_IO_MUX     PERIPHS_IO_MUX_GPIO0_U
#define GPIO_BEEP_0_IO_NUM     0
#define GPIO_BEEP_0_IO_FUNC    FUNC_GPIO0

void user_beep_on(unsigned int beep);

void user_beep_init(void);
#endif
