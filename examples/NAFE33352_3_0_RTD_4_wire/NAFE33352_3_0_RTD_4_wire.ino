/* 
 * The NAFE33352-UIOM board operation sample for Arduino
 * 
 *  Copyright: 2026 Tedd OKANO
 *  Released under the MIT license
 *
 *	Sample of using both ADC and DAC (input and output)
 */

/*
  * ** CAUTION ** CAUTION ** CAUTION ** CAUTION ** CAUTION ** CAUTION ** CAUTION **
  * ** CAUTION ** CAUTION ** CAUTION ** CAUTION ** CAUTION ** CAUTION ** CAUTION **
  * ** CAUTION ** CAUTION ** CAUTION ** CAUTION ** CAUTION ** CAUTION ** CAUTION **
  * 
  * The NAFE33352-UIOM board and Arduino mocrocontroller cannot be connected 
  * directly on Arduino-shield socket. The 3.3V supply should be disconnected. 
  * Visit next URL page to confirm how to do it. 
  *   -->  https://github.com/teddokano/AFE_NXP_Arduino/blob/main/README.md
  */

#include <NAFE33352_UIOM.h>

NAFE33352_UIOM shasta;

double get_temp_cvd(double resistance);

//#define VOLTAGE_OUTPUT_SETTING
double output_value;
int count = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("\n***** Hello, NAFE33352_UIOM! *****");

  SPI.begin();
  pinMode(SS, OUTPUT);  //  Required for UNO R4

  if (!shasta.begin()) {
    Serial.println("shasta.begin() failed. Check power supply or pin connections");
    while (true)
      ;
  }

  output_value = 250 * 1e-6;  //	250uA
  shasta.dac.configure(NAFE33352_UIOM::DAC::ModeSelect::CURRENT);

  shasta.dac = output_value;
  delay(100);

  shasta.logical_channel[0].configure(0x0308, 0x70B4, 0x2C00);
  shasta.logical_channel[1].configure(0x0080, 0x5064, 0x5000);
  shasta.logical_channel[2].configure(0x0088, 0x5064, 0x5000);
  shasta.logical_channel[3].configure(0x0038, 0x2064, 0x5000);
  shasta.logical_channel[4].configure(0x0030, 0x3064, 0x5000);

//  shasta.reg(NAFE33352_UIOM::Register24::OFFSET_COEF7, shasta.reg(NAFE33352_UIOM::Register24::EXTRA_CAL_COEF6));
//  shasta.reg(NAFE33352_UIOM::Register24::GAIN_COEF7, shasta.reg(NAFE33352_UIOM::Register24::EXTRA_CAL_COEF7));

  Serial.print("GAIN_COEF7 = ");
  Serial.println(shasta.reg(NAFE33352_UIOM::Register24::GAIN_COEF7), HEX);

  Serial.print("OFFSET_COEF7 = ");
  Serial.println(shasta.reg(NAFE33352_UIOM::Register24::OFFSET_COEF7), HEX);

  Serial.print("EXTRA_CAL_COEF0 = ");
  Serial.println(shasta.reg(NAFE33352_UIOM::Register24::EXTRA_CAL_COEF0), HEX);
  Serial.print("EXTRA_CAL_COEF1 = ");
  Serial.println(shasta.reg(NAFE33352_UIOM::Register24::EXTRA_CAL_COEF1), HEX);
  Serial.print("EXTRA_CAL_COEF2 = ");
  Serial.println(shasta.reg(NAFE33352_UIOM::Register24::EXTRA_CAL_COEF2), HEX);
  Serial.print("EXTRA_CAL_COEF3 = ");
  Serial.println(shasta.reg(NAFE33352_UIOM::Register24::EXTRA_CAL_COEF3), HEX);
  Serial.print("EXTRA_CAL_COEF4 = ");
  Serial.println(shasta.reg(NAFE33352_UIOM::Register24::EXTRA_CAL_COEF4), HEX);
  Serial.print("EXTRA_CAL_COEF5 = ");
  Serial.println(shasta.reg(NAFE33352_UIOM::Register24::EXTRA_CAL_COEF5), HEX);
  Serial.print("EXTRA_CAL_COEF6 = ");
  Serial.println(shasta.reg(NAFE33352_UIOM::Register24::EXTRA_CAL_COEF6), HEX);
  Serial.print("EXTRA_CAL_COEF7 = ");
  Serial.println(shasta.reg(NAFE33352_UIOM::Register24::EXTRA_CAL_COEF7), HEX);

  Serial.println("AIP(SE)[V], VHDD[V], VHSS[V], VSNS[V], ISNS[A], AIO_STATUS, Temp[deg-C]");
}

void loop() {
  double data[shasta.enabled_logical_channels()];
  for (auto i = 0; i < shasta.enabled_logical_channels(); i++) {
    data[i] = shasta.logical_channel[i];
  }

#if 0
  for (auto i = 0; i < shasta.enabled_logical_channels(); i++) {
    Serial.print(data[i], 9);
    Serial.print(",    ");
  }
#endif

  double Rrtd = data[0] / data[4];
//  double Rrtd = data[0] / 250e-6;
  double Trtd = get_temp_cvd(Rrtd);

  Serial.print("AI1P-AI1N = ");
  Serial.print(data[0], 8);

  Serial.print(",   I_exc = ");
  Serial.print(data[4], 8);

  Serial.print(",   Rrtd = ");
  Serial.print(Rrtd, 8);


  Serial.print(",   Temp_rtd = ");
  Serial.print(Trtd, 8);




  Serial.print(shasta.reg(NAFE33352_UIOM::Register16::AIO_STATUS), HEX);
  Serial.print(",    ");
  Serial.println(shasta.temperature(), 1);

  delay(1000);
}

double get_temp_cvd(double resistance) {
  static constexpr double R0 = 100.0;
  static constexpr double A = 3.9083e-3;
  static constexpr double B = -5.775e-7;
  static constexpr double C = -4.183e-12;

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
