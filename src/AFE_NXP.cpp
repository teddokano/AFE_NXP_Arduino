#include "AFE_NXP.h"

/* NAFE13388_Base class ******************************************/

NAFE13388_Base::NAFE13388_Base() : enabled_channels( 0 )
{
}

NAFE13388_Base::~NAFE13388_Base()
{
}

void NAFE13388_Base::begin( void )
{
	reset();
	boot();	
}

void NAFE13388_Base::boot( void )
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

void NAFE13388_Base::reset( void )
{
	write_r16( 0x0014 ); 
	delay( 1 );
}

void NAFE13388_Base::board_init( int _pin_nINT, int _pin_DRDY, int _pin_SYN, int _pin_nRESET )
{
	pinMode( _pin_nINT,		INPUT );
	pinMode( _pin_DRDY,		INPUT );
	pinMode( _pin_SYN,		OUTPUT );
	pinMode( _pin_nRESET,	OUTPUT );

	digitalWrite( _pin_SYN,		1 );
	digitalWrite( _pin_nRESET,	1 );
	
	Serial.print("_pin_nRESET = ");
	Serial.println(_pin_nRESET);

}

void NAFE13388_Base::logical_ch_config( int ch, uint16_t cc0, uint16_t cc1, uint16_t cc2, uint16_t cc3 )
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

double NAFE13388_Base::read( int ch )
{
	write_r16( ch );
	write_r16( 0x2000 );
	delay( 100 );
	
	return read_r24( 0x2040 + ch ) * coeff_uV[ ch ];
};

/* NAFE13388 class ******************************************/

NAFE13388::NAFE13388() : NAFE13388_Base()
{
	board_init( pin_nINT, pin_DRDY, pin_SYN, pin_nRESET );
}

NAFE13388::~NAFE13388()
{
}

/* NAFE13388_UIM class ******************************************/

NAFE13388_UIM::NAFE13388_UIM() : NAFE13388_Base()
{
	board_init( pin_nINT, pin_DRDY, pin_SYN, pin_nRESET );
}

NAFE13388_UIM::~NAFE13388_UIM()
{
}

//double	NAFE13388::coeff_uV[ 16 ];
