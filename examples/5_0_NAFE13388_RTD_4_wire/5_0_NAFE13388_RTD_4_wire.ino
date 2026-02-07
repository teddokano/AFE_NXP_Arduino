/* 
 * NAFE13388-UIM board operation sample for Arduino
 * 
 *  Copyright: 2023 - 2026 Tedd OKANO
 *  Released under the MIT license
 *
 * Sample of using MCMR(Multi Channel Continuous Read) command for 8 channels
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

double get_temp(int logical_channel_num);

NAFE13388_UIM afe;

constexpr auto r_32 = 2940.00;
constexpr auto r_34 = 2940.00;
constexpr auto r_series = r_32 + r_34;
constexpr auto vref_buf = 2.50;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("\n***** Hello, NAFE13388! *****");

  SPI.begin();
  pinMode(SS, OUTPUT);  //  Required for UNO R4

  afe.begin();
  afe.blink_leds();

  //  Logical-channel for RTD measurement by 4 wire method
  //
  //  measureing RTD voltage between AI2P and AI2N pins
  //  using 250uA excitation current from AI1P pin
  //  PGA gain = x16, coefficient set slot = 8,
  //  ADC data rate = 2.5Hz, delay for measurement start = 5000us, filter = SINC4, CH_CHOP = on
  afe.logical_channel[0].configure(0x22F8, 0x80C4, 0x8480, 0xA608);

  //  Logical-channel for RTD measurement by 4 wire method
  //  measureing RTD voltage between AI4P and AI4N pins
  //  using 250uA excitation current from AI3P pin
  //  PGA gain = x16, coefficient set slot = 8,
  //  ADC data rate = 2.5Hz, delay for measurement start = 5000us, filter = SINC4, CH_CHOP = on
  afe.logical_channel[1].configure(0x44F8, 0x80C4, 0x8480, 0xA618);

  //  Set factory calibrated coefficients for PGA gain=x16, with excitation current = 250uA
  //  using coefficient slot on 8
  //  reference : AN14539, Table 3
  afe.reg(NAFE13388_UIM::Register24::OFFSET_COEFF8, afe.reg(NAFE13388_UIM::Register24::OPT_COEF3));
  afe.reg(NAFE13388_UIM::Register24::GAIN_COEFF8, afe.reg(NAFE13388_UIM::Register24::OPT_COEF4));

  Serial.print("\r\nenabled logical channel(s) = ");
  Serial.println(afe.enabled_logical_channels());
}

void loop() {
  double temp0 = get_temp(0);
  double temp1 = get_temp(1);

  Serial.print(temp0, 8);
  Serial.print(", ");
  Serial.print(temp1, 8);
  Serial.println("");
}

double get_temp(int logical_channel_num) {
  static constexpr auto coef_pt100 = 0.385;

  //  get logical channel voltage in microvolt and convert to volt
  double volt = afe.logical_channel[logical_channel_num] * 1e-6;

  //  calcurate resistance by deviding excitation current
  double resistance = volt / 250e-6;

  //  convert from resistance to tempoerature (degree Celsius)
  return (resistance - 100) / coef_pt100;
}
