# **ESP32 WITH RFID AND ELECTRONIC SOLENOID LOCK**

### Components: 
1. [ESP32](https://www.makerlab-electronics.com/products/30-pins-and-38-pins-esp32-wifi-iot-development-board?_pos=17&_sid=fc24fba61&_ss=r)
2. [RFID](https://www.makerlab-electronics.com/products/rc522-mfrc-52213-56mhz-13-56-mhz-rfid-key-card-reader-module-set-for-arduino?variant=428268679989110)
3. [ELECTRONIC SOLENOID LOCK](https://www.makerlab-electronics.com/products/electric-solenoid-lock?_pos=3&_psq=solenoid&_ss=e&_v=1.0)
4. [BATTERY (at least 9v)](https://www.makerlab-electronics.com/products/maxell-aaa-batteries-triple-a-battery-long-life-heavy-duty?_pos=3&_sid=5a8d6c077&_ss=r)

### Software Tools:
1. VSCode 
2. Arduino IDE
3. Libraries for ESP32

### Task List:
- [x] Connect the components to the ESP32 and make them communicate to each
other
- [x] Connect the ESP32 to the WiFI
- [ ] (Optional) Port Forward the ESP32 from the router for web access/mobile
app access
- [ ] Set up RFID cards - 2 cards with the same UID per room/tenant (1 main
key, 1 backup)
- [x] Make an API for database
    * [ ] Make a registration API for RFID Cards
- [ ] Make a User Database access of the gate
- [ ] Filter out the Admin and User Account


#### For future plans:
- [ ] Communicate 2 or more ESP32 for more concrete microcontrols
- [ ] Make a mobile application for Admin
- [ ] Enhance the Database design and its api for more stability

## PROGRESS SO FAR...
1. Initialized the buzzer for alarm whenever the access to the gate has been
   denied.
   ```c++
   #include <Arduino.h>
   #include <SPI.h>
   #include <MFRC522.h>

   #define BUZZER_PIN 13 //ESP32 pin GPIO13

   void setup () {
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW); 
   }
   ```

2. Initialized the RFID for giving access to the user/tenants of the boarding
   house.
   TODO: Need to set up the Database for the Tenants only access
   ```c++
   #include <Arduino.h>
   #include <SPI.h>
   #include <MFRC522.h>

   #define SS_PIN  5  // ESP32 pin GPIO5
   #define RST_PIN 27 // ESP32 pin GPIO27

   MFRC522 rfid(SS_PIN, RST_PIN);

   void setup () {
    Serial.begin(9600);
    SPI.begin(); // init SPI bus
    rfid.PCD_Init(); // init MFRC522
   }
   ```

3. Initialized the LCD Display for User Interface
    ```C++
    #include <Wire.h>
    #include <LiquidCrystal_I2C.h> 
    #include <Arduino.h>
    #include <SPI.h>

    #define LCD_SCL_PIN 33 // ESP32 pin GPIO33
    #define LCD_SDA_PIN 21 // ESP32 pin GPIO21

    LiquidCrystal_I2C lcd(0x27,20,4);

    void setup () {
      Wire.begin(LCD_SDA_PIN, LCD_SCL_PIN);
      lcd.init();
      lcd.clear();         
      lcd.backlight();

      lcd.setCursor(0, 0);
      lcd.print("SCAN RFID");
    }
    ```

4. Initialized Relay Module for using the Elecronic Solenoid LOCK
    ```C++
    #include <Arduino.h>
    #include <SPI.h>
    #include <MFRC522.h>
    
    #define RELAY_PIN 27 // ESP32 pin GPIO27
    
    void setup () {
        pinMode(RELAY_PIN, OUTPUT);
        digitalWrite(RELAY_PIN, LOW);
    }
    ```

5. Initialized the connection WiFi
    ```C++
    #include <WiFi.h>

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

    void setup () {
        initWiFi();
    }
    ```

6. Initialized the API for Web Server
    ```c++
    #include <ESPAsyncWebServer.h> 
    const char* ssid = "Moorse";
    const char* password = "WIFI_PASSWORD";

    AsyncWebServer server(80);

    void setup () {
      Serial.begin(9600);

      server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
        String html = "<html><body><h1>Gate Control<h1>";
        html += "<button onclick=\"location.href='/unlock'\">Unlock Gate</button>";
        html += "<button onclick=\"location.href='/register'\">Register Card</button>";
        html += "</body></html>";

        request->send(200, "text/html", html);
      });
      server.on("/unlock", HTTP_GET, [](AsyncWebServerRequest* request) {
        unlockGate();
        request->send(200, "text/html", "Gate Unlocked");
      });
      server.on("/register", HTTP_GET, [](AsyncWebServerRequest* request) {
        if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
          if (!isAuthorized()) {
            AuthorizedRFIDs.put(readKeyCard_asHex(), 3);
          }

          rfid.PICC_HaltA(); // halt PICC
          rfid.PCD_StopCrypto1(); // stop encryption on PCD  
        }
        request->send(200, "text/html", "Register Card");
      });
      server.begin();
    }
    ```
