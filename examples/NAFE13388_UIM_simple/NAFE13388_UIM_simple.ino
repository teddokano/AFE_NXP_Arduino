#include <NAFE13388_UIM.h>

NAFE13388_UIM afe;

void setup() {
  Serial.begin(9600);

  Serial.println("\n***** Hello, NAFE13388! *****");

  SPI.begin();

  afe.begin();
  afe.blink_leds();

  afe.logical_ch_config(0, 0x1110, 0x00BC, 0x4C80, 0x0000);
  afe.logical_ch_config(1, 0x2210, 0x00BC, 0x4C80, 0x0000);

  Serial.println("\nlogical channel 0 (AI1P-AI1N) and 1 (AI2P-AI2N) voltages are shown in micro-volt");
}

void loop() {
  Serial.print(afe.read<NAFE13388_UIM::microvolt_t>(0));
  Serial.print(",  ");
  Serial.println(afe.read<NAFE13388_UIM::microvolt_t>(1));
}
