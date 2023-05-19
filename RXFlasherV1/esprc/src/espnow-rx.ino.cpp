# 1 "C:\\Users\\wolfm\\AppData\\Local\\Temp\\tmpa403ay0_"
#include <Arduino.h>
# 1 "C:/Users/wolfm/Desktop/FlasherV1/esprc/src/espnow-rx.ino"
#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>


uint8_t wifiChannel = 1;


typedef struct {
  uint16_t channel1;
  uint16_t channel2;
  uint16_t channel3;
  uint16_t channel4;
  uint16_t channel5;
  uint16_t channel6;
  uint16_t channel7;
  uint16_t channel8;
} __attribute__((packed)) EspNowMessage;


uint16_t channel1 = 0;
uint16_t channel2 = 0;
uint16_t channel3 = 0;
uint16_t channel4 = 0;
uint16_t channel5 = 0;
uint16_t channel6 = 0;
uint16_t channel7 = 0;
uint16_t channel8 = 0;


unsigned long lastUpdateTime = 0;

int i =1000;
byte servoPinZwei = 24;
Servo servoZwei;

byte servoPin = 22;
Servo escservo;

byte servoPinEins = 24;
Servo servoEins;
void setup();
void loop();
void onEspNowReceive(const uint8_t *macAddr, const uint8_t *data, int dataLen);
void failsafe();
#line 43 "C:/Users/wolfm/Desktop/FlasherV1/esprc/src/espnow-rx.ino"
void setup() {

  Serial.begin(115200);


  servoEins.attach(servoPinEins);
  escservo.attach(servoPin);
  servoZwei.attach(servoPinZwei);
  delay(1000);

  escservo.writeMicroseconds(1000);
  delay(5000);


  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }


  esp_now_register_recv_cb(onEspNowReceive);


}

void loop() {

  escservo.writeMicroseconds(channel3);
  failsafe();
  if (channel2 > 1690 || channel2 < 1310) {
    lastUpdateTime = millis();

    servoEins.writeMicroseconds(channel2);
    servoZwei.writeMicroseconds(2000 - channel2 + 1000);
  } else {

    servoEins.writeMicroseconds(channel1);
    servoZwei.writeMicroseconds(channel1);
  }
}



void onEspNowReceive(const uint8_t *macAddr, const uint8_t *data, int dataLen) {

  if (dataLen != sizeof(EspNowMessage)) {
    Serial.println("Invalid ESP-NOW message length");
    return;
  }


  EspNowMessage *msg = (EspNowMessage *)data;
# 117 "C:/Users/wolfm/Desktop/FlasherV1/esprc/src/espnow-rx.ino"
  channel1 = msg->channel1;
  channel2 = msg->channel2;
  channel3 = msg->channel3;
  channel4 = msg->channel4;
  channel5 = msg->channel5;
  channel6 = msg->channel6;
  channel7 = msg->channel7;
  channel8 = msg->channel8;

  lastUpdateTime = millis();
}


void failsafe() {
  if (millis() - lastUpdateTime > 2500) {
    channel2 = 1500;
    channel3 = 990;
    channel1 = 1500;
    servoEins.writeMicroseconds(1500);
    servoZwei.writeMicroseconds(1500);

  }
}