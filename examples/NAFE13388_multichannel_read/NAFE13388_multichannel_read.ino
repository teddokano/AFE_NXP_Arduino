/* 
 * NAFE13388-UIM board operation sample for Arduino
 * 
 *  Copyright: 2023 - 2026 Tedd OKANO
 *  Released under the MIT license
 *
 * This sample code shows operation with **DRDY** signal (at D4 pin) from AFE 
 * Since the DRDY signal is very short pulse, it will be captured by interrupt on D2 pin
 * Short D4 and D2 pin to handle it
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

  Serial.println("\r\nenabled logical channel(s) ");

	Serial.println( afe.enabled_logical_channels() );
//	logical_ch_config_view();


	afe.use_DRDY_trigger( true );		//	default = true
	afe.DRDY_by_sequencer_done( true );	//	generate DRDY at all logical channel conversions are done
}

void loop() {
  NAFE13388_UIM::microvolt_t data[8];
  afe.start_and_read(data);

  for (auto i = 0; i < 8; i++) {
    Serial.print(data[i]);
    Serial.print(",  ");
  }

  Serial.println("");
}
