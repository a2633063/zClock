#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "mem.h"
#include "user_interface.h"

#include "user_beep.h"
#include "user_alarm.h"

struct struct_alarm alarm[ALARM_COUNT];
unsigned char alarm_main_switch=0;
unsigned int alarm_flag=0;



LOCAL os_timer_t timer_alarm;

void ICACHE_FLASH_ATTR user_alarm_timer_func(void *arg) {
	user_alarm_close();
}

void ICACHE_FLASH_ATTR user_alarm_check(struct struct_time t){
	if(alarm_main_switch==0 || alarm_flag!=0) return;
	unsigned char i=0;
	for(i=0;i<ALARM_COUNT;i++){
		if(alarm[i].on==1 && t.second==0 && t.minute==alarm[i].minute && t.hour==alarm[i].hour){
			if(alarm[i].repeat==0 || alarm[i].repeat>=0x80 || alarm[i].repeat&(0x01<<(t.week-1))){	//响一次/每天/周X
				alarm_flag|=(1<<i);
			}
		}
	}
	if(alarm_flag!=0){
		//闹钟启动,启动蜂鸣器
		os_printf("alarm start:%x\n",alarm_flag);
		user_beep_on(300);
		os_timer_disarm(&timer_alarm);
		os_timer_setfn(&timer_alarm, (os_timer_func_t *) user_alarm_timer_func, NULL);
		os_timer_arm(&timer_alarm, 50000, 0);	//50s后关闭闹钟
	}
}

void ICACHE_FLASH_ATTR user_alarm_close(){
	unsigned int i=0;
	if(alarm_flag==0) return;
	for(i=0;i<8;i++){
		if((alarm_flag>>i)&0x01){
			if(alarm[i].repeat==0) {
				alarm[i].on=0;
//				user_setting_save_alarm(i);
			}
			alarm_flag &=(~(0x01<<i));
		}
	}
	os_printf("alarm close:%x\n",alarm_flag);
	if(alarm_flag!=0){
		os_printf("user_alarm_close ERROR:%x\n",alarm_flag);
		alarm_flag=0;
	}
}


void ICACHE_FLASH_ATTR
user_alarm_init(void) {

//	os_timer_disarm(&timer_alarm);
//	os_timer_setfn(&timer_alarm, (os_timer_func_t *) user_alarm_timer_func, NULL);
	//os_timer_arm(&timer_beep, 500, 0);	//30ms

}
