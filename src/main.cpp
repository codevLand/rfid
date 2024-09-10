#include <SPI.h>
#include <MFRC522.h>
#include <Hashtable.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define SS_PIN    5   // ESP32 pin GPIO5 
#define RST_PIN   27  // ESP32 pin GPIO27 
#define LED_GR    9   // ESP32 pin GPIO9 
#define BUZZ_PIN  16 // ESP32 pin GPIO16 

MFRC522 rfid(SS_PIN, RST_PIN);
Hashtable<String, u8_t> AuthorizedRFIDs;
const char* ssid = "Moorse";
const char* password = "WIFI_PASSWORD";
const char* hostname = "esp32-rfid-access_control";
const char* apiServer = "https://fake-json-api.mock.beeceptor.com/users";

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.setHostname(hostname);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  
  Serial.print("\nESP32 IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("ESP32 HostName: ");
  Serial.println(WiFi.getHostname());
  Serial.print("RRSI: ");
  Serial.println(WiFi.RSSI());
}

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
      // Serial.println(uthorizedRFIDs.getElement(key)); // Get the value associated with the key
      Serial.println(*AuthorizedRFIDs.get(key)); // Get the value associated with the key using a pointer
    }
}

bool isAuthorized() {
  return AuthorizedRFIDs.exists(readKeyCard_asHex());
}

void setup() {
  Serial.begin(9600);
  SPI.begin(); // init SPI bus
  rfid.PCD_Init(); // init MFRC522
  initWiFi();

  // Serial.println("Tap an RFID/NFC tag on the RFID-RC522 reader");
  pinMode(BUZZ_PIN, OUTPUT);
  pinMode(LED_GR, OUTPUT);

  Serial.println(apiServer);
  AuthorizedRFIDs.put("837b9ee4", 0);
}

void loop() {

  digitalWrite(BUZZ_PIN, LOW);
  digitalWrite(LED_GR, LOW);

  if (rfid.PICC_IsNewCardPresent()) { // new tag is available
    if (rfid.PICC_ReadCardSerial()) { // NUID has been readed
      // MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
      // Serial.print("RFID/NFC Tag Type: ");
      // Serial.println(rfid.PICC_GetTypeName(piccType));

      // print UID in Serial Monitor in the hex format
      if (!isAuthorized()) { AuthorizedRFIDs.put(readKeyCard_asHex(), 1); }
      getRegisteredUIDs();
      // Serial.printf( "heap internal: %d K free / %d K total\n heap spiram: %d K free / %d K total", heap_caps_get_free_size(MALLOC_CAP_INTERNAL)/1024, heap_caps_get_total_size(MALLOC_CAP_INTERNAL)/1024, heap_caps_get_free_size(MALLOC_CAP_SPIRAM)/1024, heap_caps_get_total_size(MALLOC_CAP_SPIRAM)/1024);

      rfid.PICC_HaltA(); // halt PICC
      rfid.PCD_StopCrypto1(); // stop encryption on PCD
    }
  }
}