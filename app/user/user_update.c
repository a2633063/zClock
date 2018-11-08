#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "mem.h"
#include "user_interface.h"

#include "../include/upgrade.h"
#include "../include/espconn.h"
#include "user_update.h"

#include "user_led.h"

void ICACHE_FLASH_ATTR ota_finished_callback(void *arg) {
	struct upgrade_server_info *update = arg;
	if (update->upgrade_flag == 1) {
		os_printf("[OTA]success; rebooting!\n");
		system_upgrade_reboot();
	} else {
		os_printf("[OTA]failed!\n");
	}

	os_free(update->pespconn);
	os_free(update->url);
	os_free(update);
}

void ICACHE_FLASH_ATTR ota_start_Upgrade(uint8_t serverVersion, const char *server_ip, uint16_t port, const char *path) {
	const char* file;
	uint8_t userBin = system_upgrade_userbin_check();
	switch (userBin) {
	case UPGRADE_FW_BIN1:
		file = "user2.4096.new.6.bin";
		break; //  user2.bin
	case UPGRADE_FW_BIN2:
		file = "user1.4096.new.6.bin";
		break; // user1.bin
	default:
		os_printf("[OTA]Invalid userbin number!\n");
		user_set_led(1);
		return;
	}

	uint16_t version = 1;
	if (serverVersion <= version) {
		os_printf("[OTA]No update. Server version:%d, local version %d\n", serverVersion, version);

		user_set_led(1);
		return;
	}
	os_printf("[OTA]Upgrade available version: %d\n", serverVersion);

	struct upgrade_server_info* update = (struct upgrade_server_info *) os_zalloc(sizeof(struct upgrade_server_info));
	update->pespconn = (struct espconn *) os_zalloc(sizeof(struct espconn));

	os_memcpy(update->ip, server_ip, 4);
	update->port = port;
	os_printf("[OTA]Server "IPSTR":%d. Path: [%s],file: [%s]\n", IP2STR(update->ip), update->port, path, file);
	update->check_cb = ota_finished_callback;
	update->check_times = 10000;
	update->url = (uint8 *) os_zalloc(512);

	os_sprintf((char*) update->url, "GET /%s%s HTTP/1.1\r\n"
			"Host: "IPSTR":%d\r\n"
	"Connection: keep-alive\r\n"
	"\r\n", path, file, IP2STR(update->ip), update->port);

	if (system_upgrade_start(update) == false) {
		os_printf("[OTA]Could not start upgrade\n");
		os_free(update->pespconn);
		os_free(update->url);
		os_free(update);
		user_set_led(1);
	} else {
		os_printf("[OTA]Upgrading...\n");
		user_set_led(0);
	}
}

void ICACHE_FLASH_ATTR user_update(char a,char b,char c,char d) {
	char serverip[4] ;
	serverip[0]=a;
	serverip[1]=b;
	serverip[2]=c;
	serverip[3]=d;
	char path[] = "ESP/";
	ota_start_Upgrade(2, serverip, 80, path);
	user_set_led(0);
}
