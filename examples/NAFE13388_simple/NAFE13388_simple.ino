/* 
 * NAFE13388-UIM board operation sample for Arduino
 * 
 *  Copyright: 2023 - 2026 Tedd OKANO
 *  Released under the MIT license
 */

#include <NAFE13388_UIM.h>

NAFE13388_UIM afe;

void setup() {
  Serial.begin(115200);
  Serial.println("\n***** Hello, NAFE13388! *****");

  SPI.begin();

  afe.begin();
  afe.blink_leds();

  afe.logical_channel[0].configure(0x1710, 0x00A4, 0xBC00, 0x0000);
  afe.logical_channel[1].configure(0x2710, 0x00A4, 0xBC00, 0x0000);

  Serial.println("\nlogical channel 0 (AI1P-GND) and 1 (AI2P-GND) voltages are shown in ADC readout value [V]");
}

void loop() {
  Serial.print((NAFE13388_UIM::microvolt_t)afe.logical_channel[0] * 1e-6);
  Serial.print(",  ");
  Serial.println((NAFE13388_UIM::microvolt_t)afe.logical_channel[1] * 1e-6);
}
