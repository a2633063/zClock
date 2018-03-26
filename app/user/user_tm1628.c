#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "mem.h"
#include "user_interface.h"

#include "gpio.h"

#include "user_tm1628.h"
#include "user_sntp.h"


const unsigned char SEG_TEMP[4]={0x00,0x80,0x40,0xc0};
#define SEG_REB(a) ((a&0x3f)|SEG_TEMP[a>>6])


unsigned char Seg[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};

unsigned char display[6] = {0x40,0x40,0x40,0x40,0x40,0x40};
unsigned char brightness = 3;

LOCAL os_timer_t timer_tm1628;

void user_tm1628_timer_func(void *arg) {
	user_tm1628_write_cmd(0x02);	//设置显示模式(02H:6位11段)
	user_tm1628_write_cmd(0x40);	//设置写显存的数据命令,采用地址自动加1
//	user_tm1628_write_page(0xc0, display, 6);
	user_tm1628_write_dat(0xc0,SEG_REB(display[5]));
	user_tm1628_write_dat(0xc2,SEG_REB(display[4]));
	user_tm1628_write_dat(0xc4,(display[3]));
	user_tm1628_write_dat(0xc6,SEG_REB(display[2]));
	user_tm1628_write_dat(0xc8,(display[1]));
	user_tm1628_write_dat(0xca,(display[0]));
	if (brightness > 7)
		brightness = 7;
	user_tm1628_write_cmd(0x88 | brightness);	//开显示,设置亮度
}

void ICACHE_FLASH_ATTR
user_tm1628_init(void) {

	PIN_FUNC_SELECT(GPIO_TM1628_DIO_IO_MUX, GPIO_TM1628_DIO_IO_FUNC);
	PIN_FUNC_SELECT(GPIO_TM1628_CLK_IO_MUX, GPIO_TM1628_CLK_IO_FUNC);
	PIN_FUNC_SELECT(GPIO_TM1628_STB_IO_MUX, GPIO_TM1628_STB_IO_FUNC);	//IO口输出

	GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_TM1628_DIO_IO_NUM), 0);
	GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_TM1628_CLK_IO_NUM), 0);
	GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_TM1628_STB_IO_NUM), 0);

//	os_timer_disarm(&timer_tm1628);
	os_timer_setfn(&timer_tm1628, (os_timer_func_t *) user_tm1628_timer_func, NULL);
	os_timer_arm(&timer_tm1628, 200, 1);	//每200ms刷新一次显示
}

void ICACHE_FLASH_ATTR
user_tm1628_time_refresh(void) {

	display[0]=Seg[time.hour/10%10];
	display[1]=Seg[time.hour%10] |0x80;
	display[2]=Seg[time.minute/10%10]|0x80;
	display[3]=Seg[time.minute%10] |0x80;
	display[4]=Seg[time.second/10%10]|0x80;
	display[5]=Seg[time.second%10] ;

}

void ICACHE_FLASH_ATTR
user_tm1628_write_cmd(unsigned char cmd) {

	unsigned char i, temp;

	GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_TM1628_STB_IO_NUM), 1);
	GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_TM1628_STB_IO_NUM), 0);	//STB下降沿

	temp = cmd;
	for (i = 0; i < 8; i++) {
		GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_TM1628_CLK_IO_NUM), 0);
		if (temp & 0x01)
			GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_TM1628_DIO_IO_NUM), 1);
		else
			GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_TM1628_DIO_IO_NUM), 0);
		GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_TM1628_CLK_IO_NUM), 1);
		temp = temp >> 1;
	}

	GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_TM1628_STB_IO_NUM), 1);

}

void ICACHE_FLASH_ATTR
user_tm1628_write_dat(unsigned char addr, unsigned char dat) {

	unsigned char i, temp;

	GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_TM1628_STB_IO_NUM), 1);
	GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_TM1628_STB_IO_NUM), 0);	//STB下降沿

	temp = addr;
	for (i = 0; i < 8; i++) {
		GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_TM1628_CLK_IO_NUM), 0);
		if (temp & 0x01)
			GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_TM1628_DIO_IO_NUM), 1);
		else
			GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_TM1628_DIO_IO_NUM), 0);
		GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_TM1628_CLK_IO_NUM), 1);
		temp = temp >> 1;
	}

	temp = dat;
	for (i = 0; i < 8; i++) {
		GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_TM1628_CLK_IO_NUM), 0);
		if (temp & 0x01)
			GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_TM1628_DIO_IO_NUM), 1);
		else
			GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_TM1628_DIO_IO_NUM), 0);
		GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_TM1628_CLK_IO_NUM), 1);
		temp = temp >> 1;
	}

	GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_TM1628_STB_IO_NUM), 1);

}

void ICACHE_FLASH_ATTR
user_tm1628_write_page(unsigned char addr, unsigned char* p, unsigned char length) {

	unsigned char i, j, temp;

	if (length > 14)
		length = 14;
	GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_TM1628_STB_IO_NUM), 1);
	GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_TM1628_STB_IO_NUM), 0);	//STB下降沿

	temp = addr;
	for (i = 0; i < 8; i++) {
		GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_TM1628_CLK_IO_NUM), 0);
		if (temp & 0x01)
			GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_TM1628_DIO_IO_NUM), 1);
		else
			GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_TM1628_DIO_IO_NUM), 0);
		GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_TM1628_CLK_IO_NUM), 1);
		temp = temp >> 1;
	}

	for (j = 0; j < length; j++) {
		temp = *p;
		for (i = 0; i < 16; i++) {
			GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_TM1628_CLK_IO_NUM), 0);
			if (temp & 0x01)
				GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_TM1628_DIO_IO_NUM), 1);
			else
				GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_TM1628_DIO_IO_NUM), 0);
			GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_TM1628_CLK_IO_NUM), 1);
			temp = temp >> 1;
		}
		p++;
	}
	GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_TM1628_STB_IO_NUM), 1);

}
