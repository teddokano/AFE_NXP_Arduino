#include <NAFE13388_UIM.h>

NAFE13388_UIM afe;

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;

  Serial.println("\n***** Hello, NAFE13388! *****");

  SPI.begin();
  pinMode(SS, OUTPUT);  //  Required for UNO R4
  
  afe.begin();

  afe.logical_ch_config(0, 0x22F0, 0x70AC, 0x5800, 0x0000);
  afe.logical_ch_config(1, 0x33F0, 0x70B1, 0x5800, 0x3860);

  Serial.println("logical channel 0 and 1 are shown in micro-volt");
}

void loop() {
  Serial.print(afe.read(0));
  Serial.print(",  ");
  Serial.println(afe.read(1));
  delay(100);
}
