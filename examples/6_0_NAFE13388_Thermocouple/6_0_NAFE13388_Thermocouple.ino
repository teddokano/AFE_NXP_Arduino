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

double get_termocouple_temp(void);
double get_rtd_temp(void);

NAFE13388_UIM afe;

constexpr auto logical_channel_for_thermocouple = 0;
constexpr auto logical_channel_for_on_board_rtd = 1;

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
  //  measureing Thermocouple voltage between AI1P and AI1N pins
  //  PGA gain = x16, coefficient set slot = 7 (for gainsetting x16),
  //  ADC data rate = 2.5Hz, delay for measurement start = 5000us, filter = SINC4, CH_CHOP = on
  afe.logical_channel[logical_channel_for_thermocouple].configure(0x11F0, 0x70C4, 0x8480, 0x0000);

  //  Logical-channel for RTD on GPIO0 and GPIO1
  //
  //  The NAFE13388-UIM board has an RTD on GPIO0 and GPIO1 pins
  //  Using this temperature, the thermocouple maesured value will be compensated
  //
  //  measureing RTD voltage between GPIO0 and GPIO1 pins
  //  ADC data rate = 2.5Hz, delay for measurement start = 5000us, filter = SINC4, CH_CHOP = on
  afe.logical_channel[logical_channel_for_on_board_rtd].configure(0xAAE2, 0x70C4, 0x8480, 0x0000);

  Serial.print("\r\nenabled logical channel(s) = ");
  Serial.println(afe.enabled_logical_channels());
}

void loop() {
  double tc = get_termocouple_temp();
  double rtd = get_rtd_temp();
  double temp = tc + rtd;

  Serial.print(tc, 8);
  Serial.print(", ");
  Serial.print(rtd, 8);
  Serial.print(", ");
  Serial.print(temp, 8);
  Serial.println("");
}

double get_termocouple_temp(void) {
  static constexpr auto thermocouple_coefficient = 41.00;  //  [uV / degree-C]

  return afe.logical_channel[logical_channel_for_thermocouple] / thermocouple_coefficient;
}

double get_rtd_temp(void)
{
  static constexpr auto r_32 = 2940.00;
  static constexpr auto r_34 = 2940.00;
  static constexpr auto r_series = r_32 + r_34;
  static constexpr auto vref_buf = 2.50;
  static constexpr auto coef_pt100 = 0.385;

  //  get logical channel voltage in microvolt and convert to volt
  double v_rtd = afe.logical_channel[logical_channel_for_on_board_rtd] * 1e-6;

  Serial.println(v_rtd, 8);
  //  calcurate resistance by deviding excitation current
  double r_rtd_gpio = (r_series * v_rtd) / (vref_buf - v_rtd);

  //  convert from resistance to tempoerature (degree Celsius)
  return (r_rtd_gpio - 100) / coef_pt100;
}
