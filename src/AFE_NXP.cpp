#include "AFE_NXP.h"

/* NAFE13388 class ******************************************/

NAFE13388::NAFE13388() : enabled_channels( 0 )
{
}

NAFE13388::~NAFE13388()
{
}

void NAFE13388::begin( void )
{
	pinMode( pin_nRESET,	OUTPUT );
	pinMode( pin_SYN,		OUTPUT );
	pinMode( pin_DRDY,		INPUT );
	pinMode( pin_nINT,		INPUT );
	digitalWrite( pin_nRESET,	1 );
	digitalWrite( pin_SYN,		1 );

	reset();
	boot();	
}

void NAFE13388::boot( void )
{
	write_r16( 0x0010 ); 
	write_r16( 0x002A, 0x0000 );
	write_r16( 0x002B, 0x0000 );
	write_r16( 0x002C, 0x0000 );
	write_r16( 0x002F, 0x0000 );
	write_r16( 0x0029, 0x0000 );
	delay( 1 );
	write_r16( 0x0030, 0x0010 );
	delay( 1 );
}

void NAFE13388::reset( void )
{
	write_r16( 0x0014 ); 
	delay( 1 );
}

void NAFE13388::logical_ch_config( int ch, uint16_t cc0, uint16_t cc1, uint16_t cc2, uint16_t cc3 )
{	
	constexpr double	pga_gain[]	= { 0.2, 0.4, 0.8, 1, 2, 4, 8, 16 };
	
	write_r16( ch );
	
	write_r16( 0x0020, cc0 );
	write_r16( 0x0021, cc1 );
	write_r16( 0x0022, cc2 );
	write_r16( 0x0023, cc3 );
	
	uint16_t	mask	= 1;
	uint16_t	bits	= read_r16( 0x0024 ) | (mask << ch);
	enabled_channels	= 0;
	
	for ( int i = 0; i < 16; i++ ) {
		if ( bits & (mask << i) )
			enabled_channels++;
	}
	
	write_r16( 0x0024, bits );
	
	coeff_uV[ ch ]	= ((10.0 / (double)(1L << 24)) / pga_gain[ (cc0 >> 5) & 0x7 ]) * 1e6;
}

double NAFE13388::read( int ch )
{
	write_r16( ch );
	write_r16( 0x2000 );
	delay( 100 );
	
	return read_r24( 0x2040 + ch ) * coeff_uV[ ch ];
};

//double	NAFE13388::coeff_uV[ 16 ];
