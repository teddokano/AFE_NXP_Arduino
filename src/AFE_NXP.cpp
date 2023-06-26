#include "AFE_NXP.h"

/* PCA8561 class ******************************************/

NAFE13388::NAFE13388() : enabled_channels( 0 )
{
}

NAFE13388::~NAFE13388()
{
}

void NAFE13388::begin( void )
{
	pinMode( 6, OUTPUT );
	pinMode( 5, OUTPUT );
	pinMode( 3, INPUT );
	pinMode( 2, INPUT );
	digitalWrite( 6 , 1 );
	digitalWrite( 5 , 1 );

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
	
	double pga;
	
	switch ( (cc0 >> 5) & 0x7  )
	{
		case 0:
			pga	= 0.2;
			break;
		case 1:
			pga	= 0.4;
			break;
		default:
			pga	= 0.8;
			break;
	}
	
	coeff_uV[ ch ]	= ((10.0 / (double)(1L << 24)) / pga) * 1e6;
}

double NAFE13388::read( int ch )
{
	write_r16( ch );
	write_r16( 0x2000 );
	delay( 100 );
	
	return read_r24( 0x2040 + ch ) * coeff_uV[ ch ];
};

//double	NAFE13388::coeff_uV[ 16 ];
