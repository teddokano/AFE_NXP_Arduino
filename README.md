# AFE_NXP_Arduino
Analog Front End device operation sample code for [Arduino](https://www.arduino.cc) 

## What is this?
An Arduino library for NXP Analog Front End device with sample code.  
This library provides simple API to get analog input channels.  
Include device name header file (`NAFE13388.h`) to use those class libraries. 

With `AFE_NXP_Arduino` library, characters can be shown by next sample code. 

> **Note**  
> Current version of NAFE13388_UIM class and its sample code is supporting Arduino **UNO R3 only**. 

```cpp
#include <NAFE13388.h>

NAFE13388 afe;

void setup() {
  Serial.begin(9600);

  Serial.println("\n***** Hello, NAFE13388! *****");

  SPI.begin();

  afe.begin();
  afe.blink_leds();

  afe.open_logical_channel(0, 0x1110, 0x00BC, 0x4C80, 0x0000);
  afe.open_logical_channel(1, 0x2210, 0x00BC, 0x4C80, 0x0000);

  Serial.println("\nlogical channel 0 (AI1P-AI1N) and 1 (AI2P-AI2N) voltages are shown in ADC readout raw value");
}

void loop() {
  Serial.print(afe.start_and_read(0));
  Serial.print(",  ");
  Serial.println(afe.start_and_read(1));
}
```

![UIM_on_FRDM.jpg](https://github.com/teddokano/additional_files/blob/main/AFE_NXP_Arduino/UIM.jpg)  
_NAFE13388-UIM 8 Channels Universal Input AFE Evaluation Board with Arduino UNO R3_

![OPREF pin for UIM.jpg](https://github.com/teddokano/additional_files/blob/main/AFE_NXP_Arduino/IOREF.jpg)  
_For the UIM board, IOREF pin need to be unconnected to avoid supply conflict_

![Boards](https://github.com/teddokano/additional_files/blob/main/AFE_NXP_Arduino/afe.jpg)  
_NAFExx388-EVB 8 Channels Universal Input AFE Evaluation Board with Arduino UNO R3_

![NAFE13388.jpg](https://github.com/teddokano/additional_files/blob/main/AFE_NXP_Arduino/NAFE13388.jpg)
*Analog-Front-End chip: NAFE13388*



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
NAFE13388_simple				|NAFE13388-EVB	|**Simple** sample to get 2ch inputs
NAFE13388_UIM_simple			|NAFE13388-UIM	|**Simple** sample to get 2ch inputs on UIM board
NAFE13388_UIM_multichannel_read	|NAFE13388-UIM	|**8ch** inputs sample on UIM board
NAFE13388_UIM_self_calibration	|NAFE13388-UIM	|A sample of performing **self calibration** on UIM board


# Document
For details of the library, please find descriptions in [this document](https://teddokano.github.io/AFE_NXP_Arduino/annotated.html).
