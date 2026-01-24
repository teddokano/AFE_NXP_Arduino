/* 
 * NAFE13388-UIM board operation sample for Arduino
 * 
 *  Copyright: 2023 - 2026 Tedd OKANO
 *  Released under the MIT license
 *
 * Sample of using MCMR(Multi Channel Continuous Read) command for 8 channels
 *
 * *****************************
 * ** BEFORE TRYING THIS CODE **
 * *****************************
 * This sample code shows operation with **DRDY** signal (at D4 pin) from AFE 
 * Since the DRDY signal is very short pulse, it will be captured by interrupt on D2 pin
 * Short D4 and D2 pin to handle it
 */

#include <NAFE13388_UIM.h>

NAFE13388_UIM afe;

constexpr auto normal_lc = 8;
constexpr auto monitor_lc = normal_lc + 6;
volatile bool conversion_done = false;

void drdy_callback(void) {
  conversion_done = true;
}

void setup() {
  Serial.begin(115200);

  Serial.println("\n***** Hello, NAFE13388! *****");

  SPI.begin();

  afe.begin();
  afe.blink_leds();

  constexpr uint16_t cc0 = 0x0010;
  constexpr uint16_t cc1 = 0x007C;
  constexpr uint16_t cc2 = 0x4C00;
  constexpr uint16_t cc3 = 0x0000;

  for (auto i = 0; i < normal_lc / 2; i++) {
    afe.open_logical_channel(i * 2 + 0, cc0 | (i + 1) << 12 | 7 << 8, cc1, cc2, cc3);
    afe.open_logical_channel(i * 2 + 1, cc0 | 7 << 12 | (i + 1) << 8, cc1, cc2, cc3);
  }

  afe.open_logical_channel(normal_lc + 0, 0x9900, 0x007C, 0x4C00, 0x0000);
  afe.open_logical_channel(normal_lc + 1, 0xAA02, 0x007C, 0x4C00, 0x0000);
  afe.open_logical_channel(normal_lc + 2, 0xBB04, 0x007C, 0x4C00, 0x0000);
  afe.open_logical_channel(normal_lc + 3, 0xCC06, 0x007C, 0x4C00, 0x0000);
  afe.open_logical_channel(normal_lc + 4, 0xDD08, 0x007C, 0x4C00, 0x0000);
  afe.open_logical_channel(normal_lc + 5, 0xEE0A, 0x007C, 0x4C00, 0x0000);

  Serial.print("\r\nenabled logical channel(s) = ");
  Serial.println(afe.enabled_logical_channels());

  afe.set_DRDY_callback(drdy_callback);  //	set callback function for when DRDY detected
  afe.DRDY_by_sequencer_done(true);      //	generate DRDY at all logical channel conversions are done

  afe.start_continuous_conversion();  //	measurement start as MCCR (Multi-Channel Continuous-Reading)
}

void loop() {
  NAFE13388_UIM::microvolt_t data[monitor_lc];

  if (conversion_done) {
    conversion_done = false;

    afe.read(data);  //	read data from all enabled channels

    for (auto i = 0; i < monitor_lc; i++) {
      Serial.print(data[i] * 1e-6);
      Serial.print(",  ");
    }

    Serial.println("");
  }
}
