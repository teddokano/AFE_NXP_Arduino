# AFE_NXP_Arduino
Analog Front End device operation sample code for [Arduino](https://www.arduino.cc) 

## What is this?
An Arduino library for NXP Analog Front End device with sample code.  
This library provides simple API to get analog input channels.  
Include device name header file (`NAFE1338.h`) to use those class libraries. 

With `AFE_NXP_Arduino` library, characters can be shown by next sample code. 

> **Note**  
> Current version of NAFE13388_UIM class and its sample code is supporting Arduino **UNO R3 only**. 

![UIM_on_FRDM.jpg](https://github.com/teddokano/additional_files/blob/main/AFE_NXP_Arduino/UIM.jpg)  
_NAFE13388-UIM 8 Channels Universal Input AFE Evaluation Board with Arduino UNO R3_

![Boards](https://github.com/teddokano/additional_files/blob/main/AFE_NXP_Arduino/afe.jpg)  
_NAFExx388-EVB 8 Channels Universal Input AFE Evaluation Board with Arduino UNO R3_

![NAFE13388.jpg](https://github.com/teddokano/additional_files/blob/main/AFE_NXP_Arduino/NAFE13388.jpg)
*Analog-Front-End chip: NAFE13388*



```cpp
#include <NAFE13388.h>

NAFE13388 afe;

void setup() {
  Serial.begin(9600);
  Serial.println("\n***** Hello, NAFE13388! *****");

  SPI.begin();
  
  afe.begin();
  afe.blink_leds();
  
  afe.logical_ch_config(0, 0x1710, 0x00BC, 0x4C80, 0x0000);
  afe.logical_ch_config(1, 0x2710, 0x00BC, 0x4C80, 0x0000);

  Serial.println("logical channel 0 (AI1P-AICOM) and 1 (AI2P-AICOM) voltages are shown in micro-volt");
}

void loop() {
  Serial.print(afe.read(0));
  Serial.print(",  ");
  Serial.println(afe.read(1));
  delay(100);
}
```

## Supported device
Type#|Features|Interface
---|---|---
[NAFE13388](https://www.nxp.com/products/peripherals-and-logic/signal-chain/analog-front-end/highly-configurable-8-channel-25-v-universal-input-low-power-analog-front-end:NAFEx1388)	|Highly Configurable 8 Channel ±25 V Universal Input Low Power Analog Front-End	|SPI (mode1)

## Evaluation boards
Name|Header file|Features|Interface|Evaluation board
---|---|---|---|---
[NAFExx388-EVB](https://www.nxp.com/design/design-center/development-boards-and-designs/NAFExx388-EVB)	|`NAFE13388.h`	|	General purpose evaluation board for NAFE13388 |SPI (mode1)	|[NAFE13388-UIM 8-Channel Universal Input AFE Arduino® Shield Board](https://www.nxp.com/design/design-center/development-boards-and-designs/NAFExx388-EVB)
[NAFE13388_UIM](https://www.nxp.com/products/peripherals-and-logic/signal-chain/analog-front-end/highly-configurable-8-channel-25-v-universal-input-low-power-analog-front-end:NAFEx1388)	|`NAFE13388_UIM.h`	|AFE Arduino shield type evaluation board |SPI (mode1)	|[NAFE13388-UIM 8-Channel Universal Input AFE Arduino® Shield Board](https://www.nxp.com/design/design-center/development-boards-and-designs/NAFE13388-UIM)

# Getting started
How to setup? --> [https://youtu.be/UEOFC0TwA6c](https://youtu.be/UEOFC0TwA6c)  
![Boards](https://github.com/teddokano/additional_files/blob/main/AFE_NXP_Arduino/demo_video.png) 

# What's inside?

## Examples
Examples are provided as scketch files.

### How to use?

After library install, Choose menu on Arduino-IDE: `File`→`Examples`→`AFE_NXP_Arduino`→ **sketch for your try**

### List of sample code

Sketch|Target|Feature
---|---|---
NAFE13388_simple		|NAFE13388-FVB	|**Simple** sample for just showing 2ch outputs in micro-volt value
NAFE13388_UIM_simple		|NAFE13388-UIM	|**Simple** sample for just showing 2ch outputs in micro-volt value


# Document
For details of the library, please find descriptions in [this document](https://teddokano.github.io/AFE_NXP_Arduino/annotated.html).
