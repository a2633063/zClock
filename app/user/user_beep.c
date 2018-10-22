#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "mem.h"
#include "user_interface.h"
#include "gpio.h"
#include "user_beep.h"
#include "user_alarm.h"



LOCAL os_timer_t timer_beep;

#define BEEP_ON 0
#define BEEP_OFF 1
void ICACHE_FLASH_ATTR user_beep_timer_func(void *arg) {
	if(GPIO_INPUT_GET(GPIO_BEEP_0_IO_NUM)==BEEP_ON){
		GPIO_OUTPUT_SET(GPIO_BEEP_0_IO_NUM, BEEP_OFF);
		if(alarm_flag){
			os_timer_disarm(&timer_beep);
			os_timer_setfn(&timer_beep, (os_timer_func_t *) user_beep_timer_func, NULL);
			os_timer_arm(&timer_beep, 200, 0);
		}
	}else if(alarm_flag){
		user_beep_on(300);
	}
}


void ICACHE_FLASH_ATTR
user_beep_on(unsigned int beep_time) {
	GPIO_OUTPUT_SET(GPIO_BEEP_0_IO_NUM, BEEP_ON);
	os_timer_disarm(&timer_beep);
	os_timer_setfn(&timer_beep, (os_timer_func_t *) user_beep_timer_func, NULL);
	os_timer_arm(&timer_beep, beep_time, 0);
}





void ICACHE_FLASH_ATTR
user_beep_init(void) {

	GPIO_DIS_OUTPUT(GPIO_BEEP_0_IO_NUM);	//配置蜂鸣器GPIO0为输出
	//PIN_PULLUP_DIS(GPIO_BEEP_0_IO_MUX); //设置成开漏

	os_timer_disarm(&timer_beep);
	os_timer_setfn(&timer_beep, (os_timer_func_t *) user_beep_timer_func, NULL);
	//os_timer_arm(&timer_beep, 500, 0);	//30ms

}
