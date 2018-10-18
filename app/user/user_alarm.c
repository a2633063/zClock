#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "mem.h"
#include "user_interface.h"

#include "user_beep.h"
#include "user_alarm.h"

struct struct_alarm alarm[ALARM_COUNT];
unsigned char alarm_main_switch=0;



LOCAL os_timer_t timer_alarm;

void ICACHE_FLASH_ATTR user_alarm_timer_func(void *arg) {
}



void ICACHE_FLASH_ATTR
user_alarm_init(void) {

	os_timer_disarm(&timer_alarm);
	os_timer_setfn(&timer_alarm, (os_timer_func_t *) user_alarm_timer_func, NULL);
	//os_timer_arm(&timer_beep, 500, 0);	//30ms

}
