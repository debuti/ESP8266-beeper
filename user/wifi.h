#ifndef WIFI_H
#define WIFI_H

/** WIFI Functions **/
void ICACHE_FLASH_ATTR wifiInit();
const char ICACHE_FLASH_ATTR *id_to_encryption_mode(int id);

#endif

