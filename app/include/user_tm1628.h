#ifndef __USER_TM1628_H__
#define __USER_TM1628_H__


#define GPIO_TM1628_DIO_IO_MUX     PERIPHS_IO_MUX_MTCK_U
#define GPIO_TM1628_DIO_IO_NUM     13
#define GPIO_TM1628_DIO_IO_FUNC    FUNC_GPIO13

#define GPIO_TM1628_CLK_IO_MUX     PERIPHS_IO_MUX_MTDI_U
#define GPIO_TM1628_CLK_IO_NUM     12
#define GPIO_TM1628_CLK_IO_FUNC    FUNC_GPIO12

#define GPIO_TM1628_STB_IO_MUX     PERIPHS_IO_MUX_MTMS_U
#define GPIO_TM1628_STB_IO_NUM     14
#define GPIO_TM1628_STB_IO_FUNC    FUNC_GPIO14

unsigned char auto_brightness;
unsigned char brightness;

const unsigned char Seg[10];
unsigned char display[6];
void user_tm1628_init(void);
void user_tm1628_time_refresh(void) ;
void user_tm1628_write_cmd(unsigned char cmd);
void user_tm1628_write_dat(unsigned char addr, unsigned char dat);
void user_tm1628_write_page(unsigned char addr, unsigned char* p, unsigned char length);
#endif
