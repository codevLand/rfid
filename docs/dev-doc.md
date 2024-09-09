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
- [ ] Connect the components to the ESP32 and make them communicate to each
other
- [ ] Connect the ESP32 to the WiFI
- [ ] (Optional) Port Forward the ESP32 from the router for web access/mobile
app access
- [ ] Make an API for database
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
