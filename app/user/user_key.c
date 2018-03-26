#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "mem.h"
#include "user_interface.h"

#include "driver\key.h"
#include "user_key.h"
#include "user_led.h"
#include "user_wifi.h"

#include "user_tm1628.h"
LOCAL struct keys_param keys;
LOCAL struct single_key_param *single_key[GPIO_KEY_NUM];

LOCAL unsigned char key_press_flag = 0;	//按键长按标志位,防止按键长按后松开时执行短按代码

LOCAL void ICACHE_FLASH_ATTR
user_key0_short_press(void) {
	if (key_press_flag == 1) {	//防止按键长按后松开时执行短按代码
		key_press_flag = 0;
		return;
	}
	user_smartconfig_stop();
	os_printf("key0_short_press\n");
	if(++brightness>7) brightness=0;

}

LOCAL void ICACHE_FLASH_ATTR
user_key0_long_press(void) {
	key_press_flag = 1;
	os_printf("key0_long_press\n");
	user_smartconfig();
}


LOCAL void ICACHE_FLASH_ATTR
user_key1_short_press(void) {
	if (key_press_flag == 1) {	//防止按键长按后松开时执行短按代码
		key_press_flag = 0;
		return;
	}
	user_smartconfig_stop();
	os_printf("key1_short_press\n");
	display[0]++;
}

LOCAL void ICACHE_FLASH_ATTR
user_key1_long_press(void) {
	key_press_flag = 1;
	os_printf("key1_long_press\n");
	user_smartconfig();
}

void ICACHE_FLASH_ATTR
user_key_init(void) {
	single_key[0] = key_init_single(GPIO_KEY_0_IO_NUM, GPIO_KEY_0_IO_MUX,
	GPIO_KEY_0_IO_FUNC, user_key0_long_press, user_key0_short_press);

//	single_key[1] = key_init_single(GPIO_KEY_1_IO_NUM, GPIO_KEY_1_IO_MUX,
//	GPIO_KEY_1_IO_FUNC, user_key1_long_press, user_key1_short_press);


	keys.key_num = GPIO_KEY_NUM;
	keys.single_key = single_key;
	key_init(&keys);
}
