#include <SPI.h>
#include <LoRa.h>
#define ss 18
#define rst 14
#define dio0 26

// PPM signal input pin
int ppmPin = 22;

// PPM signal array
volatile uint16_t ppmValues[8];

// PPM signal counter
volatile uint8_t ppmCount = 0;

// PPM signal last received time
volatile unsigned long ppmLastTime = 0;

// PPM channel variables
uint16_t ppmChannel1, ppmChannel2, ppmChannel3, ppmChannel4, ppmChannel5, ppmChannel6;

void setup() {
  // Set PPM signal pin as input
  pinMode(ppmPin, INPUT);
  SPI.begin(5, 19, 27, 18);
  LoRa.setPins(ss, rst, dio0);
  // Attach PPM signal interrupt
  attachInterrupt(digitalPinToInterrupt(ppmPin), ppmInterrupt, CHANGE);

  // Initialize serial communication
  Serial.begin(9600);
  LoRa.begin(433E6);
}

void loop() {
 
  sendData();
  delay(10);
}

void sendData() {
  LoRa.beginPacket();
  LoRa.print('S' + String(ppmChannel1) + ',' + String(ppmChannel2) + ',' + String(ppmChannel3) + ',' + String(ppmChannel4) + ',' + String(ppmChannel5) + ',' + String(ppmChannel6) + "," + String(ppmChannel7) + ',' + String(ppmChannel8) + 'E');
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
    ppmCount = 0;
  }

  ppmLastTime = ppmTime;
}
