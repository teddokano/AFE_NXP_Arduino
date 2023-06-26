#include <NAFE13388.h>

NAFE13388 afe;

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;

  SPI.begin();
  afe.begin();

  Serial.println("\n***** Hello, NAFE13388! *****");

  afe.logical_ch_config(0, 0x1150, 0x00AC, 0x1400, 0x0000);
  afe.logical_ch_config(1, 0x3350, 0x00A4, 0x1400, 0x3060);

  Serial.println("logical channel 0 and 1 are shown in micro-volt");
}

void loop() {
  Serial.print(afe.read(0));
  Serial.print(",  ");
  Serial.println(afe.read(1));
  delay(100);
}
