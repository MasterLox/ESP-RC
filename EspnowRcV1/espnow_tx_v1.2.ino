#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//screen
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// PPM signal input pin
int ppmPin = 2;
char rxda = 'n';

// PPM signal array
volatile uint16_t ppmValues[8];

// PPM signal counter
volatile uint8_t ppmCount = 0;

// PPM signal last received time
volatile unsigned long ppmLastTime = 0;

// PPM channel variables
uint16_t ppmChannel1, ppmChannel2, ppmChannel3, ppmChannel4, ppmChannel5, ppmChannel6, ppmChannel7, ppmChannel8;

// ESP-NOW peer address
// ADD YOUR OWN !!!
// ADD YOUR OWN !!!
//
uint8_t broadcastAddress[] = {0x10, 0x52, 0x1C, 0x62, 0x9C, 0x4C};

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

void setup() {
  // Set PPM signal pin as input
  pinMode(ppmPin, INPUT);

  // Attach PPM signal interrupt
  attachInterrupt(digitalPinToInterrupt(ppmPin), ppmInterrupt, CHANGE);

  // Initialize serial communication
  Serial.begin(115200);
  oledsetup();
  // Initialize WiFi and ESP-NOW
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register ESP-NOW peer
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

void oledsetup() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println("ESP-Rc v1.1!");
  display.setCursor(0, 20);
  display.println("RX Connect=");
  display.display(); 


}



void loop() {

  // Create ESP-NOW message
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

  } else {
    Serial.println("Error sending ESP-NOW message");
  }

  // Wait for 10ms
  delay(10);
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
