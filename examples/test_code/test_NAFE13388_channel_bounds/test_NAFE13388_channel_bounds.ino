/*
 * T-07 verification: out-of-range logical channel numbers must not
 * crash or corrupt memory -- they should be rejected with a warning
 * (visible here because build_opt.h defines AFE_NXP_DEBUG) and a
 * harmless return value (0 / NAN).
 *
 * Run this with the NAFE13388-UIM connected. If the board survives
 * every call below and prints "still alive" after each one, the
 * bounds check in open_logical_channel() / enable_logical_channel() /
 * close_logical_channel() / start(int) / read(int) / raw2v() is
 * working as intended.
 *
 *  Copyright: 2023 - 2026 Tedd OKANO
 *  Released under the MIT license
 */

#include <NAFE13388_UIM.h>

NAFE13388_UIM afe;

void alive(const char *label) {
  Serial.print("  -> still alive after ");
  Serial.println(label);
}

void probe_channel(int ch) {
  Serial.print("\n--- probing ch = ");
  Serial.println(ch);

  afe.open_logical_channel(ch, 0x1710, 0x00A4, 0xBC00, 0x0000);
  alive("open_logical_channel()");

  afe.enable_logical_channel(ch);
  alive("enable_logical_channel()");

  afe.start(ch);
  alive("start(ch)");

  NAFE13388_UIM::raw_t raw = afe.read(ch);
  Serial.print("  read(ch) returned ");
  Serial.println(raw);
  alive("read(ch)");

  double v = afe.raw2v(ch, 12345);
  Serial.print("  raw2v(ch, 12345) returned ");
  Serial.println(v);
  alive("raw2v(ch, value)");

  afe.close_logical_channel(ch);
  alive("close_logical_channel()");
}

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("\n***** T-07 channel bounds-check test *****");

  SPI.begin();
  pinMode(SS, OUTPUT);  //  Required for UNO R4

  afe.begin();
  afe.blink_leds();

  Serial.println("\n=== control: ch = 0 (valid) ===");
  probe_channel(0);

  Serial.println("\n=== ch = -1 (out of range, low) ===");
  probe_channel(-1);

  Serial.println("\n=== ch = 16 (out of range, high) ===");
  probe_channel(16);

  Serial.println("\n=== ch = 0 again: confirm still usable after the invalid calls ===");
  afe.logical_channel[0].configure(0x1710, 0x00A4, 0xBC00, 0x0000);
  Serial.print("logical_channel[0] reads: ");
  Serial.println((NAFE13388_UIM::volt_t)afe.logical_channel[0]);

  Serial.println("\n***** test complete: no crash means the bounds check held *****");
}

void loop() {
}
