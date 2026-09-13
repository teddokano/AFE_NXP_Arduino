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

	//	ch_chop halves base_freq again on top of the plain case
	assert( close_enough( AFE_base::calc_delay_from_config( 0x0000, 0x0280, false ), 1.0 / 144000.0 ) );

	//	ch_delay index 5 -> delays[5] = 10, adds 10/4608000 seconds on top of the base 1/288000
	assert( close_enough( AFE_base::calc_delay_from_config( 0x0000, 0x1600, false ), (1.0 / 288000.0) + (10.0 / 4608000.0) ) );

	std::printf( "test_calc_delay: all assertions passed\n" );
	return 0;
}
