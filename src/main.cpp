#include <LiquidCrystal_I2C.h> 
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Hashtable.h>
#include <MFRC522.h>

#define RELAY_PIN 27 // ESP32 pin GPIO27

#define RFID_SDA_PIN  5  // ESP32 pin GPIO5 
#define RFID_RST_PIN 26 // ESP32 pin GPIO26

#define LCD_SCL_PIN 33 // ESP32 pin GPIO33
#define LCD_SDA_PIN 21 // ESP32 pin GPIO21

MFRC522 rfid(RFID_SDA_PIN, RFID_RST_PIN);
Hashtable<String, uint8_t> AuthorizedRFIDs;
LiquidCrystal_I2C lcd(0x27,20,4);

String readKeyCard_asHex() {
  String RFID = "";
  for (byte i=0; i < rfid.uid.size; i++) {
    RFID = RFID + String(rfid.uid.uidByte[i], HEX); }
  return RFID;
}

void getRegisteredUIDs() {
  Serial.print("\n");
  SimpleVector<String> keys = AuthorizedRFIDs.keys(); // Get a vector of all the keys in the hashtable
    for (const String& key : keys) { // Iterate through the keys, using a range-based for loop... String& is used to avoid copying the key
      Serial.print("Key: ");
      Serial.print(key.c_str()); // c_str() returns a pointer to the underlying char array of the String
      Serial.print(", Value: ");
      Serial.println(*AuthorizedRFIDs.get(key)); // Get the value associated with the key using a pointer
    }
}

bool isAuthorized() {
  return AuthorizedRFIDs.exists(readKeyCard_asHex());
}

void setup () {
  Serial.begin(9600);
  SPI.begin(); // init SPI bus
  rfid.PCD_Init(); // init MFRC522

  Wire.begin(LCD_SDA_PIN, LCD_SCL_PIN);
  lcd.init();
  lcd.clear();         
  lcd.backlight(); 

  lcd.setCursor(1, 0);
  lcd.print("SCAN YOUR KEYCARD!");

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  AuthorizedRFIDs.put("837b9ee4", 0);
}

void loop () {
  if (rfid.PICC_IsNewCardPresent()) { // new tag is available
    if (rfid.PICC_ReadCardSerial()) { // NUID has been readed

      if (!isAuthorized()) { 
        lcd.setCursor(3, 2);
        lcd.print("ACCESS  DENIED");
        delay(500);
        lcd.clear();
        delay(1000);
        lcd.setCursor(1, 0);
        lcd.print("SCAN YOUR KEYCARD!");
        // AuthorizedRFIDs.put(readKeyCard_asHex(), 1); 
      } else {
        lcd.setCursor(3, 2);
        lcd.print("ACCESS GRANTED");
        lcd.setCursor(6, 3);
        lcd.print("WELCOME!");
        delay(1000);
        lcd.clear();
        digitalWrite(RELAY_PIN, HIGH);
        delay(2000);
        digitalWrite(RELAY_PIN, LOW);
        delay(1000);
        lcd.setCursor(1, 0);
        lcd.print("SCAN YOUR KEYCARD!");
      }
      getRegisteredUIDs();

      rfid.PICC_HaltA(); // halt PICC
      rfid.PCD_StopCrypto1(); // stop encryption on PCD
    }
  }
}
