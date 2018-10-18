#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "mem.h"
#include "user_interface.h"

#include "user_beep.h"



LOCAL os_timer_t timer_beep;


void ICACHE_FLASH_ATTR
user_beep_on(unsigned char beep) {
	GPIO_OUTPUT_SET(GPIO_BEEP_0_IO_NUM, !beep);
}


void ICACHE_FLASH_ATTR user_beep_timer_func(void *arg) {

}


void ICACHE_FLASH_ATTR
user_beep_init(void) {
	//GPIO16不支持外部中断,所以无法直接使用key driver
	GPIO_DIS_OUTPUT(GPIO_BEEP_0_IO_NUM);	//配置按键GPIO16为输出


	os_timer_disarm(&timer_beep);
	os_timer_setfn(&timer_beep, (os_timer_func_t *) user_beep_timer_func, NULL);
	//os_timer_arm(&timer_beep, 500, 0);	//30ms

}
