/** LCDDriver operation library for Arduino
 *
 *  @author Tedd OKANO
 *
 *  Released under the MIT license License
 */

#ifndef ARDUINO_LCD_DRIVER_H
#define ARDUINO_LCD_DRIVER_H

#include <Arduino.h>
#include <stdint.h>

#include <SPI.h>


class SPI_for_AFE
{
public:
	/** Send data
	 * 
	 * @param data pointer to data buffer
	 * @param size data size
	 * @param stop option: generating STOP-condition after transaction. Defailt: true
	 * @return transferred data size
	 */
	void txrx( uint8_t *data, int size );

	/** Register write, 8 bit
	 *
	 * @param reg register index/address/pointer
	 */
	void write_r16( uint16_t reg );

	/** Register write, 16 bit
	 *
	 * @param reg register index/address/pointer
	 * @param val data value
	 */
	void write_r16( uint16_t reg, uint16_t val );

	/** Register read, 16 bit
	 *
	 * @param reg register index/address/pointer
	 * @return data value
	 */
	uint16_t read_r16( uint16_t reg );

	/** Register write, 24 bit
	 *
	 * @param reg register index/address/pointer
	 * @param val data value
	 */
	void write_r24( uint16_t reg, uint32_t val );

	/** Register read, 24 bit
	 *
	 * @param reg register index/address/pointer
	 * @return data value
	 */
	int32_t read_r24( uint16_t reg );

};



class NAFE13388 : public SPI_for_AFE
{
public:	
	/** Create a NAFE13388 instance */
	NAFE13388();

	virtual ~NAFE13388();
	
	/** Begin the device operation
	 *
	 *	NAFE13388 initialization
	 */
	void begin( void );

	void boot( void );
	void reset();
	void logical_ch_config( int ch, uint16_t cc0, uint16_t cc1, uint16_t cc2, uint16_t cc3, double coeff = ((10.0 / (double)(1L << 24)) / 0.8) * 1e6 );
	double read( int ch );

	int		enabled_channels;
	double	coeff_uV[ 16 ];
};

#endif //	ARDUINO_LCD_DRIVER_H
