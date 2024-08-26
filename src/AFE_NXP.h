/**	NXP Analog Front End class library for Arduino
 *
 *	Copyright: 2023 - 2024 Tedd OKANO
 *	Released under the MIT license
 *
 *	A simple class library for NXP Analog Front End: NAFE13388 evaluation boards
 */

#ifndef ARDUINO_AFE_DRIVER_H
#define ARDUINO_AFE_DRIVER_H

#include <Arduino.h>
#include <stdint.h>

#include <SPI_for_AFE.h>

/** NAFE13388_Base : A base class for NAFE13388 and NAFE13388_UIM
 *
 *  @class   NAFE13388
 */

class NAFE13388_Base : public SPI_for_AFE
{
public:	
	/** Constructor to create a NAFE13388 instance */
	NAFE13388_Base();

	/** Destractor */
	virtual ~NAFE13388_Base();
	
	/** Begin the device operation
	 *
	 *	NAFE13388 initialization. It does following steps
	 *	(1) Set pins 2 and 3 are input for nINT and nDRDY
	 *	(2) Set pins 5 and 6 are output and fixed to HIGH for ADC_SYN and ADC_nRESET
	 *	(3) Call reset()
	 *	(4) Call boot()
	 */
	virtual void begin( void );

	/** Set system-level config registers */
	virtual void boot( void );

	/** Issue RESET command */
	virtual void reset( void );
	
	/** Board initialization (initializing control pin state)
	 *
	 * @param _pin_nINT pin number for nINT
	 * @param _pin_DRDY pin number for DRDY
	 * @param _pin_SYN pin number for SYN
	 * @param _pin_nRESET pin number for nRESET
	 */
	virtual void board_init( int _pin_nINT, int _pin_DRDY, int _pin_SYN, int _pin_nRESET );

	/** Configure logical channel
	 *
	 * @param ch logical channel number (0 ~ 15)
	 * @param cc0	16bit value to be set CH_CONFIG0 register (0x20)
	 * @param cc1	16bit value to be set CH_CONFIG1 register (0x21)
	 * @param cc2	16bit value to be set CH_CONFIG2 register (0x22)
	 * @param cc3	16bit value to be set CH_CONFIG3 register (0x23)
	 */
	virtual void logical_ch_config( int ch, uint16_t cc0, uint16_t cc1, uint16_t cc2, uint16_t cc3 );

	/** Read ADC
	 *
	 * @param ch logical channel number (0 ~ 15)
	 * @return ADC read value in micro-volt
	 */
	virtual double read( int ch );

	/** Number of enabled logical channels */
	int		enabled_channels;
	
	/** Coefficient to convert from ADC read value to micro-volt */
	double	coeff_uV[ 16 ];
};

/** NAFE13388 : Class for Annapurna application board
 *
 *  @class   NAFE13388
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

class NAFE13388 : public NAFE13388_Base
{
public:	
	/** Constructor to create a NAFE13388 instance */
	NAFE13388();

	/** Destractor */
	virtual ~NAFE13388();

	/** Control pins definitions */
	constexpr static int	pin_nINT	= 2;
	constexpr static int	pin_DRDY	= 3;
	constexpr static int	pin_SYN		= 5;
	constexpr static int	pin_nRESET	= 6;
};

/** NAFE13388 : Class for NAFE13388-UIM board
 *
 *  @class   NAFE13388_UIM
 *
 *  Example:
 *  @code
 *  #include <NAFE13388_UIM.h>
 *  
 *   NAFE13388_UIM afe;
 *  
 *   void setup() {
 *     Serial.begin(9600);
 *     Serial.println("\n***** Hello, NAFE13388! *****");
 *     SPI.begin();  
 *    
 *     afe.begin();
 *  
 *     afe.logical_ch_config(0, 0x1070, 0x00A4, 0x2880, 0x0000);
 *     afe.logical_ch_config(1, 0x2070, 0x00A4, 0x2880, 0x0000);
 *  
 *     Serial.println("logical channel 0 and 1 are shown in micro-volt");
 *   }
 *  
 *   void loop() {
 *     Serial.print(afe.read(0));
 *    Serial.print(",  ");
 *     Serial.println(afe.read(1));
 *   }
 *  @endcode
 */

class NAFE13388_UIM : public NAFE13388_Base
{
public:	
	/** Constructor to create a NAFE13388 instance */
	NAFE13388_UIM();

	/** Destractor */
	virtual ~NAFE13388_UIM();

	/** Control pins definitions */
	constexpr static int	pin_nINT	= 3;
	constexpr static int	pin_DRDY	= 4;
	constexpr static int	pin_SYN		= 6;
	constexpr static int	pin_nRESET	= 7;	
};

#endif //	ARDUINO_AFE_DRIVER_H
