#include <Arduino.h>
#include <SPI.h>
#include <ESPAsyncWebServer.h>
#include <RTClib.h>

#include "Api.h"
#include "Network.h"
#include "Peripherals.h"

void setup () {
  Serial.begin(DEVICE_BAUD);
  SPI.begin();
  Periperals start();
  Network wifiConnect();
  Network apConnect();
  Api routes();
}

void loop () {
  Periperals readRFID();
  Network connectedDevices();
}