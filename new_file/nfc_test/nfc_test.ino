#include <Servo.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

#define PN532_SCK  (13)
#define PN532_MOSI (11)
#define PN532_SS   (10)
#define PN532_MISO (12)

#define servo_pin 9
#define led_pin 2

Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

uint8_t myuid[] = {71, 87, 133, 123};

Servo servo;

void setup() {
  pinMode(led_pin, OUTPUT);
  
  Serial.begin(115200);

  nfc.begin();  
  nfc.setPassiveActivationRetries(0xFF);  
  nfc.SAMConfig();
  
  Serial.println("Waiting for an ISO14443A card");
}

void loop() {
  servo.attach(servo_pin);
  servo.write(0);
  
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  
  if (success) {
    if((myuid[0] == uid[0]) && (myuid[1] == uid[1]) && (myuid[2] == uid[2]) && (myuid[3] == uid[3])) {
      Serial.println("true");
      servo.write(90);
      digitalWrite(led_pin, HIGH); delay(2000); digitalWrite(led_pin, LOW);
      servo.write(0);
      servo.detach();
    } else {
      Serial.println("It's not my UID");
      digitalWrite(led_pin, HIGH); delay(300);
      digitalWrite(led_pin, LOW); delay(300);
      digitalWrite(led_pin, HIGH); delay(300);
      digitalWrite(led_pin, LOW); delay(300);
    }
  delay(1000);
  }
  else {
    Serial.println("Timed out waiting for a card");
  }
}
