#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "ip_addr.h"
#include "espconn.h"
#include "user_interface.h"
#include "user_config.h"
#include "wifi.h"

/** WIFI Functions **/
void wifiInit() {
    os_printf(">>>%s\n", __FUNCTION__);
    //wifi_station_set_hostname("autolocator");
    wifi_set_opmode(STATION_MODE);

  /*// Check the opmode
  ret = wifi_get_opmode();

  os_sprintf("Get op mode");
  if (ret != STATION_MODE){
    os_sprintf("STATION:MODE");
    // Wifi Operation Mode is STATION, which is set by default
    wifi_set_opmode(STATION_MODE);

    // Restart
    system_restart();
  }*/

    os_printf("<<<%s\n", __FUNCTION__);
}

void wifiConnect( void ) {
    static struct station_config config;
    config.bssid_set = 0;
    os_memcpy( &config.ssid, SSID, 32 );
    os_memcpy( &config.password, SSID_PASSWORD, 64 );
    wifi_station_set_config( &config );
}

const char *id_to_encryption_mode(int id) {
    switch (id) {
        case AUTH_OPEN:
            return "AUTH_OPEN";
            break;
        case AUTH_WEP:
            return "AUTH_WEP";
            break;
        case AUTH_WPA_PSK:
            return "AUTH_WPA_PSK";
            break; 
        case AUTH_WPA2_PSK:
            return "AUTH_WPA2_PSK";
            break; 
        case AUTH_WPA_WPA2_PSK:
            return "AUTH_WPA_WPA2_PSK";
            break; 
        case AUTH_MAX:
            return "AUTH_MAX";
            break; 
        default: 
            return "UNKNOWN";
            break; 
        }
}

/*static void ICACHE_FLASH_ATTR wifiScan_cb(void *arg, STATUS status) {
    os_printf(">>>%s\n", __FUNCTION__);

    uint8 ssid[33];
    uint8 numberOfAPsFound=0;

    switch (status ) {
    case OK:
        os_printf("Status OK\n");
        struct bss_info *bss_link = (struct bss_info *)arg;
        bss_link = bss_link->next.stqe_next;//ignore first

        while (bss_link != NULL) {
          numberOfAPsFound++;

          os_memset(ssid, 0, 33);
          if (os_strlen(bss_link->ssid) <= 32) os_memcpy(ssid, bss_link->ssid, os_strlen(bss_link->ssid));
          else os_memcpy(ssid, bss_link->ssid, 32);
          
          os_sprintf("%s\r\n", ssid);
      
          os_sprintf("+CWLAP:(%d,\"%s\",%d,\""MACSTR"\",%d)\r\n",
                     bss_link->authmode, 
                     ssid, 
                     bss_link->rssi,
                     MAC2STR(bss_link->bssid),
                     bss_link->channel);
          
          bss_link = bss_link->next.stqe_next;
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

    os_sprintf("%d\r\n", numberOfAPsFound);
    isScanRunning=false;
    os_printf("<<<%s\n", __FUNCTION__);
}*/

//void wifiScan_handler_cb
//    wifi_set_event_handler_cb( wifi_callback );

/**void ICACHE_FLASH_ATTR wifi_callback( System_Event_t *evt ) {
    os_printf(">>>%s: %d\n", __FUNCTION__, evt->event );
    
    switch ( evt->event ) {
        case EVENT_STAMODE_CONNECTED:
        {
            os_printf("connect to ssid %s, channel %d\n",
                        evt->event_info.connected.ssid,
                        evt->event_info.connected.channel);
            break;
        }

        case EVENT_STAMODE_DISCONNECTED:
        {
            os_printf("disconnect from ssid %s, reason %d\n",
                        evt->event_info.disconnected.ssid,
                        evt->event_info.disconnected.reason);
            
            deep_sleep_set_option( 0 );
            system_deep_sleep( 60 * 1000 * 1000 );  // 60 seconds
            break;
        }

        case EVENT_STAMODE_GOT_IP:
        {
            os_printf("ip:" IPSTR ",mask:" IPSTR ",gw:" IPSTR,
                        IP2STR(&evt->event_info.got_ip.ip),
                        IP2STR(&evt->event_info.got_ip.mask),
                        IP2STR(&evt->event_info.got_ip.gw));
            os_printf("\n");
            
            espconn_gethostbyname( &dweet_conn, dweet_host, &dweet_ip, dns_done );
            break;
        }
        
        default:
        {
            break;
        }
    }
    os_printf("<<<%s\n", __FUNCTION__);
}
**/

