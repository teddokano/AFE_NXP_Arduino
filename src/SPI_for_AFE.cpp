#include "AFE_NXP.h"

void SPI_for_AFE::txrx( uint8_t *data, int size )
{
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
	
	return (uint16_t)(v[ 2 ]) << 8 | v[ 3 ];
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
	
	int32_t	r0	= v[ 2 ];
	int32_t	r1	= v[ 3 ];
	int32_t	r2	= v[ 4 ];
	int32_t	r	= ( (r0 << 24) | (r1 << 16) | (r2 << 8) );

	return r >> 8;
}
