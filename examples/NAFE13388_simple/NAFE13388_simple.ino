#include <NAFE13388.h>

NAFE13388 afe;

void setup() {
  Serial.begin(9600);

  Serial.println("\n***** Hello, NAFE13388! *****");

  SPI.begin();

  afe.begin();
  afe.blink_leds();

  afe.open_logical_channel(0, 0x1110, 0x00BC, 0x4C80, 0x0000);
  afe.open_logical_channel(1, 0x2210, 0x00BC, 0x4C80, 0x0000);

  Serial.println("\nlogical channel 0 (AI1P-AI1N) and 1 (AI2P-AI2N) voltages are shown in ADC readout raw value");
}

void loop() {
  Serial.print(afe.start_and_read(0));
  Serial.print(",  ");
  Serial.println(afe.start_and_read(1));
}
