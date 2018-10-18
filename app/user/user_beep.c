#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "mem.h"
#include "user_interface.h"
#include "gpio.h"
#include "user_beep.h"



LOCAL os_timer_t timer_beep;

#define BEEP_ON 0
#define BEEP_OFF 1
void ICACHE_FLASH_ATTR user_beep_timer_func(void *arg) {
	if(GPIO_INPUT_GET(GPIO_BEEP_0_IO_NUM)==BEEP_ON){
		GPIO_OUTPUT_SET(GPIO_BEEP_0_IO_NUM, BEEP_OFF);
	}
}


void ICACHE_FLASH_ATTR
user_beep_on(unsigned char beep_time) {
	GPIO_OUTPUT_SET(GPIO_BEEP_0_IO_NUM, BEEP_ON);
	os_timer_disarm(&timer_beep);
	os_timer_setfn(&timer_beep, (os_timer_func_t *) user_beep_timer_func, NULL);
	os_timer_arm(&timer_beep, beep_time, 0);
}





void ICACHE_FLASH_ATTR
user_beep_init(void) {

	GPIO_DIS_OUTPUT(GPIO_BEEP_0_IO_NUM);	//���÷�����GPIO0Ϊ���


	os_timer_disarm(&timer_beep);
	os_timer_setfn(&timer_beep, (os_timer_func_t *) user_beep_timer_func, NULL);
	//os_timer_arm(&timer_beep, 500, 0);	//30ms

}
