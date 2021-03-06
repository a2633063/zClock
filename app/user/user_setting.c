#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "mem.h"
#include "user_interface.h"

#include "user_config.h"
#include "user_setting.h"
#include "user_wifi.h"

void ICACHE_FLASH_ATTR
user_setting_init(void) {
	user_setting_get_config();

	os_printf("Device name:\"%s\"\r\n", user_config.name);
	os_printf("MQTT Service ip:\"%s:%d\"\r\n", user_config.mqtt_ip, user_config.mqtt_port);
	os_printf("MQTT Service user:\"%s\"\r\n", user_config.mqtt_user);
	os_printf("MQTT Service password:\"%s\"\r\n", user_config.mqtt_password);

}

void ICACHE_FLASH_ATTR
user_setting_set_config(void) {

	uint16_t i, j;
	uint32_t length = sizeof(user_config_t);
	if (length % 4 != 0)
		length += 4 - length % 4;	// 4 �ֽڶ��롣
	uint8_t *p = (uint8_t *) os_malloc(length);
	os_memcpy(p, &user_config, length);
	spi_flash_erase_sector(SETTING_SAVE_ADDR);
	spi_flash_write(SETTING_SAVE_ADDR * 4096, (uint32 *) p, length);

	os_free(p);
}

void ICACHE_FLASH_ATTR
user_setting_get_config(void) {
	uint32 val;

	uint16_t i, j;
	uint32_t length = sizeof(user_config_t);
	if (length % 4 != 0)
		length += 4 - length % 4;	// 4 �ֽڶ��롣
	uint8_t *p = (uint8_t *) os_malloc(length);

	spi_flash_read(SETTING_SAVE_ADDR * 4096, (uint32 *) p, length);

	os_memcpy(&user_config, p, length);
	os_free(p);

	if (user_config.direction != 0 && user_config.direction != 1)
		user_config.direction = 0;

//	os_printf("user_config.name[0]:0x%02x 0x%02x 0x%02x\r\n", user_config.name[0],user_config.name[1],user_config.name[2]);
	if (user_config.name[0] == 0xff && user_config.name[1] == 0xff && user_config.name[2] == 0xff || user_config.version != USER_CONFIG_VERSION) {

		wifi_get_macaddr(STATION_IF, hwaddr);
		os_sprintf(user_config.name, DEVICE_NAME, hwaddr[4], hwaddr[5]);
		os_sprintf(user_config.mqtt_ip, "");
		os_sprintf(user_config.mqtt_user, "");
		os_sprintf(user_config.mqtt_password, "");
		user_config.mqtt_port = 1883;
		user_config.version = USER_CONFIG_VERSION;
		user_config.direction = 0;
		user_setting_set_config();
	}
}

