/* 
 * The NAFE33352-UIOM board operation sample for Arduino
 * 
 *  Copyright: 2026 Tedd OKANO
 *  Released under the MIT license
 *
 *	Sample of using both AIO channel as input
 */

/*
  * ** CAUTION ** CAUTION ** CAUTION ** CAUTION ** CAUTION ** CAUTION ** CAUTION **
  * ** CAUTION ** CAUTION ** CAUTION ** CAUTION ** CAUTION ** CAUTION ** CAUTION **
  * ** CAUTION ** CAUTION ** CAUTION ** CAUTION ** CAUTION ** CAUTION ** CAUTION **
  * 
  * The NAFE33352-UIOM board and Arduino mocrocontroller cannot be connected 
  * directly on Arduino-shield socket. The 3.3V supply should be disconnected. 
  * Visit next URL page to confirm how to do it. 
  *   -->  https://github.com/teddokano/AFE_NXP_Arduino/blob/main/README.md
  */

#include <NAFE33352_UIOM.h>

NAFE33352_UIOM shasta;

#define AIO_VOLTAGE_INPUT

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("\n***** Hello, NAFE33352_UIOM! *****");

  SPI.begin();
  pinMode(SS, OUTPUT);  //  Required for UNO R4

  shasta.begin();

#ifdef AIO_VOLTAGE_INPUT
  shasta.dac.configure(NAFE33352_UIOM::DAC::ModeSelect::DAC_OFF_VOLTAGE_IN);
#else
  shasta.dac.configure(NAFE33352_UIOM::DAC::ModeSelect::DAC_OFF_CURRENT_IN);
#endif

  shasta.logical_channel[0].configure(0x0038, 0x2064, 0x5000);
  shasta.logical_channel[1].configure(0x0030, 0x3064, 0x5000);
  shasta.logical_channel[2].configure(0x0080, 0x5064, 0x5000);
  shasta.logical_channel[3].configure(0x0088, 0x5064, 0x5000);
}

void loop() {
  double data;
  for (auto i = 0; i < shasta.enabled_logical_channels(); i++) {
    data = shasta.logical_channel[i];
    Serial.print(data, 9);
    Serial.print(",    ");
  }
  Serial.println(shasta.reg(NAFE33352_UIOM::Register16::AIO_STATUS), HEX);

  delay(1000);
}
