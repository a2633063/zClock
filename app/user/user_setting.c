#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "mem.h"
#include "user_interface.h"

#include "user_setting.h"
#include "user_alarm.h"

#define	SETTING_SAVE_ALARM_SWITCH_ADDR  0xf0
#define	SETTING_SAVE_ALARM_ADDR  0xf4


//注意:struct_alarm结构体长度正好为4字节,如果修改,注意读写spi时4字节对齐

void ICACHE_FLASH_ATTR
user_setting_save_alarm_switch() {
	uint32 val=alarm_main_switch;
	spi_flash_erase_sector(SETTING_SAVE_ALARM_SWITCH_ADDR);
	spi_flash_write(SETTING_SAVE_ALARM_SWITCH_ADDR  * 4096,&val, 4);
}

void ICACHE_FLASH_ATTR
user_setting_get_alarm_switch() {
	uint32 val;
	spi_flash_read(SETTING_SAVE_ALARM_SWITCH_ADDR * 4096,&val, 4);
	alarm_main_switch=val&0xff;
}




void ICACHE_FLASH_ATTR
user_setting_save_alarm(unsigned char no) {
	spi_flash_erase_sector(SETTING_SAVE_ALARM_ADDR + no*4);

	uint32 value;
	value=alarm[no].on;
	value=(value<<8)|alarm[no].repeat;
	value=(value<<8)|alarm[no].hour;
	value=(value<<8)|alarm[no].minute;

	os_printf("\nSave Setting: %x,%x,%x,%x\n",alarm[no].on,alarm[no].repeat,alarm[no].hour,alarm[no].minute);
	spi_flash_write((SETTING_SAVE_ALARM_ADDR + no*4) * 4096, &value, 4);
}

void ICACHE_FLASH_ATTR
user_setting_get_alarm(unsigned char no) {

	uint32 value;
	spi_flash_read((SETTING_SAVE_ALARM_ADDR + no*4) * 4096, &value, 4);
	alarm[no].minute=value&0xff;
	alarm[no].hour=(value>>8)&0xff;
	alarm[no].repeat=(value>>16)&0xff;
	alarm[no].on=(value>>24)&0xff;
	//spi_flash_read((SETTING_SAVE_ALARM_ADDR + no*4) * 4096, (uint32 *)&alarm[no], 4);//直接使用此出错

}


void ICACHE_FLASH_ATTR
user_setting_init(void) {
	user_setting_get_alarm_switch();
	if(alarm_main_switch!=1) alarm_main_switch=0;
	os_printf("Get Setting alarm_main_switch:%d\n",alarm_main_switch);


	unsigned char i=0;
	for(i=0;i<ALARM_COUNT;i++)
	{
		user_setting_get_alarm(i);
		if(alarm[i].on>1){
			alarm[i].on=0;
			alarm[i].repeat=0;
			alarm[i].hour=0;
			alarm[i].minute=0;
			user_setting_save_alarm(i);
		}
		else {
			if(alarm[i].hour>23) alarm[i].hour=0;
			if(alarm[i].minute>59) alarm[i].minute=0;
		}

		os_printf("Alarm:	%d:%d,%x  %d:%d\n",i,alarm[i].on,alarm[i].repeat,alarm[i].hour,alarm[i].minute);
	}


}
