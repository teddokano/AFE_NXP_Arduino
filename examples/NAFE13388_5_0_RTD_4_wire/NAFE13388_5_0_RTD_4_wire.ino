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
#include <math.h>

double get_temp(int logical_channel_num);
double get_temp_cvd(int logical_channel_num);

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

  //  Logical-channel for RTD measurement by 4 wire method
  //
  //  measureing RTD voltage between AI2P and AI2N pins
  //  using 250uA excitation current from AI1P pin
  //  PGA gain = x16, coefficient set slot = 8,
  //  ADC data rate = 2.5Hz, delay for measurement start = 5000us, filter = SINC4, CH_CHOP = on
  afe.logical_channel[0].configure(0x22F8, 0x80C4, 0x8480, 0xA608);

  //  Logical-channel for RTD measurement by 4 wire method
  //
  //  measureing RTD voltage between AI4P and AI4N pins
  //  using 250uA excitation current from AI3P pin
  //  PGA gain = x16, coefficient set slot = 8,
  //  ADC data rate = 2.5Hz, delay for measurement start = 5000us, filter = SINC4, CH_CHOP = on
  afe.logical_channel[1].configure(0x44F8, 0x80C4, 0x8480, 0xA618);

  //  Set factory calibrated coefficients for PGA gain=x16, with excitation current = 250uA
  //  using coefficient slot on 8
  //  reference : AN14539, Table 3
  //
  //  With these factory-calibrated coefficients,
  //  the resistance can be get by simple calcuration: "R_rtd = V_measured / 250uA"
  //
  afe.reg(NAFE13388_UIM::Register24::OFFSET_COEFF8, afe.reg(NAFE13388_UIM::Register24::OPT_COEF3));
  afe.reg(NAFE13388_UIM::Register24::GAIN_COEFF8, afe.reg(NAFE13388_UIM::Register24::OPT_COEF4));

  Serial.print("\r\nenabled logical channel(s) = ");
  Serial.println(afe.enabled_logical_channels());
}

void loop() {
  double temp0 = get_temp(0);
  double temp1 = get_temp(1);
  double temp0_cvd = get_temp_cvd(0);
  double temp1_cvd = get_temp_cvd(1);

  Serial.print(temp0, 8);
  Serial.print(", ");
  Serial.print(temp1, 8);
  Serial.print(",  (CVD) ");
  Serial.print(temp0_cvd, 8);
  Serial.print(", ");
  Serial.print(temp1_cvd, 8);
  Serial.println("");
}

double get_temp(int logical_channel_num) {
  static constexpr auto coef_pt100 = 0.385;

  //  get logical channel voltage in microvolt and convert to volt
  double volt = afe.logical_channel[logical_channel_num];

  //  calcurate resistance by deviding excitation current
  double resistance = volt / 250e-6;

  //  convert from resistance to tempoerature (degree Celsius)
  return (resistance - 100) / coef_pt100;
}

//  Convert Pt100 resistance to temperature using the Callendar-Van Dusen (CVD)
//  equation (IEC 60751), instead of the linear approximation used in get_temp().
//
//  For T >= 0 degC:
//    R(T) = R0 * (1 + A*T + B*T^2)
//    -> solved directly for T (quadratic formula)
//
//  For T < 0 degC:
//    R(T) = R0 * (1 + A*T + B*T^2 + C*(T - 100)*T^3)
//    -> no closed-form inverse, solved numerically by Newton-Raphson,
//       using the linear approximation as the initial guess
double resistance_to_temp_cvd(double resistance) {
  static constexpr auto R0 = 100.0;
  static constexpr auto A = 3.9083e-3;
  static constexpr auto B = -5.775e-7;
  static constexpr auto C = -4.183e-12;

  if (resistance >= R0) {
    double ratio = resistance / R0;
    return (-A + sqrt(A * A - 4.0 * B * (1.0 - ratio))) / (2.0 * B);
  }

  //  initial guess from linear approximation
  double t = (resistance - R0) / (R0 * 0.00385);

  for (int i = 0; i < 5; i++) {
    double f = R0 * (1.0 + A * t + B * t * t + C * (t - 100.0) * t * t * t) - resistance;
    double df = R0 * (A + 2.0 * B * t + C * (4.0 * t * t * t - 300.0 * t * t));
    t -= f / df;
  }

  return t;
}

double get_temp_cvd(int logical_channel_num) {
  //  get logical channel voltage in microvolt and convert to volt
  double volt = afe.logical_channel[logical_channel_num];

  //  calcurate resistance by deviding excitation current
  double resistance = volt / 250e-6;

  return resistance_to_temp_cvd(resistance);
}
