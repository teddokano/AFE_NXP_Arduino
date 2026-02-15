/* 
 * NAFE13388-UIM board operation sample for Arduino
 * 
 *  Copyright: 2023 - 2026 Tedd OKANO
 *  Released under the MIT license
 *
 * Sample of using SCSR(Single Channel Single Read) command for 2 channels
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

NAFE13388_UIM afe;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("\n***** Hello, NAFE13388! *****");

  SPI.begin();
  pinMode(SS, OUTPUT);  //  Required for UNO R4

  afe.begin();
  afe.blink_leds();

  using LC = NAFE13388_UIM::LogicalChannel;

  afe.logical_channel[0].setting.hv_aip = LC::HV_AIP::AI1P;
  afe.logical_channel[0].setting.hv_ain = LC::HV_AIN::AICOM;
  afe.logical_channel[0].setting.ch_gain = LC::CH_GAIN::x0_2;
  afe.logical_channel[0].setting.hv_sel = LC::HV_SEL::HV_AIP_HV_AIN;
  afe.logical_channel[0].setting.tcc_off = LC::TCC_OFF::On;
  afe.logical_channel[0].setting.adc_data_rate = 20;
  afe.logical_channel[0].setting.adc_sinc = LC::ADC_SINC::SINC4;
  afe.logical_channel[0].setting.ch_delay = LC::CH_DELAY::_23040;
  afe.logical_channel[0].setting.adc_normal_settling = LC::ADC_NORMAL_SETTLING::Single;

  afe.logical_channel[1].setting.hv_aip = LC::HV_AIP::AICOM;
  afe.logical_channel[1].setting.hv_ain = LC::HV_AIN::AI1N;
  afe.logical_channel[1].setting.ch_gain = LC::CH_GAIN::x0_2;
  afe.logical_channel[1].setting.hv_sel = LC::HV_SEL::HV_AIP_HV_AIN;
  afe.logical_channel[1].setting.tcc_off = LC::TCC_OFF::On;
  afe.logical_channel[1].setting.adc_data_rate = 20;
  afe.logical_channel[1].setting.adc_sinc = LC::ADC_SINC::SINC4;
  afe.logical_channel[1].setting.ch_delay = LC::CH_DELAY::_23040;
  afe.logical_channel[1].setting.adc_normal_settling = LC::ADC_NORMAL_SETTLING::Single;

  afe.logical_channel[0].configure();
  afe.logical_channel[1].configure();

  Serial.println("\nlogical channel 0 (AI1P-AICOM) and 1 (AI2P-AICOM) voltages are shown in ADC readout value [V]");

  afe.use_DRDY_trigger(true);
}

void loop() {
  Serial.print((NAFE13388_UIM::microvolt_t)afe.logical_channel[0] * 1e-6);
  Serial.print(",  ");
  Serial.println((NAFE13388_UIM::microvolt_t)afe.logical_channel[1] * 1e-6);
}
