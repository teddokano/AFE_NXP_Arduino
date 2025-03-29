#include "AFE_NXP.h"

void SPI_for_AFE::txrx( uint8_t *data, int size )
{
//	data[ 0 ]	|= dev_ad ? 0x80 : 0x00;

	SPI.beginTransaction( SPISettings( 1000000, MSBFIRST, SPI_MODE1 ) );
	digitalWrite( SS, LOW );
	SPI.transfer( data, size );
	digitalWrite( SS, HIGH );
}

void SPI_for_AFE::write_r16( uint16_t reg )
{
	reg	<<= 1;

	uint8_t	v[]	= { (uint8_t)(reg >> 8), (uint8_t)(reg & 0xFF) };
	txrx( v, sizeof( v ) );
}

void SPI_for_AFE::write_r16( uint16_t reg, uint16_t val )
{
	reg	<<= 1;

	uint8_t	v[]	= { (uint8_t)(reg >> 8), (uint8_t)(reg & 0xFF), (uint8_t)(val >> 8), (uint8_t)val };
	txrx( v, sizeof( v ) );
}

uint16_t SPI_for_AFE::read_r16( uint16_t reg )
{
	reg	<<= 1;
	reg	 |= 0x4000;

	uint8_t	v[ 4 ]	= { (uint8_t)(reg >> 8), (uint8_t)(reg & 0xFF), 0xFF, 0xFF };
	txrx( v, sizeof( v ) );
	
	return get_data16( v + command_length );
}

void SPI_for_AFE::write_r24( uint16_t reg, uint32_t val )
{
	reg	<<= 1;

	uint8_t	v[]	= { (uint8_t)(reg >> 8), (uint8_t)(reg & 0xFF), (uint8_t)(val >> 16), (uint8_t)(val >> 8), (uint8_t)val };
	txrx( v, sizeof( v ) );
}

int32_t SPI_for_AFE::read_r24( uint16_t reg )
{
	reg	<<= 1;
	reg	 |= 0x4000;

	uint8_t	v[]	= { (uint8_t)(reg >> 8), (uint8_t)(reg & 0xFF), 0xFF, 0xFF, 0xFF };
	txrx( v, sizeof( v ) );
	
	return get_data24( v + command_length );
}

void SPI_for_AFE::burst( uint32_t *data, int length, int width )
{
	uint8_t		v[ command_length + 3 * 16 ];
	uint16_t	reg	  = (0x2005 << 1) | 0x4000;	// CMD_BURST_DATA

	v[ 0 ]	= (uint8_t)(reg >> 8);
	v[ 1 ]	= (uint8_t)(reg & 0xFF);
	
	txrx( v, command_length + length * width );
	
	for ( auto i = 0; i < length; i++ )
		*data++	= get_data24( v + command_length + i * width );
}
