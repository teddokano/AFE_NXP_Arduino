#include <NAFE13388.h>

NAFE13388 afe;

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;

  Serial.println("\n***** Hello, NAFE13388! *****");

  SPI.begin();
  pinMode(SS, OUTPUT);  //  Required for UNO R4
  
  afe.begin();

  afe.logical_ch_config(0, 0x1110, 0x00BC, 0x4C80, 0x0000);
  afe.logical_ch_config(1, 0x2210, 0x00BC, 0x4C80, 0x0000);

  Serial.println("\nlogical channel 0 (AI1P-AI1N) and 1 (AI2P-AI2N) voltages are shown in micro-volt");
}

void loop() {
  Serial.print(afe.read(0));
  Serial.print(",  ");
  Serial.println(afe.read(1));
  delay(100);
}
