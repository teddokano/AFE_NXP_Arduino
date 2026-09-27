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

typedef struct _measurement_channel_pair {
  int channel_A;
  int channel_B;
  double resistance_filter;
} measurement_channel_pair;

constexpr measurement_channel_pair pair[] = {
  { 0, 1, 2395.52 },
  { 2, 3, 2394.66 },
  { 4, 5, 2397.25 },
  { 6, 7, 2394.49 }
};

constexpr double excitation_current = 259e-6;
constexpr int n_of_rtds = 4;

double get_temp_cvd(double resistance);
void calibration(void);

NAFE13388_UIM afe;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("\n***** Hello, NAFE13388! *****");

  SPI.begin();
  pinMode(SS, OUTPUT);  //  Required for UNO R4

  if (!afe.begin()) {
    Serial.println("afe.begin() failed. Check power supply or pin connections");
    while (true)
      ;
  }
  afe.blink_leds();

  for (int rtd_index = 0; rtd_index < n_of_rtds; rtd_index++) {
    afe.logical_channel[pair[rtd_index].channel_A].configure(0x0790 | (rtd_index + 1) << 12, 0x40C4, 0x8400, 0xA600 | (rtd_index + 1));
    afe.logical_channel[pair[rtd_index].channel_B].configure(0x0090 | (rtd_index + 1) << 12 | (rtd_index + 1) << 8, 0x40C4, 0x8400, 0xA600 | (rtd_index + 1));
  }

  Serial.print("\r\nenabled logical channel(s) = ");
  Serial.println(afe.enabled_logical_channels());

  calibration();
}

void loop() {

  for (int rtd_index = 0; rtd_index < n_of_rtds; rtd_index++) {
    double Va = afe.logical_channel[pair[rtd_index].channel_A];
    double Vb = afe.logical_channel[pair[rtd_index].channel_B];

    double Ra = Va / excitation_current;
    double Rb = -Vb / excitation_current;

    double Rrtd = Rb - pair[rtd_index].resistance_filter - Ra;

    double temp = get_temp_cvd(Rrtd);

    Serial.print("rtd[");
    Serial.print(rtd_index);
    Serial.print("]: ");

    Serial.print("  Va = ");
    Serial.print(Va, 8);
    Serial.print("  Vb = ");
    Serial.print(Vb, 8);

    Serial.print("  Ra = ");
    Serial.print(Ra, 8);
    Serial.print("  Rb = ");
    Serial.print(Rb, 8);
    Serial.print("  Rrtd = ");
    Serial.print(Rrtd, 8);
    Serial.print("  temp = ");
    Serial.print(temp, 8);

    Serial.println("");
  }
  Serial.println("");
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

void calibration(void) {
  pinMode(SW3, INPUT);
  if (digitalRead(SW3))
    return;

  Serial.println("SW3 press detected. Entering into calibration mode");

  double Vb[n_of_rtds] = { 0 };
  double Rb[n_of_rtds] = { 0 };
  constexpr int loop_count = 10;

  for (int i = 0; i < loop_count; i++) {
    Serial.print("measureing Rb (");
    Serial.print(i);
    Serial.print("/");
    Serial.print(loop_count);
    Serial.println(")");

    for (int rtd_index = 0; rtd_index < n_of_rtds; rtd_index++) {
      double v = afe.logical_channel[pair[rtd_index].channel_B];
      Vb[rtd_index] += v; 
    }
  }


  for (int rtd_index = 0; rtd_index < n_of_rtds; rtd_index++) {
    Vb[rtd_index] /= loop_count;
    Rb[rtd_index] += -Vb[rtd_index] / excitation_current;

    Serial.print("  Vb[");
    Serial.print(rtd_index);
    Serial.print("] = ");
    Serial.print(Vb[rtd_index], 8);

    Serial.print("  Rb[");
    Serial.print(rtd_index);
    Serial.print("] = ");
    Serial.print(Rb[rtd_index], 8);

    Serial.println("");
  }
  Serial.println("");

  Serial.println("done.");

  while (true)
    ;
}
