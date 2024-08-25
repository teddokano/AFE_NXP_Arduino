/** NXP Analog Front End class library for Arduino
 *
 *  @class   NAFE13388
 *  @author  Tedd OKANO
 *
 *  Copyright: 2023 - 2024 Tedd OKANO
 *  Released under the MIT license
 *
 *  A simple class library for NXP Analog Front End: NAFE13388
 *
 *  Example:
 *  @code
 *  #include <NAFE13388.h>
 *  
 *   NAFE13388 afe;
 *  
 *   void setup() {
 *     Serial.begin(9600);
 *     while (!Serial)
 *  	 ;
 *  
 *     SPI.begin();
 *     pinMode(SS, OUTPUT);  //  Required for UNO R4
 *     
 *     afe.begin();
 *  
 *     Serial.println("\n***** Hello, NAFE13388! *****");
 *  
 *     afe.logical_ch_config(0, 0x22F0, 0x70AC, 0x5800, 0x0000);
 *     afe.logical_ch_config(1, 0x33F0, 0x70B1, 0x5800, 0x3860);
 *  
 *     Serial.println("logical channel 0 and 1 are shown in micro-volt");
 *   }
 *  
 *   void loop() {
 *     Serial.print(afe.read(0));
 *     Serial.print(",  ");
 *     Serial.println(afe.read(1));
 *     delay(100);
 *   }
 *  @endcode
 */

#ifndef ARDUINO_AFE_DRIVER_H
#define ARDUINO_AFE_DRIVER_H

#define	TARGET_UIM

#include <Arduino.h>
#include <stdint.h>

#include <SPI_for_AFE.h>

class NAFE13388 : public SPI_for_AFE
{
public:	
	/** Constructor to create a NAFE13388 instance */
	NAFE13388();

	/** Destractor */
	virtual ~NAFE13388();
	
	/** Begin the device operation
	 *
	 *	NAFE13388 initialization. It does following steps
	 *	(1) Set pins 2 and 3 are input for nINT and nDRDY
	 *	(2) Set pins 5 and 6 are output and fixed to HIGH for ADC_SYN and ADC_nRESET
	 *	(3) Call reset()
	 *	(4) Call boot()
	 */
	void begin( void );

	/** Set system-level config registers */
	void boot( void );

	/** Issue RESET command */
	void reset();
	
	/** Configure logical channel
	 *
	 * @param ch logical channel number (0 ~ 15)
	 * @param cc0	16bit value to be set CH_CONFIG0 register (0x20)
	 * @param cc1	16bit value to be set CH_CONFIG1 register (0x21)
	 * @param cc2	16bit value to be set CH_CONFIG2 register (0x22)
	 * @param cc3	16bit value to be set CH_CONFIG3 register (0x23)
	 */
	void logical_ch_config( int ch, uint16_t cc0, uint16_t cc1, uint16_t cc2, uint16_t cc3 );

	/** Read ADC
	 *
	 * @param ch logical channel number (0 ~ 15)
	 * @return ADC read value in micro-volt
	 */
	double read( int ch );

	/** Number of enabled logical channels */
	int		enabled_channels;
	
	/** Coefficient to convert from ADC read value to micro-volt */
	double	coeff_uV[ 16 ];

	/** Control pins definitions */
#ifdef TARGET_UIM
	constexpr static int	pin_nINT	= 3;
	constexpr static int	pin_DRDY	= 4;
	constexpr static int	pin_SYN		= 6;
	constexpr static int	pin_nRESET	= 7;
#else
	constexpr static int	pin_nINT	= 2;
	constexpr static int	pin_DRDY	= 3;
	constexpr static int	pin_SYN		= 5;
	constexpr static int	pin_nRESET	= 6;
#endif
};

#endif //	ARDUINO_AFE_DRIVER_H
