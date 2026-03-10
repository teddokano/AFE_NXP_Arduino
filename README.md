# AFE_NXP_Arduino
Analog Front End device operation sample code for [Arduino](https://www.arduino.cc) 

## What is this?
An Arduino library for NXP Analog Front End device with sample code.  
This library provides simple API to get analog input channels.  
Include device name header file (`NAFE13388_UIM.h` or `NAFE33352_UIOM.h`) to use those class libraries.   

This repo shoiws some basic operations of NAFE13388_UIM as well as practical samples to work 
with [RTD](https://github.com/teddokano/AFE_NXP_Arduino/tree/main/examples/NAFE13388_5_0_RTD_4_wire), 
[thermocouple](https://github.com/teddokano/AFE_NXP_Arduino/tree/main/examples/NAFE13388_6_0_Thermocouple) and 
[loadcell](https://github.com/teddokano/AFE_NXP_Arduino/tree/main/examples/NAFE13388_7_0_LoadCell). 

## Easy to use

3 types of Arduino UNO boards: **R3**, **R4 Minima** and **R4 WiFi** are supported.  
> **Note**  
> For any type of Arduino board and NAFE13388-UIM/NAFE33352-UIOM board connection, **DON'T PLUG ARDUINO SHIELD SOCKET DIRECTLY!!**   
> The 3.3V supply pin should be disconected to protect Arduino suppy circuit.   
> Please refer to pictures in later in this page.   

Example sketches can be built and run on any of those boards.  

### Simple example code for NAFE13388-UIM
Next is a sample of the basic operation of NAFE13388-UIM measureing analog voltage on AI1P and AI1N terminal.    

```cpp
#include <NAFE13388_UIM.h>

NAFE13388_UIM afe;	//	make an instance of NAFE13388_UIM

void setup() {
  //  serial channel (between PC and Arduino) setup
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("\n***** Hello, NAFE13388! *****");

  //  SPI bus setup
  SPI.begin();
  pinMode(SS, OUTPUT);  //  Required for UNO R4

  //  AFE reset and check connection
  afe.begin();
  afe.blink_leds();

  //  ADC logical chennel setup
  afe.logical_channel[0].configure(0x1710, 0x00A4, 0xBC00, 0x0000);
  afe.logical_channel[1].configure(0x2710, 0x00A4, 0xBC00, 0x0000);

  Serial.println("\nlogical channel 0 (AI1P-AICOM) and 1 (AI2P-AICOM) voltages are shown in ADC readout value [V]");

  //  To use DRDY signal to detect AD conversion done
  afe.use_DRDY_trigger(true);
}

void loop() {
  //  Show ADC read value on volt
  Serial.print((NAFE13388_UIM::volt_t)afe.logical_channel[0]);
  Serial.print(",  ");
  Serial.println((NAFE13388_UIM::volt_t)afe.logical_channel[1]);
}
```

To try this sample code, set jumper plug on J91 to short AICOM and GND.  
The analog input terminals are available as picture below.  

![](https://github.com/teddokano/additional_files/blob/main/AFE_NXP_Arduino/termianl_and_jumper.jpg)  


### Simple example code for NAFE33352-UIOM
Next is a sample of the basic operation of NAFE33352-UIOM measureing analog voltage 
beween AI1P and VCM with DAC working to output voltage or current.  
```cpp
#include <NAFE33352_UIOM.h>

NAFE33352_UIOM shasta;	//	make an instance of NAFE33352_UIOM

#define VOLTAGE_OUTPUT_SETTING	//	demo type selector: comment-out to select current output
double output_value;
int count = 0;

void setup() {
  //  serial channel (between PC and Arduino) setup
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("\n***** Hello, NAFE33352_UIOM! *****");

  //  SPI bus setup
  SPI.begin();
  pinMode(SS, OUTPUT);  //  Required for UNO R4

  //  AFE reset and check connection
  shasta.begin();

#ifdef VOLTAGE_OUTPUT_SETTING
  //  DAC setup: configure to voltage output
  output_value = 5.00;  //	5V
  shasta.dac.configure(NAFE33352_UIOM::DAC::ModeSelect::VOLTAGE);
#else
  //  DAC setup: configure to current output
  output_value = 20 * 1e-3;  //	20mA
  shasta.dac.configure(NAFE33352_UIOM::DAC::ModeSelect::CURRENT);
#endif

  //  Set DAC output
  shasta.dac = output_value;
  delay( 100 );

  //  ADC logical chennel setup
  shasta.logical_channel[0].configure(0x0020, 0x50B4, 0x5000);  //  voltage between AI1P - VCOM
  shasta.logical_channel[1].configure(0x0080, 0x5064, 0x5000);  //  supply monitoring: VHDD
  shasta.logical_channel[2].configure(0x0088, 0x5064, 0x5000);  //  supply monitoring: VHSS
  shasta.logical_channel[3].configure(0x0038, 0x2064, 0x5000);  //  VSNS voltage
  shasta.logical_channel[4].configure(0x0030, 0x3064, 0x5000);  //  ISNS current
}

void loop() {
  double data;
  
  //  show ADC measured data (in volt and ampere)
  for (auto i = 0; i < shasta.enabled_logical_channels(); i++) {
    data = shasta.logical_channel[i];
    Serial.print(data, 9);
    Serial.print(",    ");
  }

  //  show AIO_STATUS register value
  Serial.println(shasta.reg(NAFE33352_UIOM::Register16::AIO_STATUS), HEX);

  //  alternate DAC output polarity
  shasta.dac = output_value * (count++ & 0x1 ? +1.00 : -1.00);

  delay(1000);
}
```

### Board connections

Followings are pictures of UIM with UNO R3, UNO R4 Minima and UNO R4 WiFi boards.  
On every picture, showing the hardware setting of..
  - 3.3V are disconnected by using pin extender
  - A jumper plug is set on J91
  - D2 and D4 pins are shorted to enable to use DRDY signal

![uim_on_r3.jpg](https://github.com/teddokano/additional_files/blob/main/AFE_NXP_Arduino/uim_on_r3.jpg)  
_NAFE13388-UIM 8 Channels Universal Input AFE Evaluation Board with Arduino UNO R3_

![uim_on_r4_wifi_.jpg](https://github.com/teddokano/additional_files/blob/main/AFE_NXP_Arduino/uim_on_r4_wifi_.jpg)  
_NAFE13388-UIM 8 Channels Universal Input AFE Evaluation Board with Arduino UNO R4 Minima_

![uim_on_r4_minima.jpg](https://github.com/teddokano/additional_files/blob/main/AFE_NXP_Arduino/uim_on_r4_minima.jpg)  
_NAFE13388-UIM 8 Channels Universal Input AFE Evaluation Board with Arduino UNO R4 WiFi_

> **Note**  
> For proper voltage signal interfacing, connection between Arduino MCU board and NAFE13388-UIM board need to be modified. **Remove 3.3V supply pin** to let UIM to support 5V signaling.   
> ![](https://github.com/teddokano/additional_files/blob/main/AFE_NXP_Arduino/uim_3v3_pin_lifted_at_extender.jpg)  
> _3.3V supply pin is lifted at pin extender_  
> 
> ![](https://github.com/teddokano/additional_files/blob/main/AFE_NXP_Arduino/UIM_modification.png)  
> _Diagram of 3.3V and 5V supply rails on NAFE13388-UIM board's schematic_
> 
> ![](https://github.com/teddokano/additional_files/blob/main/AFE_NXP_Arduino/ioref.png)  
> Using 5V supply on IOREF pin from Arduino board_



![Boards](https://github.com/teddokano/additional_files/blob/main/AFE_NXP_Arduino/afe.jpg)  
_NAFExx388-EVB 8 Channels Universal Input AFE Evaluation Board with Arduino UNO R3_

![NAFE13388.jpg](https://github.com/teddokano/additional_files/blob/main/AFE_NXP_Arduino/NAFE13388.jpg)
*Analog-Front-End chip: NAFE13388*



## Supported device
Type#|Features|Interface
---|---|---
[NAFE13388](https://www.nxp.com/products/peripherals-and-logic/signal-chain/analog-front-end/highly-configurable-8-channel-25-v-universal-input-low-power-analog-front-end:NAFEx1388)	|Highly Configurable 8 Channel ±25 V Universal Input Low Power Analog Front-End	|SPI (mode1)
[NAFE73388](https://www.nxp.com/products/peripherals-and-logic/signal-chain/analog-front-end/highly-configurable-8-channel-25-v-universal-input-low-power-analog-front-end:NAFEx1388)	|Highly Configurable 8 Channel ±25 V Universal Input Low Power Analog Front-End	|SPI (mode1)
[NAFE33352](https://www.nxp.com/products/NAFEx3352)	|Software Configurable Input and Output Analog Front End|SPI (mode1)
[NAFE93352](https://www.nxp.com/products/NAFEx3352)	|Software Configurable Input and Output Analog Front End|SPI (mode1)

## Evaluation boards
Name|Header file|Features|Interface|Evaluation board
---|---|---|---|---
[NAFExx388-EVB](https://www.nxp.com/design/design-center/development-boards-and-designs/NAFExx388-EVB)	|`NAFE13388.h`	|	General purpose evaluation board for NAFE13388 |SPI (mode1)	|[NAFE13388-UIM 8-Channel Universal Input AFE Arduino® Shield Board](https://www.nxp.com/design/design-center/development-boards-and-designs/NAFExx388-EVB)
[NAFE13388-UIM](https://www.nxp.com/products/peripherals-and-logic/signal-chain/analog-front-end/highly-configurable-8-channel-25-v-universal-input-low-power-analog-front-end:NAFEx1388)	|`NAFE13388_UIM.h`	|AFE Arduino shield type evaluation board |SPI (mode1)	|[NAFE13388-UIM 8-Channel Universal Input AFE Arduino® Shield Board](https://www.nxp.com/design/design-center/development-boards-and-designs/NAFE13388-UIM)
[NAFE33352-EVB](https://www.nxp.com/design/design-center/development-boards-and-designs/NAFE33352-EVB)	|`NAFE13388_UIM.h`	|AFE Arduino shield type evaluation board |SPI (mode1)	|[Software Configurable Universal AIO-AFE Evaluation Board](https://www.nxp.com/design/design-center/development-boards-and-designs/NAFE33352-EVB)
[NAFE33352-UIOM]()	|`NAFE13388_UIM.h`	|AFE Arduino shield type evaluation board |SPI (mode1)	|[NAFE33352-UIOM Universal Input/OUTPUT AFE Arduino® Shield Board]()

# Getting started
How to setup? --> [https://youtu.be/UEOFC0TwA6c](https://youtu.be/UEOFC0TwA6c)  
![Boards](https://github.com/teddokano/additional_files/blob/main/AFE_NXP_Arduino/demo_video.png) 

# What's inside?

## Examples
Examples are provided as scketch files.

### How to use?

After library install, Choose menu on Arduino-IDE: `File`→`Examples`→`AFE_NXP_Arduino`→ **sketch for your try**

### List of sample code

#### examples for NAFE13388
Sketch|Target|Feature|NOTE
---|---|---|---
NAFE13388_1_0_simple_SCSR|NAFE13388-UIM	|**Simple** sample to get 2ch inputs|
NAFE13388_1_1_simple_SCSR_using_DRDY_signal|NAFE13388-UIM	|**Simple** sample to get 2ch inputs using DRDY signal|Need to short pins between D2 and D4
NAFE13388_2_0_multichannel_read_MCMR|NAFE13388-UIM	|Get 8ch inputs using Multi-Channel Multi-Read command| 
NAFE13388_2_1_multichannel_read_MCMR_using_DRDY|NAFE13388-UIM		|Get 8ch inputs using Multi-Channel Multi-Read command| Need to short pins between D2 and D4
NAFE13388_3_0_multichannel_read_MCCR_using_DRDY|NAFE13388-UIM	|Get 8ch inputs using Multi-Channel **Continuous**-Read command| Need to short pins between D2 and D4
NAFE13388_4_0_LVMUX_multichannel_read_MCCR_using_DRDY|NAFE13388-UIM	|Get 8ch inputs and LVMUX inputs (supply voltage, etc)|Need to short pins between D2 and D4
[NAFE13388_5_0_RTD_4_wire](https://github.com/teddokano/AFE_NXP_Arduino/tree/main/examples/NAFE13388_5_0_RTD_4_wire)|NAFE13388-UIM	|Practical sample to measure temperature by 2 RTDs using internal current excitation source|Need to short pins between D2 and D4
[NAFE13388_6_0_Thermocouple](https://github.com/teddokano/AFE_NXP_Arduino/tree/main/examples/NAFE13388_6_0_Thermocouple)|NAFE13388-UIM	|Practical sample to measure temperature by thermocouple with on-board RTD |Need to short pins between D2 and D4
[NAFE13388_7_0_LoadCell](https://github.com/teddokano/AFE_NXP_Arduino/tree/main/examples/NAFE13388_7_0_LoadCell)|NAFE13388-UIM	|Practical sample to measure loadcell output using internal voltage excitation source |Need to short pins between D2 and D4

#### examples for NAFE33352
Sketch|Target|Feature|NOTE
---|---|---|---
NAFE33352_1_0_ADC_and_DAC	|NAFE33352-UIOM	|A sample of ADC and DAC operation|Need to short pins between D2 and D4
NAFE33352_2_0_AIO_input|NAFE33352-UIOM	|A sample of AIO (Analog Input/Output) channel used as an input|Need to short pins between D2 and D4

# Document
For details of the library, please find descriptions in [this document](https://teddokano.github.io/AFE_NXP_Arduino/annotated.html).

