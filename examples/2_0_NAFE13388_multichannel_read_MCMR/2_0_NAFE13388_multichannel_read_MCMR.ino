/* 
 * NAFE13388-UIM board operation sample for Arduino
 * 
 *  Copyright: 2023 - 2026 Tedd OKANO
 *  Released under the MIT license
 *
 * Sample of using MCMR(Multi Channel Multi Read) command for 8 channels
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

NAFE13388_UIM afe;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("\n***** Hello, NAFE13388! *****");

  SPI.begin();
  pinMode(SS, OUTPUT);  //  Required for UNO R4

  afe.begin();
  afe.blink_leds();

  afe.logical_channel[0].configure(0x1710, 0x00A4, 0xBC00, 0x0000);
  afe.logical_channel[1].configure(0x7110, 0x00A4, 0xBC00, 0x0000);
  afe.logical_channel[2].configure(0x2710, 0x00A4, 0xBC00, 0x0000);
  afe.logical_channel[3].configure(0x7210, 0x00A4, 0xBC00, 0x0000);
  afe.logical_channel[4].configure(0x3710, 0x00A4, 0xBC00, 0x0000);
  afe.logical_channel[5].configure(0x7310, 0x00A4, 0xBC00, 0x0000);
  afe.logical_channel[6].configure(0x4710, 0x00A4, 0xBC00, 0x0000);
  afe.logical_channel[7].configure(0x7410, 0x00A4, 0xBC00, 0x0000);

  Serial.println("\n");
  Serial.println("logical channel 0 = (AI1P-AICOM)");
  Serial.println("logical channel 1 = (AI1N-AICOM)");
  Serial.println("logical channel 2 = (AI2P-AICOM)");
  Serial.println("logical channel 3 = (AI2N-AICOM)");
  Serial.println("logical channel 4 = (AI3P-AICOM)");
  Serial.println("logical channel 5 = (AI3N-AICOM)");
  Serial.println("logical channel 6 = (AI4P-AICOM)");
  Serial.println("logical channel 7 = (AI4N-AICOM)");

  Serial.print("\r\nenabled logical channel(s) = ");
	Serial.println( afe.enabled_logical_channels() );
}

void loop() {
  NAFE13388_UIM::microvolt_t data[8];
  afe.start_and_read(data);

  for (auto i = 0; i < 8; i++) {
    Serial.print(data[i] * 1e-6);
    Serial.print(",  ");
  }

  Serial.println("");
}
