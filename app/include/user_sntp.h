#ifndef __USER_SNTP_H__
#define __USER_SNTP_H__




enum {
	Monday = 1, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday,
};
enum {
	January = 1, February, March, April, May, June, July, August, September, October, November, December,
};
struct struct_time {
	unsigned char year;
	unsigned char month;
	unsigned char day;
	unsigned char week;
	unsigned char hour;
	unsigned char minute;
	unsigned char second;

};

struct struct_time time;
void user_sntp_init(void);
void time_strtohex(char *sntp_time);


#endif

