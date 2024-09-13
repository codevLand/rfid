#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Hashtable.h>
#include <LiquidCrystal_12C.h>

#include <WebServer.h> // GET THE WEBSERVER LIBRARY
#include <RTClib.h> // GET THE RTC (Real-Time Clock) LIBRARY
#include <SD.h> // GET THE SD CARD LIBRARY

#define RELAY_PIN 27 // ESP32 pin GPIO27

#define RFID_SDA_PIN 5 // ESP32 pin GPIO5
#define RFID_RST_PIN 26 // ESP32 pin GPIO26

#define LCD_SCL_PIN 33 // ESP32 pin GPIO33
#define LCD_SDA_PIN 21 // ESP32 pin GPIO21

// #define PROXIMITY_PIN "wherever this pin goes" // ESP32 pin GPIO`
// #define SD_PIN "wherever this pin goes" // ESP32 pin GPIO

const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASSWORD";
const char* hostname = "esp32-rfid-access-control";

// INITIATE WEB SEVER
WebServer server(80); // 192.168.1.25:80 OR localhost:80

// INITIATE RFID
MFRC522 rfid(RFID_SDA_PIN, RFID_RST_PIN);

// INITIATE HASHTABLE
Hashtable<String, uint8_t> AuthorizedRFIDs;

// INITIATE RTC
// RTC_DS3231 rtc; 

// INITIATE FILE LOGGING
FILE logFile;

// INITIATE PROXIMITY STATE
bool gateOpen = false;
unsigned long gateOpenTime = 0;



// INITIATE WIFI
void initWiFi () {
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }

  Serial.println("Connected to WiFi");
  Serial.print("\nESP32 IP Address: ");
  Serial.println(WiFi.localIP());
}

void initRelay () {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
}

void initProximity () {
  pinMode(PROXIMITY_PIN, INPUT);
}

void initLCD () {
  Wire.begin(LCD_SDA_PIN,LCD_SCL_PIN);
  lcd.init();
  lcd.clear();
  lcd.backlight();

  lcd.setCursor(1, 0);
  lcd.print("SCAN YOU KEYCARD!");
}

void initRTC () {
  if (!rct.begin()) {
    Serial.println("Couldn't find RTC");
    while(1);
  }
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__Date__), F(__Time__)));
  }
}

void readKeyCard_asHex () {
  String RFID = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    RFID = RFID + String(rfid.uid.uidByte[i]), HEX);
  }

  return RFID;
}

void getRegisteredUIDs () {
  Serial.print("\n");
  SimpleVector<String> keys = AuthorizedRFIDs.keys(); // Get the all the keys
  // in the Hashtable

  for (const String& key: keys) {
    Serial.print("Key: ");
    Serial.print(key.c_str());

    Serial.print(", Value: ");
    Serial.println(*AuthorizedRFIDs.get(key));
  }
}

// CHECK IF RFID IS AUTHORIZED
void isAuthorized () {
  return AuthorizedRFIDs.exists(readKeyCard_asHex());
}

// UNLOCK THE GATE IF RFID IS AUTHORIZED
void unlockGate() {
  lcd.setCursor(3, 2);
  lcd.print("ACCESS GRANTED");
  lcd.setCursor(6, 3);
  lcd.print("WELCOME!");
  digitalWrite(RELAY_PIN, HIGH);
  delay(2000);
  lcd.clear();
  digitalWrite(RELAY_PIN, LOW);
  
  initLCD();
}

// DO NOT UNLOCK THE GATE IF RFID IS UNAUTHORIZED
void denyAccess () {
  lcd.setCursor(3, 2);
  lcd.print("ACCESS  DENIED");
  delay(1000);

  initLCD();
}

// CHECK IF THE RFID EXIST
void checkRFID () {
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    if (!isAuthorized()) {
      denyAccess();
    } else {
      unlockGate();
    }
  } else {
    return;
  }
}

void handleRoot () {
  String html = "<html><body><h1>Gate Control<h1>";
  html += "<button onclick=\"location.href='/unlock'\">Unlock Gate</button>";
  html += "<button onclick=\"location.href='/register'\">Register Card</button>";
  html += "</body></html>";

  server.send(200, "text/html", html)
}

void handleUnlock () {
  unlockGate();
  server.send(200, "text/html", "Gate Unlocked");
}

void handleRegister () {
  if (rfid.PICC_IsNewCardPresent && rfid.PICC_ReadCardSerial) {
    if (!isAuthorized()) {
      AuthorizedRFIDs.put(readKeyCard_asHex, "Guest");
    }
  }
  server.send(200, "text/html", "Register Card")
}

// SETUP WEBSER
server.on("/", handleRoot);
server.on("/unlock", handleUnlock);
server.on("/register", handleRegister);
server.begin();

void logEvent (String event) {
  DateTime now = rtc.now();
  String log = String(now.timestamp()) + ": " + event;
  Serial.println(log);

  logFile = SD.open("/log.txt", FILE_APPEND);
  if (logFile) {
    logFile.println(log);
    logFile.close();
  } else {
    Serial.print("Error opening log file");
  }
}

void checkProximity () {
  if (digitalRead(PROXIMITY_PIN) == HIGH && !gateOpen) {
    gateOpen = true;
    gateOpenTime = millis();
    logEvent("Gate Open");
  } else if (digitalRead(PROXIMITY_PIN) == LOW && gateOpen) {
    gateOpen = false;
    unsigned long openDuration = millis() - gateOpenTime;
    logEvent("Gate Closed, Open Duration: " + String(openDuration / 1000 ) + "s");
  }
}

void setup () {
  Serial.begin(115200);
  SPI.begin();
  rfid.PCD_Init();

  initWiFi();
  initLCD();
  initRelay();
  initRTC();
  initProximity();

  AuthorizedRFIDs.put("837b9ee4", "Administrator");
}

void loop () {
  checkRFID();
}
