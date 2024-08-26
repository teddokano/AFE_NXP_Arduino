#include <NAFE13388_UIM.h>

NAFE13388_UIM afe;

void setup() {
  Serial.begin(9600);
  Serial.println("\n***** Hello, NAFE13388! *****");
  SPI.begin();  
  
  afe.begin();

  afe.logical_ch_config(0, 0x1070, 0x00A4, 0x2880, 0x0000);
  afe.logical_ch_config(1, 0x2070, 0x00A4, 0x2880, 0x0000);

  Serial.println("logical channel 0 and 1 are shown in micro-volt");
}

void loop() {
  Serial.print(afe.read(0));
  Serial.print(",  ");
  Serial.println(afe.read(1));
}
