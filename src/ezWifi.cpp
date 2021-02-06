#include "ezWifi.h"
#include <ez.h>
extern "C" {
  #include "esp_wifi.h"
  #include "esp_wps.h"
}

#define EZWIFI  ezWifiClass::instance()

WifiBars::WifiBars(ezWidget& parentWidget,
                   int16_t x_ /* = EZ_INVALID */,
                   int16_t y_ /* = EZ_INVALID */,
                   int16_t w_ /* = 0 */, int16_t h_ /* = 0 */) {
  set(x_, y_, w_, h_);
  parentWidget.add(*this);
}

WifiBars::WifiBars(int16_t x_ /* = EZ_INVALID */,
                   int16_t y_ /* = EZ_INVALID */,
                   int16_t w_ /* = 0 */, int16_t h_ /* = 0 */) {
  set(x_, y_, w_, h_);
  ezScreen.add(*this);
}

void WifiBars::draw() {
  if (parent()) colors = parent()->colors;
  const uint8_t cutoffs[] = EZWIFI_CUTOFFS;
  uint8_t max_bars = sizeof(cutoffs);
  uint8_t bars = 0;
  if (WiFi.isConnected()) {
    uint8_t signal = map(100 + WiFi.RSSI(), 5, 90, 0, 100);
    // Determine how many bars signal is.
    for (uint8_t n = 0; n < sizeof(cutoffs); n++) {
      if (signal >= cutoffs[n]) bars = n + 1;
    }
  }
  uint8_t top = h / 10;
  uint8_t max_len = h * 0.8;
  uint16_t bg = findFill();
  uint8_t this_len;
  for (uint8_t n = 0; n < max_bars; n++) {
    this_len = ((float) (n + 1) / max_bars) * max_len;
    fillRect(lmargin + n * (EZWIFI_BAR_W + EZWIFI_BAR_GAP),
             top + max_len - this_len, EZWIFI_BAR_W, this_len,
             n + 1 <= bars ? colors.text : bg );
  }
}

ezWifiClass::ezWifiClass() {  };

void ezWifiClass::begin() {
  log_d("Initialising");
  WiFi.mode(WIFI_MODE_STA);
  WiFi.setAutoConnect(false);    // We have our own multi-AP version of this
  WiFi.setAutoReconnect(false);  // So we turn off the ESP32's versions
  WiFi.setHostname("M5Stack");
  readFlash();
  _state = EZWIFI_IDLE;
  WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info) {
    log_d("STA_DISCONNECTED detected. To EZWIFI_SCANNING.");
    EZWIFI._state = EZWIFI_SCANNING;
  }, WiFiEvent_t::SYSTEM_EVENT_STA_DISCONNECTED);
}

void ezWifiClass::update() {
  if (millis() <= _wifi_time + EZWIFI_WIFI_INTERVAL) return;
  _wifi_time = millis();
  if (millis() > _widget_time + EZWIFI_WIDGET_INTERVAL) {
    if (ezHeader.displayed()) {
      ezHeader.wifi.draw();
      ezHeader.wifi.refresh();
    }
    _widget_time = millis();
  }
  if (WiFi.isConnected() && _state != EZWIFI_AUTOCONNECT_DISABLED &&
      _state != EZWIFI_IDLE) {
    _state = EZWIFI_IDLE;
    log_d("Connected, returning to EZWIFI_IDLE");
  }
  if (!autoConnect || WiFi.isConnected() || networks.size() == 0) return;
  int8_t scanresult;
  switch(_state) {
    case EZWIFI_WAITING:
      if (millis() < _wait_until) return;
      // intentional fall-through
    case EZWIFI_IDLE:
      log_d("EZWIFI_IDLE. To EZWIFI_SCANNING.");
      WiFi.mode(WIFI_MODE_STA);
      WiFi.scanNetworks(true);
      _current_from_scan = 0;
      _state = EZWIFI_SCANNING;
      _wait_until = millis() + 10000;
      break;
    case EZWIFI_SCANNING:
      scanresult = WiFi.scanComplete();
      switch(scanresult) {
        case WIFI_SCAN_RUNNING:
          break;
        case WIFI_SCAN_FAILED:
          log_d("Scan failed. To EZWIFI_WAITING (60 sec.)");
          _state = EZWIFI_WAITING;
          _wait_until = millis() + 60000;
          WiFi.scanDelete();
          return;
        default:
          log_d("Scan got %d networks", scanresult);
          for (uint8_t n = _current_from_scan; n < scanresult; n++) {
            for (uint8_t m = 0; m < networks.size(); m++) {
              String ssid = networks[m].SSID;
              String key = networks[m].key;
              if (ssid == WiFi.SSID(n)) {
                log_d("Match: %s. To EZWIFI_CONNECTING", WiFi.SSID(n));
                WiFi.mode(WIFI_MODE_STA);
                WiFi.begin(ssid.c_str(), key.c_str());
                _state = EZWIFI_CONNECTING;
                _wait_until = millis() + 8000;
                return;
              }
            }
          }
          log_d("No (further) matches. To EZWIFI_WAITING (60 sec.)");
          _state = EZWIFI_WAITING;
          _wait_until = millis() + 60000;
          WiFi.scanDelete();
        //
      }
      break;
    case EZWIFI_CONNECTING:
      if (millis() > _wait_until) {
        log_d("Connect timed out. To EZWIFI_SCANNING");
        WiFi.disconnect();
        _current_from_scan++;
        _state = EZWIFI_SCANNING;
      }
      break;
    case EZWIFI_AUTOCONNECT_DISABLED:
      log_d("_state = EZWIFI_AUTOCONNECT_DISABLED");
      break;
    default:
      log_d("default case.. _state = %d", _state);
      break;
    //
  }
}

