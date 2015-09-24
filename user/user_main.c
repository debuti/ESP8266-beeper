#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "ip_addr.h"
#include "espconn.h"
#include "user_interface.h"
#include "user_config.h"
#include "wifi.h"
#include "uart.h"
#include "gpio.h"

#define user_procTaskPrio        0
#define user_procTaskQueueLen    1

static void user_procTask(os_event_t *events);
os_event_t user_procTaskQueue[user_procTaskQueueLen];
static volatile os_timer_t some_timer;

struct BannedBSSID {
    u8 bssidValue[6];
    STAILQ_ENTRY(BannedBSSID) BannedBSSIDs;
} *temp;

STAILQ_HEAD(BannedBSSIDHeadType, BannedBSSID) BannedBSSIDHead;

bool isInitialized=false;
bool isScanRunning=false;

void wifi_cb(System_Event_t *evt) {
    os_printf(">>>%s: %d\n", __FUNCTION__, evt->event);
    
    switch (evt->event) {
        case EVENT_STAMODE_CONNECTED:
            os_printf("connected to ssid %s, channel %d\n",
                        evt->event_info.connected.ssid,
                        evt->event_info.connected.channel);
            break;

        case EVENT_STAMODE_DISCONNECTED:
            os_printf("disconnected from ssid %s, reason %d\n",
                        evt->event_info.disconnected.ssid,
                        evt->event_info.disconnected.reason);
            
            deep_sleep_set_option( 0 );
            system_deep_sleep( 60 * 1000 * 1000 );  // 60 seconds
            break;
        

        case EVENT_STAMODE_GOT_IP:
            os_printf("ip:" IPSTR ",mask:" IPSTR ",gw:" IPSTR,
                        IP2STR(&evt->event_info.got_ip.ip),
                        IP2STR(&evt->event_info.got_ip.mask),
                        IP2STR(&evt->event_info.got_ip.gw));
            os_printf("\n");
            break;
        
        default:
            break;
    }

    os_printf("<<<%s\n", __FUNCTION__);
}

static void wifiScan_cb(void *arg, STATUS status) {
    os_printf(">>>%s\n", __FUNCTION__);

    int numberOfAPsFound = 0;

    switch (status) {
        case OK:
            os_printf("Status OK\n");
            scaninfo *c = arg; 
            struct bss_info *inf; 
            if (!c->pbss) {
                os_printf("iwscan failed\n");
                return;
            }
            STAILQ_FOREACH(inf, c->pbss, next) {
                numberOfAPsFound++;
                os_printf("BSSID %02x:%02x:%02x:%02x:%02x:%02x channel %02d rssi %02d auth %16s %s\n", 
                        MAC2STR(inf->bssid),
                        inf->channel, 
                        inf->rssi, 
                        id_to_encryption_mode(inf->authmode),
                        inf->ssid
                    );
                if (inf->authmode == AUTH_OPEN) {
                    os_printf("Aqui entraré yeah\n");
                    struct station_config stationConf;
                    stationConf.bssid_set = 1;
                    os_memcpy(&stationConf.bssid, inf->bssid, 6);
                    wifi_station_set_config_current(&stationConf);
                    wifi_station_connect();
                }
                inf = (struct bss_info *) &inf->next;
            }

            break;
        case FAIL:
             os_printf("Status FAIL\n");
             break; 
        case PENDING:
             os_printf("Status PENDING\n");
             break; 
        case BUSY:
             os_printf("Status BUSY\n");
             break; 
        case CANCEL:
             os_printf("Status CANCEL\n");
             break; 
        default: 
             os_printf("Status UNKNOWN\n");
             break; 
        }

    os_printf("Number of APs found: %d\n", numberOfAPsFound);                    
    isScanRunning=false;
    os_printf("<<<%s\n", __FUNCTION__);
}

//Main loop function
void ICACHE_FLASH_ATTR loop(void *arg) {
    os_printf(">>>%s\n", __FUNCTION__ );

    if (!isScanRunning) {
        if (wifi_station_scan(NULL, wifiScan_cb)){
            isScanRunning=true;
            wifi_promiscuous_enable(0);
            os_printf("mode=%u\n",wifi_get_opmode());
            os_printf("set mode=%u\n", wifi_set_opmode(STATION_MODE));
            os_printf("mode_afterset=%u\n",wifi_get_opmode());
            os_printf("scan_result=%u\n",1);
        }   
    }

    os_printf("<<<%s\n", __FUNCTION__);
}

static void ICACHE_FLASH_ATTR system_init_done(void){
    os_printf(">>>%s\n", __FUNCTION__ );
    
    isInitialized=true;

    //Disarm timer
    os_timer_disarm(&some_timer);
    //Setup timer
    os_timer_setfn(&some_timer, (os_timer_func_t *)loop, NULL);
    //Arm the timer
    os_timer_arm(&some_timer, 3 * 1000, 1);

    os_printf("<<<%s\n", __FUNCTION__);
}

//void ICACHE_FLASH_ATTR user_rf_pre_init(void) {}

//Do nothing function
static void ICACHE_FLASH_ATTR user_procTask(os_event_t *events) {
    os_delay_us(10);
}

/** Entry point **/
void ICACHE_FLASH_ATTR user_init( void ) {
    os_printf(">>>%s\n", __FUNCTION__ );
    os_delay_us(1*1000*1000);
    isInitialized=false;
    STAILQ_INIT(&BannedBSSIDHead);
    uartInit();
    gpioInit();
    wifiInit();
    wifi_set_event_handler_cb(wifi_cb);
    system_init_done_cb(&system_init_done);

    //Start os task
    system_os_task(user_procTask,
                   user_procTaskPrio, 
                   user_procTaskQueue, 
                   user_procTaskQueueLen);

//    system_os_post(user_procTaskPrio, 
//                   0, 
//                   0);
    os_printf("<<<%s\n", __FUNCTION__);
}


