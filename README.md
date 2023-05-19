# ESP-RC


ESP-RC is a project that utilizes ESP32 microcontrollers with both ESPNow and LoRa communication protocols to provide long-range, low-latency control of aircrafts. This enables users to remotely control the flight of their aircrafts using wireless remote controls. With its powerful processing capabilities and reliable wireless connectivity, ESP-RC is an ideal solution for cheap long range purposes.

## 1. Setup
Either use the TX and RX Flasher or flash it via Arduino IDE

### 1.1 Using the Flasher 
You have to have python installed 

Execute Guimain.exe or the Guimain.py and set the pins for your module
![The Flasher]([http://url/to/img.png](https://raw.githubusercontent.com/MasterLox/ESP-RC/main/image/flasher.png?token=GHSAT0AAAAAACAHZ2D55ZNHJKF7SLB7LQVMZDHXRYQ))

Save the Pins and procede to Build and Flash


### 2.1 Update the pin assignments in the code to match the GPIO pins on your ESP32 board.
```yaml
int ppmPin = 2;        //InputPin für PPM
int b1 = 17;     //InputPin für b1
int b2 = 16;      
int zeit = 0;
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 22, /* data=*/ 21, /* reset=*/ U8X8_PIN_NONE);
```

### 2.2 Configure the radio settings (LoRa or ESPNow) based on the new board's specifications.
```yaml
uint8_t broadcastAddress[] = {0x10, 0x52, 0x1C, 0x62, 0x9C, 0x4C}; //Define ESP-Rx-MacAdress

```

### 2.3 Modify the data transmission functions (sendDataLoRa() and sendDataESPNow()) if necessary.