void ezWifiClass::add(String ssid, String key){
  WifiNetwork_t new_net;
  new_net.SSID = ssid;
  new_net.key = key;
  networks.push_back(new_net);
}

bool ezWifiClass::remove(int8_t index) {
  if (index < 0 || index >= networks.size()) return false;
  networks.erase(networks.begin() + index);
  return true;
}

bool ezWifiClass::remove(String ssid) { return remove(indexForSSID(ssid)); }

int8_t ezWifiClass::indexForSSID(String ssid) {
  for (uint8_t n = 0; n < networks.size(); n++) {
    if (networks[n].SSID == ssid) return n;
  }
  return -1;
}

void ezWifiClass::readFlash() {
  Preferences prefs;
  networks.clear();
  prefs.begin("M5ez", true);  // true: read-only
  autoConnect = prefs.getBool("autoconnect_on", true);
  log_d("EZWIFI readFlash: Autoconnect is %s", autoConnect ? "ON" : "OFF");
  WifiNetwork_t new_net;
  String idx;
  uint8_t index = 1;
  while (true) {
    idx = "SSID" + (String)index;
    String ssid = prefs.getString(idx.c_str(), "");
    idx = "key" + (String)index;
    String key = prefs.getString(idx.c_str(), "");
    if (ssid != "") {
      new_net.SSID = ssid;
      new_net.key = key;
      networks.push_back(new_net);
      log_d("EZWIFI readFlash: Read ssid: %s, key: %s", ssid, key);
      index++;
    } else {
      break;
    }
  }
  prefs.end();
}

void ezWifiClass::writeFlash() {
  Preferences prefs;
  String idx;
  uint8_t n = 1;
  prefs.begin("M5ez", false);
  // Remove unknown number of items from NVS, sequentially named SSID1 to
  // SSIDN, and key1 to keyN where N is the total number of WiFi Networks
  // stored (which may be different than networks.size() at this point.)
  while (true) {
    idx = "SSID" + (String)n;
    if(!prefs.remove(idx.c_str())) break;
    idx = "key" + (String)n;
    prefs.remove(idx.c_str());
    n++;
  }
  prefs.putBool("autoconnect_on", autoConnect);
  log_d("EZWIFI writeFlash: Autoconnect is %s", autoConnect ? "ON" : "OFF");
  for (n = 0; n < networks.size(); n++) {
    idx = "SSID" + (String)(n + 1);
    prefs.putString(idx.c_str(), networks[n].SSID);
    if (networks[n].key != "") {
      idx = "key" + (String)(n + 1);
      prefs.putString(idx.c_str(), networks[n].key);
      log_d("EZWIFI writeFlash: Wrote ssid: %s, key: %s", networks[n].SSID,
            networks[n].key);
    }
  }
  prefs.end();
}

void ezWifiClass::menu() {
  _state = EZWIFI_AUTOCONNECT_DISABLED;
  log_d("Disabling autoconnect while in Wifi menu.");
  while (true) {
    if (_menuInner() == EZ_BACK) break;
  }
  if (_state == EZWIFI_AUTOCONNECT_DISABLED) {
    _state = EZWIFI_IDLE;
    log_d("Enabling autoconnect exiting Wifi menu.");
  }
}

