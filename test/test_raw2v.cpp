/*
 * Host unit test for NAFE13388_Base::raw2v()'s low-voltage MUX branch
 * (2.00 * (v + 1.50), 32.00 * (v + 0.25), -32.00 * (v - 0.25), ...).
 *
 * Exercises the real, unmodified open_logical_channel() / raw2v() via
 * a NAFE13388_UIM instance -- coeff_V[]/mux_setting[] are set purely
 * from the cc[] argument (no register read-back needed), so the
 * stubbed, do-nothing SPI transport doesn't affect this test.
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
	NAFE13388_UIM afe;

	constexpr NAFE13388_UIM::raw_t	raw	= 1000000;

	//	LV MUX coefficient: (10.0 / 2^24) / 2.5
	const double	lv_coeff	= (10.0 / (double)(1L << 24)) / 2.5;
	const double	v			= raw * lv_coeff;

	//	mux_setting = (cc0 >> 1) & 0x7, HV bit (0x0010) clear selects the LV path
	afe.open_logical_channel( 0, (0 << 1), 0x00A4, 0x8400, 0x0000 );	//	REF2_REF2
	afe.open_logical_channel( 1, (2 << 1), 0x00A4, 0x8400, 0x0000 );	//	REFCOARSE_REF2
	afe.open_logical_channel( 2, (4 << 1), 0x00A4, 0x8400, 0x0000 );	//	VHDD_REF2
	afe.open_logical_channel( 3, (5 << 1), 0x00A4, 0x8400, 0x0000 );	//	REF2_VHSS

	assert( close_enough( afe.raw2v( 0, raw ), v ) );
	assert( close_enough( afe.raw2v( 1, raw ), 2.00 * (v + 1.50) ) );
	assert( close_enough( afe.raw2v( 2, raw ), 32.00 * (v + 0.25) ) );
	assert( close_enough( afe.raw2v( 3, raw ), -32.00 * (v - 0.25) ) );

	//	HV MUX path: bit 0x0010 set selects the high-voltage gain table instead
	//	of the LV MUX branch above, and raw2v() returns v unscaled.
	const double	hv_coeff	= (10.0 / (double)(1L << 24)) / 0.2;	//	pga_gain[0] == 0.2
	const double	hv_v		= raw * hv_coeff;

	afe.open_logical_channel( 4, 0x0010, 0x00A4, 0x8400, 0x0000 );
	assert( close_enough( afe.raw2v( 4, raw ), hv_v ) );

	std::printf( "test_raw2v: all assertions passed\n" );
	return 0;
}
