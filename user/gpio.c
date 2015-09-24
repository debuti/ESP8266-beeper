#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "ip_addr.h"
#include "espconn.h"
#include "user_interface.h"
#include "user_config.h"
#include "gpio.h"

/** GPIO Functions **/
void ICACHE_FLASH_ATTR gpioInit() {
    os_printf(">>>%s\n", __FUNCTION__ );
    gpio_init();
    os_printf("<<<%s\n", __FUNCTION__);
}

