/* 
 * The NAFE33352-UIOM board operation sample for Arduino
 * 
 *  Copyright: 2026 Tedd OKANO
 *  Released under the MIT license
 *
 *	Sample of using both ADC and DAC (input and oupput)
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

#define VOLTAGE_OUTPUT_SETTING
double output_value;
int count = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("\n***** Hello, NAFE33352_UIOM! *****");

  SPI.begin();
  pinMode(SS, OUTPUT);  //  Required for UNO R4

  Serial.println(shasta.revision_number(), HEX);


  shasta.begin();

#ifdef VOLTAGE_OUTPUT_SETTING
  output_value = 5.00;  //	5V
  shasta.dac.configure(NAFE33352_UIOM::DAC::ModeSelect::VOLTAGE);
#else
  output_value = 20 * 1e-3;  //	20mA
  shasta.dac.configure(NAFE33352_UIOM::DAC::ModeSelect::CURRENT);
#endif

  shasta.dac = output_value;
  delay( 100 );

  shasta.logical_channel[0].configure(0x0020, 0x50B4, 0x5000);
  shasta.logical_channel[1].configure(0x0080, 0x5064, 0x5000);
  shasta.logical_channel[2].configure(0x0088, 0x5064, 0x5000);
  shasta.logical_channel[3].configure(0x0038, 0x2064, 0x5000);
  shasta.logical_channel[4].configure(0x0030, 0x3064, 0x5000);
}

void loop() {
  double data;
  for (auto i = 0; i < shasta.enabled_logical_channels(); i++) {
    data = shasta.logical_channel[i];
    Serial.print(data, 9);
    Serial.print(",    ");
  }

  Serial.println(shasta.reg(NAFE33352_UIOM::Register16::AIO_STATUS), HEX);

  shasta.dac = output_value * (count++ & 0x1 ? +1.00 : -1.00);

  delay(1000);
}
