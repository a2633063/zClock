#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "mem.h"
#include "user_interface.h"

#include "user_key.h"
#include "user_led.h"
#include "user_wifi.h"

#include "user_tm1628.h"
#include "user_beep.h"
#include "user_alarm.h"

#define KEY_LONG_PRESS_TIME 50
LOCAL os_timer_t timer_key;

LOCAL unsigned char key_state=0;
LOCAL unsigned char key_time=0;

LOCAL void ICACHE_FLASH_ATTR
user_key0_short_press(void) {
	os_printf("key0_short_press\n");
	if(alarm_flag)
		user_alarm_close();
	else{
		user_beep_on(100);
		if(++brightness>7) brightness=0;
	}
}

LOCAL void ICACHE_FLASH_ATTR
user_key0_long_press(void) {
	if(alarm_flag)
		user_alarm_close();
	else{
		user_beep_on(200);
		os_printf("key0_long_press\n");
		auto_brightness=!auto_brightness;
	}
}

void ICACHE_FLASH_ATTR user_key_timer_func(void *arg) {

	unsigned char state=gpio16_input_get();

	if(key_state==0 && state==0){
		key_state=1;
	}else if(key_state==1 && state==0){
		key_state=2;
	}else if(key_state==2 && state==0){
		if(key_time++>KEY_LONG_PRESS_TIME) {
			user_key0_long_press();
			key_state=3;
		}
	}else if(key_state==2 && state==1){
		if(key_time<KEY_LONG_PRESS_TIME) user_key0_short_press();
		key_state=3;
	}else if(key_state==3 && state==0){

	}else{
		key_state=1;
		key_time=0;
	}

}





void ICACHE_FLASH_ATTR
user_key_init(void) {
	//GPIO16不支持外部中断,所以无法直接使用key driver
	gpio16_input_conf();	//配置按键GPIO16为输入


	os_timer_disarm(&timer_key);
	os_timer_setfn(&timer_key, (os_timer_func_t *) user_key_timer_func, NULL);
	os_timer_arm(&timer_key, 30, 1);	//30ms

}
