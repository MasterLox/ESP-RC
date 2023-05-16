#include <SPI.h>
#include <LoRa.h>
#include <esp_now.h>
#include <WiFi.h>
#include <U8x8lib.h>
#include <stdio.h>
#include "esp_system.h"

#define ss 18
#define rst 14
#define dio0 26

     
boolean espconnect = false; 
boolean str = true;    //Standart Boot Modus (true = LoRa, false = ESPNow)
int ppmPin = 2;        //InputPin für PPM
int b1 = 17;     //InputPin für b1
int b2 = 16;
int zeit = 0;
int sentCount = 0;  // Counter variable to track the number of "ESP-NOW message sent" occurrences

uint8_t broadcastAddress[] = {0x10, 0x52, 0x1C, 0x62, 0x9C, 0x4C}; //Define ESP-Rx-MacAdress
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 22, /* data=*/ 21, /* reset=*/ U8X8_PIN_NONE);
      //Setup OLED



uint8_t wifiChannel = 1;
volatile uint16_t ppmValues[8];
volatile uint8_t ppmCount = 0;
volatile unsigned long ppmLastTime = 0;
uint16_t ppmChannel1, ppmChannel2, ppmChannel3, ppmChannel4, ppmChannel5, ppmChannel6, ppmChannel7, ppmChannel8;
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

void setup() {
  Serial.begin(115200);
  pinMode(ppmPin, INPUT);
  pinMode(b1, INPUT_PULLUP);
  pinMode(b2, INPUT_PULLUP);
   u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0, 1, " ESP-RC-TX");
  u8x8.drawString(8, 7, " v1.4.3");
  attachInterrupt(digitalPinToInterrupt(ppmPin), ppmInterrupt, CHANGE);
  setMode();
 

}

void setMode(){
  boolean start = false;
  while(!start){
    if(digitalRead(b1)==LOW){
      if(str){
        str=false;
      }else{
        str=true;
      }
      delay(500);
    }
      if(str){
        u8x8.drawString(0, 2, ">LoRa");
        u8x8.drawString(0, 3, " ESPNow");
      }else{
        u8x8.drawString(0, 2, " LoRa");
        u8x8.drawString(0, 3, ">ESPNow");
      }
      if(digitalRead(b2)==LOW){
        start=true;
      }
      delay(100);
    }
  
  if(str){
    u8x8.begin();
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.drawString(0, 1, "ESP-RC Mode:");
    u8x8.drawString(0, 2, "LoRa-Modulation");
    u8x8.drawString(8, 7, " v1.4.3");
    SPI.begin(5, 19, 27, 18);
    LoRa.setPins(ss, rst, dio0);
    LoRa.begin(433E6);
  }else{
     uint32_t freeHeap = esp_get_free_heap_size();
    uint32_t minimumFreeHeap = esp_get_minimum_free_heap_size();
    printf("Free Heap: %u bytes\n", freeHeap);
    printf("Minimum Free Heap: %u bytes\n", minimumFreeHeap);
     u8x8.begin();
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.drawString(0, 1, "ESP-RC Mode:");
    u8x8.drawString(0, 2, "WiFi-Modulation");
    u8x8.drawString(8, 7, " v1.4.3");
    u8x8.drawString(0, 5, "RX= ");
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
      Serial.println("Error initializing ESP-NOW");
      return;
    }
    esp_now_peer_info_t peerInfo;
    memset(&peerInfo, 0, sizeof(peerInfo));
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = wifiChannel;
    peerInfo.encrypt = false;
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
      Serial.println("Error adding ESP-NOW peer");
    return;
  }
  }
  }




void loop() {
  if(str){
    sendDataLoRa();
  }else{
    sendDataESPNow();
  } 
  delay(10);
}



void sendDataESPNow(){
  EspNowMessage msg;
  msg.channel1 = ppmChannel1;
  msg.channel2 = ppmChannel2;
  msg.channel3 = ppmChannel3;
  msg.channel4 = ppmChannel4;
  msg.channel5 = ppmChannel5;
  msg.channel6 = ppmChannel6;
  msg.channel7 = ppmChannel7;
  msg.channel8 = ppmChannel8;

  // Send ESP-NOW message
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&msg, sizeof(msg));
  if (result == ESP_OK) {
    Serial.println("ESP-NOW message sent");
    sentCount++;  // Increment the counter variable

    if (sentCount == 100) {
      Serial.println("100 messages sent");
      sentCount = 0;
      
      if(espconnect){
        //do nothing
      }else{
        espconnect = true;
        u8x8.drawString(3, 5, "LINK   ");
       
      }


      }
  } else if (result == ESP_ERR_ESPNOW_NOT_INIT) {
    Serial.println("ESP-NOW not initialized");
  } else if (result == ESP_ERR_ESPNOW_ARG) {
    Serial.println("Invalid argument");
  } else if (result == ESP_ERR_ESPNOW_INTERNAL) {
    Serial.println("Internal error");
  } else if (result == ESP_ERR_ESPNOW_NO_MEM) {
    Serial.println("Out of memory");
    sentCount = 0;
    espconnect = false;
    u8x8.drawString(3, 5, "No LINK");
  }

}
void sendDataLoRa() {
  LoRa.beginPacket();
  LoRa.print('S' + String(ppmChannel1) + ',' + String(ppmChannel2) + ',' + String(ppmChannel3) + ',' + String(ppmChannel4) + ',' + String(ppmChannel5) + ',' + String(ppmChannel6)  + 'E');
  LoRa.endPacket();
}
void ppmInterrupt() {
  unsigned long ppmTime = micros();

  // Calculate time since last PPM signal pulse
  unsigned long ppmDelta = ppmTime - ppmLastTime;

  if (ppmDelta > 2000) {
    // End of PPM signal frame
    ppmCount = 0;
  } else if (ppmDelta > 500) {
    // PPM signal pulse
    ppmValues[ppmCount++] = ppmDelta;
  }

  if (ppmCount == 8) {
    // All PPM channels have been received
    ppmChannel1 = ppmValues[0] + 300;
    ppmChannel2 = ppmValues[1] + 300;
    ppmChannel3 = ppmValues[2] + 300;
    ppmChannel4 = ppmValues[3] + 300;
    ppmChannel5 = ppmValues[4] + 300;
    ppmChannel6 = ppmValues[5] + 300;
    ppmChannel7 = ppmValues[6] + 300;
    ppmChannel8 = ppmValues[7] + 300;
    ppmCount = 0;
  }

  ppmLastTime = ppmTime;
}