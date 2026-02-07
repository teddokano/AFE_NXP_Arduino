/* 
 * NAFE13388-UIM board operation sample for Arduino
 * 
 *  Copyright: 2023 - 2026 Tedd OKANO
 *  Released under the MIT license
 *
 * Sample of using MCMR(Multi Channel Continuous Read) command for 8 channels
 *
 * *****************************
 * ** BEFORE TRYING THIS CODE **
 * *****************************
 * This sample code shows operation with **DRDY** signal (at D4 pin) from AFE 
 * Since the DRDY signal is very short pulse, it will be captured by interrupt on D2 pin
 * Short D4 and D2 pin to handle it
 */

/*
  * ** CAUTION ** CAUTION ** CAUTION ** CAUTION ** CAUTION ** CAUTION ** CAUTION **
  * ** CAUTION ** CAUTION ** CAUTION ** CAUTION ** CAUTION ** CAUTION ** CAUTION **
  * ** CAUTION ** CAUTION ** CAUTION ** CAUTION ** CAUTION ** CAUTION ** CAUTION **
  * 
  * The NAFE13388-UIM board and Arduino mocrocontroller cannot be connected 
  * directly on Arduino-shield socket. The 3.3V supply should be disconnected. 
  * Visit next URL page to confirm how to do it. 
  *   -->  https://github.com/teddokano/AFE_NXP_Arduino/blob/main/README.md
  */

#include <NAFE13388_UIM.h>

double get_termocouple_temp(void);
double get_rtd_temp(void);

NAFE13388_UIM afe;

constexpr auto logical_channel_for_loadcell = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("\n***** Hello, NAFE13388! *****");

  SPI.begin();
  pinMode(SS, OUTPUT);  //  Required for UNO R4

  afe.begin();
  afe.blink_leds();

  //  Logical-channel for Thermocouple
  //
  //  measureing loadcell voltage between AI1P and AI1N pins
  //  using 9V excitation voltage from AI2P pin
  //  PGA gain = x16, coefficient set slot = 7 (for gainsetting x16),
  //  ADC data rate = 2Hz, delay for measurement start = 111.11us, filter = SINC4, CH_CHOP = off
  //
  //  ** NOTE **
  //  The excitation voltage is set to 9V but it may drop due to the loadcell resistance 
  //  since the excitation source has current limit. If you need to keep the voltage, in that case, 
  //  make buffering circuit externally. 
  afe.logical_channel[logical_channel_for_loadcell].configure(0x11F0, 0x70DC, 0x6800, 0x3810);

  Serial.print("\r\nenabled logical channel(s) = ");
  Serial.println(afe.enabled_logical_channels());
}

void loop() {
  double  microvolt = afe.logical_channel[logical_channel_for_loadcell];
  Serial.println(microvolt, 8);
}
