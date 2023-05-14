#include <ESP32Servo.h>
#include <SPI.h>
#include <LoRa.h>
#define ss 18
#define rst 14
#define dio0 26

byte escP = 22; 
byte servo1P = 0;
byte servo2P = 0;
byte servo3P = 0;
byte switch1P = 0;
byte switch2P = 0;

Servo esc;
Servo servo1;
Servo servo2;
Servo servo3;

Servo switch1;
Servo switch2;

int ppmChannel1, ppmChannel2, ppmChannel3, ppmChannel4, ppmChannel5, ppmChannel6;

void setup() {
  Serial.begin(115200);
  SPI.begin(5, 19, 27, 18);
  LoRa.setPins(ss, rst, dio0);
  LoRa.begin(433E6);

  esc.attach(escP);
  servo1.attach(servo1P);
  servo2.attach(servo2P);
  servo3.attach(servo3P);
  switch1.attach(switch1P);
  switch2.attach(switch2P);

  esc.writeMicroseconds(1000); // Arm ESC
  delay(500); 
}

void setChannels(){
 if (LoRa.parsePacket()) {
    String packet = "";
    while (LoRa.available()) {
      packet += (char)LoRa.read();
    }
  

    if (packet.startsWith("S") && packet.endsWith("E")) {
      packet = packet.substring(1, packet.length() - 1);
      int commaPos = packet.indexOf(",");
      if (commaPos != -1) {
        ppmChannel1 = packet.substring(0, commaPos).toInt();
        packet = packet.substring(commaPos + 1);

        commaPos = packet.indexOf(",");
        if (commaPos != -1) {
          ppmChannel2 = packet.substring(0, commaPos).toInt();
          packet = packet.substring(commaPos + 1);

          commaPos = packet.indexOf(",");
          if (commaPos != -1) {
            ppmChannel3 = packet.substring(0, commaPos).toInt();
            packet = packet.substring(commaPos + 1);

            commaPos = packet.indexOf(",");
            if (commaPos != -1) {
              ppmChannel4 = packet.substring(0, commaPos).toInt();
              packet = packet.substring(commaPos + 1);

              commaPos = packet.indexOf(",");
              if (commaPos != -1) {
                ppmChannel5 = packet.substring(0, commaPos).toInt();
                ppmChannel6 = packet.substring(commaPos + 1).toInt();
              }
            }
          }
        }
      }
    }
  }
}
void driveComponents(){
  esc.writeMicroseconds(ppmChannel1);
  servo1.writeMicroseconds(ppmChannel2);
  servo2.writeMicroseconds(ppmChannel3);
  servo3.writeMicroseconds(ppmChannel4);
  switch1.writeMicroseconds(ppmChannel5);
  switch2.writeMicroseconds(ppmChannel6);
}

void loop() {  
    setChannels();
    driveComponents();
    Serial.println(ppmChannel1);
    delay(10);
}
