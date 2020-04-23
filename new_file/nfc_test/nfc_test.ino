// 오픈소스소프트웨어 중간 과제
// AI응용전공_21528670_정기윤

// 라이브러리 포함
#include <Servo.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

// PN532 NFC 모듈 핀 설정
#define PN532_SCK  (13)
#define PN532_MOSI (11)
#define PN532_SS   (10)
#define PN532_MISO (12)

// 서보모터, LED 핀 설정
#define servo_pin 9
#define led_pin   2

// NFC SPI 통신 설정
Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

// NFC 카드 UID 저장
uint8_t myuid[] = {71, 87, 133, 123};

// servo 객체 선언
Servo servo;

void setup() {
  // LED 핀 출력 설정
  pinMode(led_pin, OUTPUT);

  // 시리얼 통신 보드레이트 설정
  Serial.begin(115200);

  // PN532 기본 동작
  nfc.begin();  
  nfc.setPassiveActivationRetries(0xFF);  
  nfc.SAMConfig();
  
  Serial.println("Waiting for an ISO14443A card");
}

void loop() {
  // 서보모터 핀 설정 & 0도 설정
  servo.attach(servo_pin);
  servo.write(0);

  // UID 버퍼 반환 버퍼 선언 & UID 길이 선언
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;

  // 카드가 읽혀지면 데이터 저장
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  
  if (success) {
    // 카드 UID 출력
    Serial.println(""); Serial.print("UID : ");
    for (uint8_t i = 0; i < uidLength; i++) {
      Serial.print(uid[i]); Serial.print(" ");
    }
    Serial.println("");

    // 저장한 카드 데이터와 일치할 시 실행
    if((myuid[0] == uid[0]) && (myuid[1] == uid[1]) && (myuid[2] == uid[2]) && (myuid[3] == uid[3])) {

      // LED 점등 & 서보모터 90도 설정
      Serial.println("true");
      digitalWrite(led_pin, HIGH); servo.write(90);

      // 2초 딜레이 설정
      delay(2000);

      // LED 소등 & 서보모터 0도 설정
      digitalWrite(led_pin, LOW); servo.write(0);

      // 서보모터 연결 해제
      servo.detach();
    }

    // 저장한 카드 데이터와 일치하지 않을 시 실행
    else {

      // LED 2회 점멸
      Serial.println("It's not my UID");
      digitalWrite(led_pin, HIGH); delay(300);
      digitalWrite(led_pin, LOW); delay(300);
      digitalWrite(led_pin, HIGH); delay(300);
      digitalWrite(led_pin, LOW); delay(300);
    }
  delay(1000);
  }

  // PN532 카드 대기 시간을 초과 하였을 때 실행
  else {
    Serial.println("Timed out waiting for a card");
  }
}
