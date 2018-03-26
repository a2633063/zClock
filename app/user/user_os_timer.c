/******************************************************************************
 * Copyright 2013-2014 Espressif Systems (Wuxi)
 *
 * FileName: user_main.c
 *
 * Description: entry file of user application
 *
 * Modification history:
 *     2015/1/23, v1.0 create this file.
 *******************************************************************************/
#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "user_interface.h"

#include "user_key.h"
#include "user_sntp.h"
#include "user_tm1628.h"

LOCAL os_timer_t timer_rudder;

static unsigned int tim = 55, tim1 = 0;

const unsigned char month_max[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

void ICACHE_FLASH_ATTR user_os_timer_func(void *arg) {
	static unsigned char second_temp = 60;
	if (++tim1 >= 50) {	//1s
		tim1 = 0;
		if (++tim > 60 - second_temp &&wifi_station_get_connect_status() == STATION_GOT_IP) {	//10s
			{
				tim = 1;
				user_check_sntp_stamp();
				if (time.year != 0) {
					user_tm1628_time_refresh();
					second_temp = time.second;
				}
			}
		} else {
			time.second++;
			if (time.second > 59) {
				time.second = 0;
				time.minute++;
				if (time.minute > 59) {
					time.minute = 0;
					time.hour++;
					if (time.hour > 23) {
						time.hour = 0;
						time.week++;
						if (time.week > Sunday)
							time.week = Monday;
						time.day++;
						if (time.day
								> month_max[time.month] + ((time.month == February && time.year % 4 == 0) ? 1 : 0)) {
							time.day = 0;
							time.month++;
							if (time.month > December) {
								time.month = January;
								time.year++;
								time.year %= 100;
							}
						}
					}
				}
			}

			user_tm1628_time_refresh();
		}
	}

}

void ICACHE_FLASH_ATTR
user_os_timer_init(void) {
	os_timer_disarm(&timer_rudder);
	os_timer_setfn(&timer_rudder, (os_timer_func_t *) user_os_timer_func, NULL);
	os_timer_arm(&timer_rudder, 20, 1);
}
