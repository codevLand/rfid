#include "Peripherals.h"
#include "Auth.h"

#include <SD.h>
#include <Wire.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>


#define RELAY_PIN 27            // ESP32 pin GPIO27
#define RFID_SDA_PIN 5          // ESP32 pin GPIO5
#define RFID_RST_PIN 26         // ESP32 pin GPIO26
#define LCD_SCL_PIN 33          // ESP32 pin GPIO33
#define LCD_SDA_PIN 21          // ESP32 pin GPIO21
// #define PROXIMITY_PIN "xx"      // ESP32 pin GPIOxx
// #define SD_PIN "xx"             // ESP32 pin GPIOxx

Periperals::Periperals() {
  MFRC522 rfid(RFID_SDA_PIN, RFID_RST_PIN);
  LiquidCrystal_I2C lcd(0x27,20,4);
  // RTC_DS3231 rtc;
  // FILE logFile;
}
// ~Periperals();

void start() {
  lcdSetup();
  relaySetup();
  rfid.PCD_Init();
}

void readRFID() {
  (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial())
    ? verifyGateAccess(); : return;
  rfid.PICC_HaltA();        // halt PICC
  rfid.PCD_StopCrypto1();   // stop encryption on PCD
}

void verifyGateAccess() {
  Auth isGatePassAuth() ? allowGatePassage() : denyGatePassage();
}

void allowGatePassage() {
  lcd.setCursor(3, 2);
  lcd.print("ACCESS GRANTED");
  lcd.setCursor(6, 3);
  lcd.print("WELCOME!");
  digitalWrite(RELAY_PIN, HIGH);
  delay(2500);
  lcd.clear();
  digitalWrite(RELAY_PIN, LOW);
  initLCD();
}

void denyGatePassage() {
  lcd.setCursor(3, 2);
  lcd.print("ACCESS  DENIED");
  delay(2500);
  initLCD();
}

void relaySetup() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
}

void lcdSetup() {
  Wire.begin(LCD_SDA_PIN, LCD_SCL_PIN);
  lcd.init();
  lcd.clear();
  lcd.backlight();

  lcd.setCursor(1, 0);
  lcd.print("SCAN YOU KEYCARD!");
}