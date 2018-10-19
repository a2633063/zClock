#ifndef __USER_ALARM_H__
#define __USER_ALARM_H__

#include "user_sntp.h"

#define ALARM_COUNT 5

struct struct_alarm {
	unsigned char on;
	unsigned char repeat;
	unsigned char hour;
	unsigned char minute;
};

struct struct_alarm alarm[ALARM_COUNT];

unsigned char alarm_main_switch;
unsigned int alarm_flag;


void user_alarm_init(void);
void user_alarm_close(void);
void user_alarm_check(struct struct_time t);
#endif
