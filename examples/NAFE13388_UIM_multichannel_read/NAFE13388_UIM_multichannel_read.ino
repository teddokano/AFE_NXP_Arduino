#include <NAFE13388_UIM.h>

NAFE13388_UIM afe;

void setup() {
  Serial.begin(9600);

  Serial.println("\n***** Hello, NAFE13388! *****");

  SPI.begin();

  afe.begin();
  afe.blink_leds();

  afe.open_logical_channel(0, 0x1710, 0x009C, 0x4C00, 0x0000);
  afe.open_logical_channel(1, 0x7110, 0x009C, 0x4C00, 0x0000);
  afe.open_logical_channel(2, 0x2710, 0x009C, 0x4C00, 0x0000);
  afe.open_logical_channel(3, 0x7210, 0x009C, 0x4C00, 0x0000);
  afe.open_logical_channel(4, 0x3710, 0x009C, 0x4C00, 0x0000);
  afe.open_logical_channel(5, 0x7310, 0x009C, 0x4C00, 0x0000);
  afe.open_logical_channel(6, 0x4710, 0x009C, 0x4C00, 0x0000);
  afe.open_logical_channel(7, 0x7410, 0x009C, 0x4C00, 0x0000);

  Serial.println("\n");
  Serial.println("logical channel 0 = (AI1P-AICOM)");
  Serial.println("logical channel 1 = (AI1N-AICOM)");
  Serial.println("logical channel 2 = (AI2P-AICOM)");
  Serial.println("logical channel 3 = (AI2N-AICOM)");
  Serial.println("logical channel 4 = (AI3P-AICOM)");
  Serial.println("logical channel 5 = (AI3N-AICOM)");
  Serial.println("logical channel 6 = (AI4P-AICOM)");
  Serial.println("logical channel 7 = (AI4N-AICOM)");
}

void loop() {
  long data[ 8 ];
  afe.start_and_read( data );

  for ( auto i = 0; i < 8; i++ )
  {
    Serial.print(data[ i ]);
    Serial.print(",  ");
  }

  Serial.println("");
}
