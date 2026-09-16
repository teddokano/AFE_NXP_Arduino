/*
 * Host unit test for AFE_base::calc_delay_from_config() -- the
 * data-rate/sinc/delay table lookup extracted from
 * NAFE13388_Base::calc_delay() / NAFE33352_Base::calc_delay() so it
 * can be tested without a register read over SPI.
 *
 * CH_CONFIG1 bit layout: adc_data_rate = bits[7:3], adc_sinc = bits[2:0]
 * CH_CONFIG2 bit layout: ch_delay = bits[15:10], adc_normal_setting = bit[9], ch_chop = bit[7]
 */
#include "AFE_NXP.h"

#include <cassert>
#include <cmath>
#include <cstdio>

static bool close_enough( double a, double b, double eps = 1e-9 )
{
	return std::fabs( a - b ) < eps;
}

int main()
{
	//	adc_data_rate=0 (288000 Hz), sinc=0, ch_delay=0, normal_setting=1, chop=0
	assert( close_enough( AFE_base::calc_delay_from_config( 0x0000, 0x0200, false ), 1.0 / 288000.0 ) );

	//	same, but the -MB highspeed variant halves the delay contribution and doubles base_freq
	assert( close_enough( AFE_base::calc_delay_from_config( 0x0000, 0x0200, true ), 1.0 / 576000.0 ) );

	//	adc_data_rate=29 is out of table range -> reserved, must return 0.0
	assert( AFE_base::calc_delay_from_config( (29 << 3) | 0, 0x0200, false ) == 0.0 );

	//	adc_sinc=5 exceeds the valid 0-4 range -> reserved
	assert( AFE_base::calc_delay_from_config( (0 << 3) | 5, 0x0200, false ) == 0.0 );

	//	adc_data_rate=5 (< 12) with non-zero sinc is a reserved combination
	assert( AFE_base::calc_delay_from_config( (5 << 3) | 1, 0x0200, false ) == 0.0 );

	//	adc_normal_setting=0 divides base_freq by (sinc+1): data_rate=12 (4500Hz), sinc=1
	assert( close_enough( AFE_base::calc_delay_from_config( (12 << 3) | 1, 0x0000, false ), 1.0 / (4500.0 / 2) ) );

	//	adc_normal_setting=0 with codes 0 ... 11 (SINC bypassed) is four times slower:
	//	data_rate=0 is 72000Hz, not 288000Hz
	assert( close_enough( AFE_base::calc_delay_from_config( 0x0000, 0x0000, false ), 1.0 / 72000.0 ) );

	//	Every valid data_rate/sinc/settling combination against datasheet Table 7
	//	(system clock 4.608 MHz)
	{
		constexpr double	normal[]	= {	288000, 192000, 144000, 96000, 72000, 48000, 36000, 24000,
											 18000,  12000,   9000,  6000,  4500,  3000,  2250,  1125,
											 562.5,    400,    300,   200,   100,    60,    50,    30,
												25,     20,     15,    10,   7.5, };

		constexpr double	single_bypass[]	= {	72000, 48000, 36000, 24000, 18000, 12000,
												 9000,  6000,  4500,  3000,  2250,  1500, };

		constexpr double	single_sinc[][ 4 ]	= {	//	SINC1, SINC2, SINC3, SINC4
			{ 2250.00, 1500.00, 1125.00, 900.00 },
			{ 1500.00, 1000.00,  750.00, 600.00 },
			{ 1125.00,  750.00,  562.50, 450.00 },
			{  562.50,  375.00,  281.25, 225.00 },
			{  281.25,  187.50,  140.625, 112.50 },
			{  200.00,  400.0 / 3, 100.00,  80.00 },
			{  150.00,  100.00,   75.00,  60.00 },
			{  100.00,  200.0 / 3,  50.00,  40.00 },
			{   50.00,  100.0 / 3,  25.00,  20.00 },
			{   30.00,   20.00,   15.00,  12.00 },
			{   25.00,   50.0 / 3,  12.50,  10.00 },
			{   15.00,   10.00,    7.50,   6.00 },
			{   12.50,   25.0 / 3,   6.25,   5.00 },
			{   10.00,   20.0 / 3,   5.00,   4.00 },
			{    7.50,    5.00,    3.75,   3.00 },
			{    5.00,   10.0 / 3,   2.50,   2.00 },
			{    3.75,    2.50,    1.875,  1.50 },
		};

		for ( int code = 0; code <= 28; code++ )
		{
			if ( code < 12 )
			{
				const uint16_t	cc1	= (uint16_t)( code << 3 );
				assert( close_enough( AFE_base::calc_delay_from_config( cc1, 0x0200, false ), 1.0 / normal[ code ] ) );
				assert( close_enough( AFE_base::calc_delay_from_config( cc1, 0x0000, false ), 1.0 / single_bypass[ code ] ) );
			}
			else
			{
				for ( int sinc = 1; sinc <= 4; sinc++ )
				{
					const uint16_t	cc1	= (uint16_t)( ( code << 3 ) | sinc );
					assert( close_enough( AFE_base::calc_delay_from_config( cc1, 0x0200, false ), 1.0 / normal[ code ] ) );
					assert( close_enough( AFE_base::calc_delay_from_config( cc1, 0x0000, false ), 1.0 / single_sinc[ code - 12 ][ sinc - 1 ] ) );
				}
			}
		}
	}

	//	ch_chop halves base_freq again on top of the plain case
	assert( close_enough( AFE_base::calc_delay_from_config( 0x0000, 0x0280, false ), 1.0 / 144000.0 ) );

	//	ch_delay index 5 -> delays[5] = 10, adds 10/4608000 seconds on top of the base 1/288000
	assert( close_enough( AFE_base::calc_delay_from_config( 0x0000, 0x1600, false ), (1.0 / 288000.0) + (10.0 / 4608000.0) ) );

	//	CH_DELAY is a 6 bit field but only codes 0 ... 33 have a defined delay.
	//	Sweep every code the field can hold: codes above the table must be clamped
	//	to the last defined one rather than reading past the end of delays[].
	{
		constexpr double	base		= 1.0 / 288000.0;		//	data_rate=0, sinc=0
		constexpr uint16_t	normal_set	= 0x0200;				//	adc_normal_setting=1, chop=0
		constexpr int		last_code	= 33;
		constexpr double	last_delay	= 23040.0 / 4608000.0;	//	delays[ 33 ]

		double	prev	= -1.0;

		for ( int code = 0; code < 64; code++ )
		{
			const uint16_t	cc2	= (uint16_t)( ( code << 10 ) | normal_set );
			const double	d	= AFE_base::calc_delay_from_config( 0x0000, cc2, false );

			//	always at least the conversion time, and never NaN/garbage
			assert( base <= d );
			assert( d < 1.0 );

			//	delays[] is monotonically increasing, so the result must be too
			assert( prev <= d );
			prev	= d;

			if ( last_code < code )
				assert( close_enough( d, base + last_delay ) );
		}

		//	the last defined code and everything above it land on the same value
		assert( close_enough( AFE_base::calc_delay_from_config( 0x0000, (last_code << 10) | normal_set, false ),
							  AFE_base::calc_delay_from_config( 0x0000, (63 << 10) | normal_set, false ) ) );

		//	0xBC00 is the CH_CONFIG2 value the examples used to carry: CH_DELAY = 47,
		//	which is undefined and used to read past delays[]. It now behaves the
		//	same as 0x8400, the explicit CH_DELAY = 33 the examples were moved to.
		assert( close_enough( AFE_base::calc_delay_from_config( 0x00A4, 0xBC00, false ),
							  AFE_base::calc_delay_from_config( 0x00A4, 0x8400, false ) ) );
	}

	std::printf( "test_calc_delay: all assertions passed\n" );
	return 0;
}
