#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "mem.h"
#include "user_interface.h"

#include "driver\key.h"
#include "user_led.h"

void ICACHE_FLASH_ATTR
user_led_init(void) {
	//PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO0_U);
	PIN_FUNC_SELECT(GPIO_LED_0_IO_MUX, GPIO_LED_0_IO_FUNC);
	user_set_led(1);
}


unsigned char ICACHE_FLASH_ATTR user_get_led(){
	return GPIO_INPUT_GET(GPIO_ID_PIN(GPIO_LED_0_IO_NUM));
}

void ICACHE_FLASH_ATTR user_set_led(unsigned char level){
	GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_LED_0_IO_NUM), level);
}
