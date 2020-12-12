#ifndef _EZWIFI_H_
#define _EZWIFI_H_

#include "Arduino.h"
#include "Preferences.h"
#include <ezWidget.h>
#include <WiFi.h>

#define EZWIFI_WIFI_INTERVAL       500
#define EZWIFI_WIDGET_INTERVAL    2000
#define EZWIFI_CUTOFFS { 0, 20, 40, 70 }
#define EZWIFI_BAR_W                 4
#define EZWIFI_BAR_GAP               2


class WifiBars : public ezWidget {
 public:
  WifiBars(ezWidget& parentWidget,
           int16_t x_ = EZ_INVALID, int16_t y_ = EZ_INVALID,
           int16_t w_ = 0, int16_t h_ = 0);
  WifiBars(int16_t x_ = EZ_INVALID, int16_t y_ = EZ_INVALID,
           int16_t w_ = 0, int16_t h_ = 0);
  void draw();
};

enum WifiState_t {
  EZWIFI_NOT_INIT,
  EZWIFI_WAITING,
  EZWIFI_IDLE,
  EZWIFI_SCANNING,
  EZWIFI_CONNECTING,
  EZWIFI_AUTOCONNECT_DISABLED
};

struct WifiNetwork_t {
  String SSID;
  String key;
};

class ezWifiClass {
 public:
  static ezWifiClass& instance() {
    static ezWifiClass INSTANCE;
    return INSTANCE;
  }
  ezWifiClass(ezWifiClass const&)     = delete;
  void operator=(ezWifiClass const&)  = delete;
 private:
  ezWifiClass();

  public:
    void begin();
    void update();
    void add(String ssid, String key);
    bool remove(int8_t index);
    bool remove(String ssid);
    int8_t indexForSSID(String ssid);
    void readFlash();
    void writeFlash();
    void menu();
    std::vector<WifiNetwork_t> networks;
    bool autoConnect;
  private:
    int16_t _menuInner();
    void _wps();
    WifiState_t _state;
    uint8_t _current_from_scan;
    uint32_t _wait_until, _wifi_time, _widget_time;

  public:
    WiFiEvent_t _WPS_event;
    String _WPS_pin;
    bool _WPS_new_event;
};

extern ezWifiClass& ezWifi;

#endif /* _EZWIFI_H_ */
