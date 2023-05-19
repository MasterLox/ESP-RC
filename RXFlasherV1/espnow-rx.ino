#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>

// WiFi channel
uint8_t wifiChannel = 1;

// ESP-NOW message structure
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

// Global variables to store channel data
uint16_t channel1 = 0;
uint16_t channel2 = 0;
uint16_t channel3 = 0;
uint16_t channel4 = 0;
uint16_t channel5 = 0;
uint16_t channel6 = 0;
uint16_t channel7 = 0;
uint16_t channel8 = 0;


unsigned long lastUpdateTime = 0; // Initialize last update time to 0

int i =1000;
byte servoPinZwei = 21; // signal pin for the ESC.
Servo servoZwei;

byte servoPin = 22; // signal pin for the ESC.
Servo escservo;

byte servoPinEins = 23; // signal pin for the servoEins.
Servo servoEins;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

    // Servo
  servoEins.attach(servoPinEins);
  escservo.attach(servoPin);
  servoZwei.attach(servoPinZwei);
  delay(1000);

  escservo.writeMicroseconds(1000);
  delay(5000); // delay to allow the ESC to recognize the stopped signal.

  // Initialize WiFi and ESP-NOW
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register ESP-NOW receiver callback
  esp_now_register_recv_cb(onEspNowReceive);


}

void loop() {
  // throttle
  escservo.writeMicroseconds(channel3); 
  failsafe();
  if (channel2 > 1690 || channel2 < 1310) {
    lastUpdateTime = millis(); 
    //pitch
    servoEins.writeMicroseconds(channel2);
    servoZwei.writeMicroseconds(2000 - channel2 + 1000);
  } else {
    // execute roll code
    servoEins.writeMicroseconds(channel1);
    servoZwei.writeMicroseconds(channel1);
  }
}



void onEspNowReceive(const uint8_t *macAddr, const uint8_t *data, int dataLen) {
  // Check message length
  if (dataLen != sizeof(EspNowMessage)) {
    Serial.println("Invalid ESP-NOW message length");
    return;
  }

  // Parse message
  EspNowMessage *msg = (EspNowMessage *)data;

  // Print channel data
  //Serial.print("Channel 1: ");
  //Serial.println(msg->channel1);
  //Serial.print("Channel 2: ");
  //Serial.println(msg->channel2);
  //Serial.print("Channel 3: ");
  //Serial.println(msg->channel3);
  //Serial.print("Channel 4: ");
  //Serial.println(msg->channel4);
  //Serial.print("Channel 5: ");
  //Serial.println(msg->channel5);
  //Serial.print("Channel 6: ");
  //Serial.println(msg->channel6);
  //Serial.print("Channel 7: ");
  //Serial.println(msg->channel7);
  //Serial.print("Channel 8: ");
  //Serial.println(msg->channel8);
  //delay(70);

    // Update global variables with channel data
  channel1 = msg->channel1;
  channel2 = msg->channel2;
  channel3 = msg->channel3;
  channel4 = msg->channel4;
  channel5 = msg->channel5;
  channel6 = msg->channel6;
  channel7 = msg->channel7;
  channel8 = msg->channel8;
  // Update last update time to current time
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
