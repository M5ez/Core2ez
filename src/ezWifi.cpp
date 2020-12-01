// #include "ezWifi.h"
//
//
// void ezWifiClass::begin() {
//   log_d("EZWIFI: Initialising");
//   WiFi.mode(WIFI_MODE_STA);
//   WiFi.setAutoConnect(false);		// We have our own multi-AP version of this
//   WiFi.setAutoReconnect(false);	// So we turn off the ESP32's versions
//   WiFi.setHostname("M5Stack");
//   readFlash();
//   _state = EZWIFI_IDLE;
//   const uint8_t cutoffs[] = { 0, 20, 40, 70 };
//   ez.settings.menuObj.addItem("Wifi settings", menu);
//   ez.header.insert(RIGHTMOST, "wifi", sizeof(cutoffs) * (ez.theme->signal_bar_width + ez.theme->signal_bar_gap) + 2 * ez.theme->header_hmargin, _drawWidget);
//   // For handling issue #50, when initial connection attempt fails in this specific mode but will succeed if tried again.
//   WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info) {
//     if(info.disconnected.reason == WIFI_REASON_ASSOC_FAIL) {
//       log_d("EZWIFI: Special case: Disconnect w/ ASSOC_FAIL. Setting _state to EZWIFI_SCANNING;");
//       _state = EZWIFI_SCANNING;
//     }
//   }, WiFiEvent_t::SYSTEM_EVENT_STA_DISCONNECTED);
// }
//
// void ezWifiClass::update() {
//   if (millis() <= _wifi_time + EZWIFI_WIFI_INTERVAL) return;
//   _wifi_time = millis();
//   if (millis() > _widget_time + EZWIFI_WIDGET_INTERVAL {
//     ez.header.draw("wifi");
//     _widget_time = millis();
//   }
//   if (WiFi.isConnected() && _state != EZWIFI_AUTOCONNECT_DISABLED && _state != EZWIFI_IDLE) {
//     _state = EZWIFI_IDLE;
//     log_d("EZWIFI: Connected, returning to IDLE state");
//   }
//   if (!autoConnect || WiFi.isConnected() || networks.size() == 0) return;
//   int8_t scanresult;
//   switch(_state) {
//     case EZWIFI_WAITING:
//       log_d("EZWIFI: State Machine: _state = EZWIFI_WAITING");
//       if (millis() < _wait_until) return;
//       // intentional fall-through
//     case EZWIFI_IDLE:
//       log_d("EZWIFI: State Machine: _state = EZWIFI_IDLE");
//       log_d("EZWIFI: Starting scan");
//       WiFi.mode(WIFI_MODE_STA);
//       WiFi.scanNetworks(true);
//       _current_from_scan = 0;
//       _state = EZWIFI_SCANNING;
//       _wait_until = millis() + 10000;
//       break;
//     case EZWIFI_SCANNING:
//       log_d("EZWIFI: State Machine: _state = EZWIFI_SCANNING");
//       scanresult = WiFi.scanComplete();
//       switch(scanresult) {
//         case WIFI_SCAN_RUNNING:
//           break;
//         case WIFI_SCAN_FAILED:
//           log_d("EZWIFI: Scan failed");
//           _state = EZWIFI_WAITING;
//           _wait_until = millis() + 60000;
//           WiFi.scanDelete();
//           return;
//         default:
//           log_d("EZWIFI: Scan got " + (String)scanresult + " networks");
//           for (uint8_t n = _current_from_scan; n < scanresult; n++) {
//             for (uint8_t m = 0; m < networks.size(); m++) {
//               String ssid = networks[m].SSID;
//               String key = networks[m].key;
//               if (ssid == WiFi.SSID(n)) {
//                 log_d("EZWIFI: Match: " + WiFi.SSID(n) + ", connecting...");
//                 WiFi.mode(WIFI_MODE_STA);
//                 WiFi.begin(ssid.c_str(), key.c_str());
//                 _state = EZWIFI_CONNECTING;
//                 _wait_until = millis() + 7000;
//                 return;
//               }
//             }
//           }
//           log_d("EZWIFI: No (further) matches, waiting...");
//           _state = EZWIFI_WAITING;
//           _wait_until = millis() + 60000;
//           WiFi.scanDelete();
//         //
//       }
//     case EZWIFI_CONNECTING:
//       log_d("EZWIFI: State Machine: _state = EZWIFI_CONNECTING");
//       if (millis() > _wait_until) {
//         log_d("EZWIFI: Connect timed out...");
//         WiFi.disconnect();
//         _current_from_scan++;
//         _state = EZWIFI_SCANNING;
//       }
//       break;
//     case EZWIFI_AUTOCONNECT_DISABLED:
//       log_d("EZWIFI: State Machine: _state = EZWIFI_AUTOCONNECT_DISABLED");
//       break;
//     default:
//       log_d("EZWIFI: State Machine: default case! _state = %d", _state);
//       break;
//     //
//   }
// }
//
//
//
// void ezWifiClass::_drawWidget(uint16_t x, uint16_t w) {
//   const uint8_t cutoffs[] = { 0, 20, 40, 70 };
//   uint8_t max_bars = sizeof(cutoffs);
//   uint8_t bars;
//   uint16_t left_offset = x + ez.theme->header_hmargin;
//   bars = 0;
//   if (WiFi.isConnected()) {
//     uint8_t signal = map(100 + WiFi.RSSI(), 5, 90, 0, 100);
//     for (uint8_t n = 0; n < sizeof(cutoffs); n++) {				// Determine how many bars signal is.
//       if (signal >= cutoffs[n]) bars = n + 1;
//     }
//   }
//   uint8_t top = ez.theme->header_height / 10;
//   uint8_t max_len = ez.theme->header_height * 0.8;
//   uint8_t this_len;
//   for (uint8_t n = 0; n < max_bars; n++) {
//     this_len = ((float) (n + 1) / max_bars) * max_len;
//     m5.lcd.fillRect(left_offset + n * (ez.theme->signal_bar_width + ez.theme->signal_bar_gap), top + max_len - this_len, ez.theme->signal_bar_width, this_len, (n + 1 <= bars ? ez.theme->header_fgcolor : ez.theme->header_bgcolor) );
//   }
// }
//
// void ezWifiClass::add(String ssid, String key){
//   WifiNetwork_t new_net;
//   new_net.SSID = ssid;
//   new_net.key = key;
//   networks.push_back(new_net);
// }
//
// bool ezWifiClass::remove(int8_t index) {
//   if (index < 0 || index >= networks.size()) return false;
//   networks.erase(networks.begin() + index);
//   return true;
// }
//
// bool ezWifiClass::remove(String ssid) { return remove(indexForSSID(ssid)); }
//
// int8_t ezWifiClass::indexForSSID(String ssid) {
//   for (uint8_t n = 0; n < networks.size(); n++) {
//     if (networks[n].SSID == ssid) return n;
//   }
//   return -1;
// }
//
// void ezWifiClass::readFlash() {
//   Preferences prefs;
//   networks.clear();
//   prefs.begin("M5ez", true);	// true: read-only
//   autoConnect = prefs.getBool("autoconnect_on", true);
//   log_d("EZWIFI readFlash: Autoconnect is %s", autoConnect ? "ON" : "OFF");
//   WifiNetwork_t new_net;
//   String idx;
//   uint8_t index = 1;
//   while (true) {
//     idx = "SSID" + (String)index;
//     String ssid = prefs.getString(idx.c_str(), "");
//     idx = "key" + (String)index;
//     String key = prefs.getString(idx.c_str(), "");
//     if (ssid != "") {
//       new_net.SSID = ssid;
//       new_net.key = key;
//       networks.push_back(new_net);
//       log_d("EZWIFI readFlash: Read ssid: %s, key: %s", ssid, key);
//       index++;
//     } else {
//       break;
//     }
//   }
//   prefs.end();
// }
//
// void ezWifiClass::writeFlash() {
//   Preferences prefs;
//   String idx;
//   uint8_t n = 1;
//   prefs.begin("M5ez", false);
//   // Remove unknown number of items from NVS, sequentially named SSID1 to SSIDN, and key1 to keyN where N
//   // is the total number of WiFi Networks stored (which may be different than networks.size() at this point.)
//   while (true) {
//     idx = "SSID" + (String)n;
//     if(!prefs.remove(idx.c_str())) break;
//     idx = "key" + (String)n;
//     prefs.remove(idx.c_str());
//     n++;
//   }
//   prefs.putBool("autoconnect_on", autoConnect);
//   log_d("EZWIFI writeFlash: Autoconnect is %s", autoConnect ? "ON" : "OFF");
//   for (n = 0; n < networks.size(); n++) {
//     idx = "SSID" + (String)(n + 1);
//     prefs.putString(idx.c_str(), networks[n].SSID);
//     if (networks[n].key != "") {
//       idx = "key" + (String)(n + 1);
//       prefs.putString(idx.c_str(), networks[n].key);
//       log_d("EZWIFI writeFlash: Wrote ssid: %s, key: %s", networks[n].SSID, networks[n].key);
//     }
//   }
//   prefs.end();
// }
//
// void ezWifiClass::menu() {
//   _state = EZWIFI_AUTOCONNECT_DISABLED;
//   log_d("EZWIFI: Disabling autoconnect while in Wifi menu.");
//   ezMenu wifimain ("Wifi settings");
//   wifimain.txtSmall();
//   wifimain.addItem("onoff | Autoconnect\t" + (String)(autoConnect ? "ON" : "OFF"), NULL, _onOff);
//   wifimain.addItem("connection | " + (String)(WiFi.isConnected() ? "Connected: " + WiFi.SSID() : "Join a network"), NULL, _connection);
//   wifimain.addItem("Manage autoconnects", _manageAutoconnects);
//   wifimain.buttons("up#Back#select##down#");
//   wifimain.run();
//   _state = EZWIFI_IDLE;
//   log_d("EZWIFI: Enabling autoconnect exiting Wifi menu.");
// }
//
// bool ezWifiClass::_onOff(ezMenu* callingMenu) {
//   autoConnect = !autoConnect;
//   callingMenu->setCaption("onoff", "Autoconnect\t" + (String)(autoConnect ? "ON" : "OFF"));
//   writeFlash();
//   return true;
// }
//
// void ezWifiClass::_manageAutoconnects() {
//   ezMenu autoconnect("Managing autoconnects");
//   if (!networks.size()) {
//     ez.msgBox("No autoconnects", "You have no saved autoconnect networks.", "OK");
//     return;
//   }
//   for (uint8_t n = 0; n < networks.size(); n++) {
//     autoconnect.addItem(networks[n].SSID, NULL, _autoconnectSelected);
//   }
//   autoconnect.txtSmall();
//   autoconnect.buttons("up#Back#Forget##down#");
//   autoconnect.run();
// }
//
// bool ezWifiClass::_autoconnectSelected(ezMenu* callingMenu) {
//   if (callingMenu->pickButton() == "Forget") {
//     if (ez.msgBox("Forgetting wifi network", "Are you sure you want | to forget wifi network | " + callingMenu->pickName() + " ?", "Yes##No") == "Yes") {
//       remove(callingMenu->pick() - 1);
//       callingMenu->deleteItem(callingMenu->pick());
//       writeFlash();
//     }
//   }
//   return false;
// }
//
// bool ezWifiClass::_connection(ezMenu* callingMenu) {
//   if (WiFi.isConnected()) {
//     const uint8_t tab = 140;
//     ez.screen.clear();
//     ez.header.show("Current wifi connection");
//     ez.canvas.font(&FreeSans9pt7b);
//     ez.canvas.lmargin(10);
//     ez.canvas.y(ez.canvas.top() + 5);
//     ez.canvas.print("SSID:"); ez.canvas.x(tab); ez.canvas.println(WiFi.SSID());
//     ez.canvas.print("Key:"); ez.canvas.x(tab); ez.canvas.println(WiFi.psk());
//     ez.canvas.print("My IP:"); ez.canvas.x(tab); ez.canvas.println(WiFi.localIP().toString());
//     ez.canvas.print("My MAC:"); ez.canvas.x(tab); ez.canvas.println(WiFi.macAddress());
//     ez.canvas.print("My hostname:"); ez.canvas.x(tab); ez.canvas.println(WiFi.getHostname());
//     ez.canvas.print("Router IP:"); ez.canvas.x(tab); ez.canvas.println(WiFi.gatewayIP().toString());
//     ez.canvas.print("Router BSSID:"); ez.canvas.x(tab); ez.canvas.println(WiFi.BSSIDstr());
//     ez.canvas.print("DNS IP:"); ez.canvas.x(tab); ez.canvas.println(WiFi.dnsIP(0).toString());
//     if (WiFi.dnsIP(1)) { ez.canvas.x(tab); ez.canvas.println(WiFi.dnsIP(1).toString()); }
//     String pressed = ez.buttons.wait("Back#Disconnect#");
//     if (pressed == "Back") return true;
//     if (pressed == "Disconnect") {
//       WiFi.disconnect();
//       while(WiFi.isConnected()) {}
//     }
//
//   } else {
//
//     String SSID = "", key = "";
//     ezMenu joinmenu("Joining a network");
//     joinmenu.txtSmall();
//     joinmenu.addItem("Scan and join");
//     joinmenu.addItem("SmartConfig");
//     #ifdef M5EZ_WPS
//       joinmenu.addItem("WPS Button");
//       joinmenu.addItem("WPS Pin Code");
//     #endif
//     joinmenu.buttons("up#Back#select##down#");
//     joinmenu.runOnce();
//
//     if (joinmenu.pickName() == "Scan and join") {
//       ez.msgBox("WiFi setup menu", "Scanning ...", "");
//       WiFi.disconnect();
//       WiFi._setStatus(WL_IDLE_STATUS);
//       delay(100);
//       int16_t n = WiFi.scanNetworks();
//       if (n == 0) {
//         ez.msgBox("WiFi setup menu", "No networks found", "OK");
//       } else {
//         ezMenu networks("Select your netork");
//         networks.txtSmall();
//         for (uint16_t i = 0; i < n; ++i) {
//           // No duplicates (multiple BSSIDs on same SSID)
//           // because we're only picking an SSID here
//           if (!networks.getItemNum(WiFi.SSID(i))) {
//             networks.addItem(WiFi.SSID(i));
//           }
//         }
//         networks.buttons("up#Back#select##down#");
//         if (networks.runOnce()) {
//           SSID = networks.pickName();
//           if ( WiFi.encryptionType(networks.pick() - 1) == WIFI_AUTH_OPEN) {
//             WiFi.mode(WIFI_MODE_STA);
//             WiFi.begin(SSID.c_str());
//           } else {
//             key = ez.textInput("Enter wifi password");
//             WiFi.mode(WIFI_MODE_STA);
//             WiFi.begin(SSID.c_str(), key.c_str());
//           }
//           ez.msgBox("WiFi setup menu", "Connecting ...", "Abort", false);
//           String button;
//           int16_t status;
//           while (button = ez.buttons.poll()) {
//             if (button == "Abort") {
//               WiFi.disconnect();
//               break;
//             }
//             status = WiFi.status();
//             if (status == WL_CONNECTED) {
//               break;
//             }
//             if (status == WL_CONNECT_FAILED || status == WL_NO_SSID_AVAIL) {
//               ez.msgBox("WiFi setup menu", "Connect failed. | | (Wrong password?)", "OK");
//               break;
//             }
//           }
//         }
//         WiFi.scanDelete();
//       }
//     }
//
//     if (joinmenu.pickName() == "SmartConfig") {
//       ez.msgBox("SmartConfig setup", "Waiting for SmartConfig", "Abort", false);
//       WiFi.mode(WIFI_MODE_STA);
//       WiFi.beginSmartConfig();
//       bool done_already = false;
//       while (!WiFi.isConnected()) {
//         if (ez.buttons.poll() == "Abort") {
//           WiFi.stopSmartConfig();
//           break;
//         }
//         if (WiFi.smartConfigDone() && !done_already) {
//           ez.msgBox("SmartConfig setup", "SmartConfig received | Connecting ...", "Abort", false);
//           done_already = true;
//         }
//       }
//     }
//
//     #ifdef M5EZ_WPS
//       if (joinmenu.pickName().substring(0,3) == "WPS") {
//         ez.msgBox("WPS setup", "Waiting for WPS", "Abort", false);
//         WiFi.mode(WIFI_MODE_STA);
//         static esp_wps_config_t config;
//         config.crypto_funcs = &g_wifi_default_wps_crypto_funcs;
//         strcpy(config.factory_info.manufacturer, "ESPRESSIF");
//         strcpy(config.factory_info.model_number, "ESP32");
//         strcpy(config.factory_info.model_name, "ESPRESSIF IOT");
//         strcpy(config.factory_info.device_name, "ESP STATION");
//         if (joinmenu.pickName() == "WPS Button") {
//           config.wps_type = WPS_TYPE_PBC;
//         } else {
//           config.wps_type = WPS_TYPE_PIN;
//         }
//         WiFi.onEvent(_WPShelper);
//         esp_wifi_wps_enable(&config);
//         esp_wifi_wps_start(0);
//
//         _WPS_new_event = false;
//         while (!WiFi.isConnected()) {
//           if (ez.buttons.poll() == "Abort") {
//             esp_wifi_wps_disable();
//             break;
//           }
//           if (_WPS_new_event) {
//             switch(_WPS_event) {
//               case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
//                 ez.msgBox("WPS setup", "WPS successful | Connecting ...", "Abort", false);
//                 esp_wifi_wps_disable();
//                 delay(10);
//                 WiFi.begin();
//                 break;
//               case SYSTEM_EVENT_STA_WPS_ER_FAILED:
//               case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
//                 ez.msgBox("WPS setup", "WPS failed or timed out | Retrying ...", "Abort", false);
//                 esp_wifi_wps_disable();
//                 esp_wifi_wps_enable(&config);
//                 esp_wifi_wps_start(0);
//                 break;
//               case SYSTEM_EVENT_STA_WPS_ER_PIN:
//                 ez.msgBox("WPS setup", "WPS PIN: " + _WPS_pin, "Abort", false);
//                 break;
//               default:
//                 break;
//             }
//             _WPS_new_event = false;
//           }
//         }
//       }
//     #endif
//
//     if (WiFi.isConnected()) _askAdd();
//   }
//   callingMenu->setCaption("connection", (String)(WiFi.isConnected() ? "Connected: " + WiFi.SSID() : "Join a network"));
//   return true;
// }
//
// #ifdef M5EZ_WPS
//   void ezWifiClass::_WPShelper(WiFiEvent_t event, system_event_info_t info) {
//     _WPS_event = event;
//     _WPS_new_event = true;
//     if (event == SYSTEM_EVENT_STA_WPS_ER_PIN) {
//       char wps_pin[9];
//       for (int8_t i = 0; i < 8; i++) {
//         wps_pin[i] = info.sta_er_pin.pin_code[i];
//       }
//       wps_pin[8] = '\0';
//       _WPS_pin = String(wps_pin);
//     }
//   }
// #endif
//
// void ezWifiClass::_askAdd() {
//   for (uint8_t n = 0; n < networks.size(); n++) {
//     if (networks[n].SSID == WiFi.SSID()) return;
//   }
//   if (ez.msgBox("Wifi settings", "Save this network | to your autoconnects?", "no##yes") == "yes") {
//     add(WiFi.SSID(), WiFi.psk());
//     writeFlash();
//   }
// }
//
// ezWifiClass& ezWifi = ezWifiClass::instance();
