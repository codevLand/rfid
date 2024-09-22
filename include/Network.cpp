#include <WiFi.h>
#include "Network.h"

#define STATIC_IP INADDR_NONE,INADDR_NONE,INADDR_NONE,INADDR_NONE
#define AP_CH rand() % 13
#define AP_HIDDEN true
#define AP_CONNX  2


Network::Network() {
  WiFi.mode(WIFI_NETW);
  WiFi.config(STATIC_IP);
  WiFi.setHostname(DEVICE_NAME);
  WiFi.begin(WIFI_SSID, WIFI_PWD);
}

void wifiConnect() {
  Serial.print("Connecting to WiFi ...");
  while (WiFi.status() != WL_CONNECTED) { Serial.print('.'); delay(2000); }
  Serial.println("Wifi connection is established.")
}

void apConnect() {
  Serial.print("Creating AP Host ...");
  WiFi.softAP(DEVICE_NAME, AP_PWD, AP_CH, AP_HIDDEN, AP_CONNX);
  Serial.println("Connection Gateway: " + WiFi.softAPIP());
}

void connectedDevices() {
  wifi_sta_list_t wifi_sta_list;
  tcpip_adapter_sta_list_t adapter_sta_list;
  esp_wifi_ap_get_sta_list(&wifi_sta_list);
  tcpip_adapter_get_sta_list(&wifi_sta_list, &adapter_sta_list);

  if (adapter_sta_list.num > 0) Serial.println("-----------");
  for (uint8_t i=0; i<adapter_sta_list.num; i++) {
    tcpip_adapter_sta_info_t station = adapter_sta_list.sta[i];
    Serial.print((String)"[+] Device " + i + " | MAC : ");
    Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X", station.mac[0], station.mac[1], station.mac[2], station.mac[3], station.mac[4], station.mac[5]);
    Serial.println((String) " | IP " + ip4addr_ntoa(&(station.ip)));
  }
}

bool isWifiConnected() { WiFi.status() != WL_CONNECTED ? false : true }