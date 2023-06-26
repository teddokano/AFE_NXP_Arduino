# AFE_NXP_Arduino
Analog Front End device operation sample code for [Arduino](https://www.arduino.cc) 

![Boards](https://github.com/teddokano/additional_files/blob/main/AFE_NXP_Arduino/afe.jpg)  
_NAFExx388-EVB 8 Channels Universal Input AFE Evaluation Board with Arduino_

## What is this?
An Arduino library for NXP Analog Front End device with sample code.  
This library provides simple API to get analog input channels.  
Include device name header file (`NAFE1338.h`) to use those class libraries. 

With `AFE_NXP_Arduino` library, characters can be shown by next sample code. 
```cpp
#include <NAFE13388.h>

NAFE13388 afe;

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;

  SPI.begin();
  afe.begin();

  Serial.println("\n***** Hello, NAFE13388! *****");

  afe.logical_ch_config(0, 0x1150, 0x00AC, 0x1400, 0x0000);
  afe.logical_ch_config(1, 0x3350, 0x00A4, 0x1400, 0x3060);

  Serial.println("logical channel 0 and 1 are shown in micro-volt");
}

void loop() {
  Serial.print(afe.read(0));
  Serial.print(",  ");
  Serial.println(afe.read(1));
  delay(100);
}
```

## Supported device
Type#|Header file|Features|Interface|Evaluation board
---|---|---|---|---
[NAFE13388](https://www.nxp.com/products/peripherals-and-logic/signal-chain/analog-front-end/highly-configurable-8-channel-25-v-universal-input-low-power-analog-front-end:NAFEx1388)	|`NAFE13388.h`	|Highly Configurable 8 Channel ±25 V Universal Input Low Power Analog Front-End	|SPI (mode1)	|[NAFExx388-EVB 8 Channels Universal Input AFE Evaluation Board](https://www.nxp.com/design/development-boards/analog-toolbox/nafexx388-evb-8-channels-universal-input-afe-evaluation-board:NAFExx388-EVB)

# Getting started

Use Library manager in Arduino IDE for easy install

# What's inside?

## Examples
Examples are provided as scketch files.

### How to use?

After library install, Choose menu on Arduino-IDE: `File`→`Examples`→`LCDDrivers_NXP_Arduino`→ **sketch for your try**

### List of sample code

Sketch|Target|Feature
---|---|---
NAFE13388_simple		|NAFE13388	|**Simple** sample for just showing 2ch outputs in micro-volt value


# Document
For details of the library, please find descriptions in [this document](https://teddokano.github.io/AFE_NXP_Arduino/annotated.html).