int16_t ezWifiClass::_menuInner() {

  ezMenu& wifimain = *new ezMenu("Wifi settings");
  wifimain.addItem("Autoconnect (" +
                   (String)(EZWIFI.autoConnect ? "On)" : "Off)"), doFunction {
    EZWIFI.autoConnect = !EZWIFI.autoConnect;
    EZWIFI.writeFlash();
  });
  if (WiFi.isConnected()) {
    wifimain.addItem("Connected: " + WiFi.SSID(), doFunction {
      ezMsgBox connection("Wifi connection", "", "Disconnect", "Back");
      connection.buttons[0]->setPos.w = -7;

      char tmp[100];
      sprintf(tmp, "SSID: %s\nkey: %s, MAC: %s / IP: %s", WiFi.SSID(), WiFi.psk().c_str(), WiFi.macAddress().c_str(), WiFi.localIP().toString().c_str());
      connection.label.text = tmp;
      connection.label.font = FSS9;

//       fieldnames.text = "SSID:\nKey:\nIP:\nMAC:\nMAC:\nHostname:\nRouter:\nBSSID:\nDNS:";
//       values.text     = (String)WiFi.SSID() + "\n" +
//                         (String)WiFi.psk() + "\n" +
//                         WiFi.localIP().toString() + "\n" +
//                         (String)WiFi.macAddress() + "\n" +
//                         (String)WiFi.getHostname() + "\n" +
//                         WiFi.gatewayIP().toString() + "\n" +
//                         (String)WiFi.BSSIDstr() + "\n" +
//                         (String)WiFi.dnsIP(0).toString() + "\n";
//
//       if (WiFi.dnsIP(1)) values.text += WiFi.dnsIP(1).toString();
//
//       fieldnames.align                  = EZ_RIGHT;
//       values.align                      = EZ_LEFT;
//       fieldnames.valign = values.valign = EZ_TOP;
//       fieldnames.font   = values.font   = FSS9;
//       fieldnames.wrap   = values.wrap   = true;

      if (connection.run() == "Disconnect") {
        WiFi.disconnect();
        while(WiFi.isConnected()) {}
      }

    });
  } else {
    wifimain.addItem("Join a network", doFunction {

      ezMenu& joinmenu = *new ezMenu("Joining a network");

      joinmenu.addItem("Scan and join", doFunction {
        ezMsgBox scanning("WiFi setup menu", "scanning");
        scanning.focus();
        WiFi.disconnect();
        WiFi._setStatus(WL_IDLE_STATUS);
        delay(100);
        int16_t n = WiFi.scanNetworks();
        scanning.blur();
        log_d("Scan found %d networks", n);
        if (n == 0) {
          msgBox("WiFi setup menu", "No networks found", "OK");
        } else {
          ezMenu& networks = *new ezMenu("Select your netork");
          for (uint16_t i = 0; i < n; ++i) {
            // No duplicates (multiple BSSIDs on same SSID)
            // because we're only picking an SSID here
            bool haveAlready = false;
            for (auto item : networks.items) {
              if (item.text == WiFi.SSID(i)) haveAlready = true;
            }
            if (!haveAlready && networks.items.size() < 15) {
              networks.addItem(WiFi.SSID(i));
            }
          }
          log_d("%d unique networks", networks.items.size());
          networks.addItem("Back", EZ_BACK);
          networks.itemsPerRow    = 1;
          networks.rowsPerScreen  = 4;
          networks.font           = FSS9;
          networks.spriteBPP      = 16;
          networks.colors         = { TFT_WHITE, TFT_BLACK, TFT_BLACK };
          networks.onColors       = networks.colors;

          int16_t selected = networks.runOnce();

          if (selected < networks.items.size() - 1) {

            if (WiFi.encryptionType(selected) == WIFI_AUTH_OPEN) {
              WiFi.mode(WIFI_MODE_STA);
              WiFi.begin(WiFi.SSID(selected).c_str());
            } else {
              String key = keyboardInput("Enter wifi password", "");
              WiFi.mode(WIFI_MODE_STA);
              WiFi.begin(WiFi.SSID(selected).c_str(), key.c_str());
            }
            ezMsgBox connecting("WiFi setup menu", "Connecting ...", "Abort");
            connecting.focus();
            int16_t status;
            while (!connecting.somethingPressed()) {
              status = WiFi.status();
              if (status == WL_CONNECTED) {
                ezBack();
                break;
              }
              if (status != WL_IDLE_STATUS) {
                msgBox("WiFi setup menu", "Connect failed.\n\n(Wrong password?)", "OK");
                ezBack();
                break;
              }
              loop();
            }
          }
          delete &networks;
          WiFi.scanDelete();
        }
      });

      joinmenu.addItem("SmartConfig", doFunction {
        // msgBox("SmartConfig setup", "Waiting for SmartConfig", "Abort");
        WiFi.mode(WIFI_MODE_STA);
        WiFi.beginSmartConfig();
        bool done_already = false;
        while (!WiFi.isConnected()) {
//           if (ez.buttons.poll() == "Abort") {
//             WiFi.stopSmartConfig();
//             break;
//           }
          if (WiFi.smartConfigDone() && !done_already) {
            // ez.msgBox("SmartConfig setup", "SmartConfig received | Connecting ...", "Abort", false);
            done_already = true;
          }
        }
      });

      joinmenu.addItem("WPS", doFunction { EZWIFI._wps(); });

      joinmenu.addItem("Back", EZ_BACK);

      joinmenu.run();


      if (WiFi.isConnected()) {
        bool haveAlready = false;
        for (uint8_t n = 0; n < EZWIFI.networks.size(); n++) {
          if (EZWIFI.networks[n].SSID == WiFi.SSID()) haveAlready = true;
        }
        if (!haveAlready &&
            msgBox("Wifi settings", "Save this network to autoconnects?",
                   "No", "Yes") == "Yes") {
          EZWIFI.add(WiFi.SSID(), WiFi.psk());
          EZWIFI.writeFlash();
        }
      }

      delete &joinmenu;

    });
  }

  if (networks.size()) wifimain.addItem("Manage autoconnects", doFunction {
    ezMenu autoconnect("Managing autoconnects");
    for (uint8_t n = 0; n < EZWIFI.networks.size(); n++) {
      autoconnect.addItem(EZWIFI.networks[n].SSID);
    }
    autoconnect.addItem("Back", EZ_BACK);
    int16_t selected = autoconnect.run();

    if (selected < autoconnect.items.size() - 1) {
      String ssid = EZWIFI.networks[selected].SSID;
      if (msgBox("Forgetting wifi network",
          "Are you sure you want to forget " + ssid + " ?",
          "Yes", "No") == "Yes") {
        EZWIFI.remove(EZWIFI.networks[selected].SSID);
        EZWIFI.writeFlash();
      }
    }
  });

  wifimain.addItem("Back", EZ_BACK);

  int16_t r = wifimain.runOnce();
  delete &wifimain;
  return r;
}

