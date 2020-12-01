// #ifndef _EZWIFI_H_
// #define _EZWIFI_H_
//
// #include "Arduino.h"
// #include "Preferences.h"
//
// #define EZWIFI_WIFI_INTERVAL       250
// #define EZWIFI_WIDGET_INTERVAL    2000
//
// enum WifiState_t {
//   EZWIFI_NOT_INIT,
//   EZWIFI_WAITING,
//   EZWIFI_IDLE,
//   EZWIFI_SCANNING,
//   EZWIFI_CONNECTING,
//   EZWIFI_AUTOCONNECT_DISABLED
// };
//
// struct WifiNetwork_t {
//   String SSID;
//   String key;
// };
//
// class ezWifiClass {
//  public:
//   static ezWifiClass& instance() {
//     static ezWifiClass INSTANCE;
//     return INSTANCE;
//   }
//   ezWifiClass(ezWifiClass const&)     = delete;
//   void operator=(ezWifiClass const&)  = delete;
//  private:
//   ezWifiClass();
//
//   public:
//     void begin();
//     void update();
//     void add(String ssid, String key);
//     bool remove(int8_t index);
//     bool remove(String ssid);
//     int8_t indexForSSID(String ssid);
//     void readFlash();
//     void writeFlash();
//     void menu();
//     std::vector<WifiNetwork_t> networks;
//     bool autoConnect;
//   private:
//     WifiState_t _state;
//     uint8_t _current_from_scan;
//     uint32_t _wait_until, _wifi_time, _widget_time;
//     void _drawWidget(uint16_t x, uint16_t w);
//     bool _onOff(ezMenu* callingMenu);
//     void _manageAutoconnects();
//     bool _autoconnectSelected(ezMenu* callingMenu);
//     void _askAdd();
//     bool _connection(ezMenu* callingMenu);
//     #ifdef M5EZ_WPS
//       void _WPShelper(WiFiEvent_t event, system_event_info_t info);
//       WiFiEvent_t _WPS_event;
//       String _WPS_pin;
//       bool _WPS_new_event;
//     #endif
// };
//
// extern ezWifiClass& ezWifi;
//
// #endif /* _EZWIFI_H_ */
