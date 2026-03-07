/* 
 * NAFE13388-UIM board operation sample for Arduino
 * 
 *  Copyright: 2023 - 2026 Tedd OKANO
 *  Released under the MIT license
 *
 * Sample of using MCMR(Multi Channel Continuous Read) command for 8 channels
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

void table_view(NAFE13388::Register24 reg_addr, int length);
void zprintf(const char *format, ...);

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("\n***** Hello, NAFE13388! *****");

  SPI.begin();
  pinMode(SS, OUTPUT);  //  Required for UNO R4

  afe.begin();
  afe.blink_leds();

  afe.open_logical_channel(0, 0x1110, 0x00BC, 0x4C80, 0x0000);
  afe.open_logical_channel(1, 0x2210, 0x00BC, 0x4C80, 0x0000);

  Serial.println("Registre dump: GAIN_COEFF0~15, OFFSET_COEF0~15");
  table_view(NAFE13388::Register24::GAIN_COEFF0, 32);

  Serial.println("\nRe-calibration (for PGA_gain=0.2~16) in progress");
  for (auto i = 0; i < 8; i++) {
    Serial.print("  ..");
    Serial.print(i+1);
    Serial.println("/8");
    afe.self_calibrate(i);
  }

  Serial.println("\nRegistre dump (after re-calibration): GAIN_COEFF0~15, OFFSET_COEF0~15");
  table_view(NAFE13388::Register24::GAIN_COEFF0, 32);

  Serial.println("\nlogical channel 0 (AI1P-AI1N) and 1 (AI2P-AI2N) voltages are shown in micro-volt");
}

void loop() {
  Serial.print(afe.raw2uv(0,afe.start_and_read(0)));
  Serial.print(",  ");
  Serial.println(afe.raw2uv(1,afe.start_and_read(1)));
}

#define COLS 4

void table_view(NAFE13388::Register24 reg_addr, int length) {
  auto ROWS = length / COLS;
  for (int y = 0; y < ROWS; y++) {
    for (int x = 0; x < COLS; x++) {
      int n = x * ROWS + y;

      zprintf("  %8ld @ 0x%04X,", afe.reg(reg_addr + n), reg_addr + n);
    }
    zprintf("\n");
  }
}

#define MAX_STR_LENGTH 80
void zprintf(const char *format, ...) {
  char s[MAX_STR_LENGTH];
  va_list args;

  va_start(args, format);
  vsnprintf(s, MAX_STR_LENGTH, format, args);
  va_end(args);

  Serial.print(s);
}