void ezWifiClass::_wps() {
  bool PINmode = (msgBox("WPS type select", "WPS with PIN or button?",
                         "PIN", "button") == "PIN");
  // ez.msgBox("WPS setup", "Waiting for WPS", "Abort", false);
  WiFi.mode(WIFI_MODE_STA);
  static esp_wps_config_t config;
  config.crypto_funcs = &g_wifi_default_wps_crypto_funcs;
  strcpy(config.factory_info.manufacturer, "ESPRESSIF");
  strcpy(config.factory_info.model_number, "ESP32");
  strcpy(config.factory_info.model_name, "ESPRESSIF IOT");
  strcpy(config.factory_info.device_name, "ESP STATION");
  if (PINmode) {
    config.wps_type = WPS_TYPE_PIN;
  } else {
    config.wps_type = WPS_TYPE_PBC;
  }
  WiFi.onEvent([](WiFiEvent_t event, system_event_info_t info) {
    EZWIFI._WPS_event = event;
    EZWIFI._WPS_new_event = true;
    if (event == SYSTEM_EVENT_STA_WPS_ER_PIN) {
      char wps_pin[9];
      for (int8_t i = 0; i < 8; i++) {
        wps_pin[i] = info.sta_er_pin.pin_code[i];
      }
      wps_pin[8] = '\0';
      EZWIFI._WPS_pin = String(wps_pin);
    }
  });
  esp_wifi_wps_enable(&config);
  esp_wifi_wps_start(0);
  EZWIFI._WPS_new_event = false;
  while (!WiFi.isConnected()) {
    //         if (ez.buttons.poll() == "Abort") {
    //           esp_wifi_wps_disable();
    //           break;
    //         }
    if (EZWIFI._WPS_new_event) {
      switch(EZWIFI._WPS_event) {
        case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
          //ez.msgBox("WPS setup", "WPS successful | Connecting ...", "Abort", false);
          esp_wifi_wps_disable();
          delay(10);
          WiFi.begin();
          break;
        case SYSTEM_EVENT_STA_WPS_ER_FAILED:
        case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
          //ez.msgBox("WPS setup", "WPS failed or timed out | Retrying ...", "Abort", false);
          esp_wifi_wps_disable();
          esp_wifi_wps_enable(&config);
          esp_wifi_wps_start(0);
          break;
        case SYSTEM_EVENT_STA_WPS_ER_PIN:
          //ez.msgBox("WPS setup", "WPS PIN: " + _WPS_pin, "Abort", false);
          break;
        default:
          break;
      }
      EZWIFI._WPS_new_event = false;
    }
  }
}


ezWifiClass& ezWifi = ezWifiClass::instance();
