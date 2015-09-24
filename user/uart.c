#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "ip_addr.h"
#include "espconn.h"
#include "user_interface.h"
#include "user_config.h"
#include "uart.h"

/** UART Functions **/
void ICACHE_FLASH_ATTR uartInit() {
    os_printf(">>>%s\n", __FUNCTION__ );
    uart_div_modify( 0, UART_CLK_FREQ / ( 115200 ) );
    os_printf("<<<%s\n", __FUNCTION__);
}

