#ifndef __USER_SETTING_H__
#define __USER_SETTING_H__


void user_setting_save_alarm_switch() ;
void user_setting_get_alarm_switch();

void user_setting_save_alarm(unsigned char no);
void user_setting_get_alarm(unsigned char no);

void user_setting_init(void);
#endif
