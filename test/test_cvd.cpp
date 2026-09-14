/*
 * Host unit test for the RTD Callendar-Van Dusen conversion used in
 * examples/NAFE13388_5_0_RTD_4_wire (ported from
 * examples/NAFE13388_5_0_RTD_4_wire/test_code/test_get_temp_cvd/).
 *
 * Pure math, no AFE / SPI hardware involved.
 */
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <initializer_list>

static double get_temp( double resistance )
{
	static constexpr double coef_pt100 = 0.385;
	return (resistance - 100) / coef_pt100;
}

static double get_temp_cvd( double resistance, int iterations = 5 )
{
	static constexpr double R0 = 100.0;
	static constexpr double A  = 3.9083e-3;
	static constexpr double B  = -5.775e-7;
	static constexpr double C  = -4.183e-12;

	if ( resistance >= R0 )
	{
		double ratio = resistance / R0;
		return (-A + std::sqrt( A * A - 4.0 * B * (1.0 - ratio) )) / (2.0 * B);
	}

	double t = (resistance - R0) / (R0 * 0.00385);

	for ( int i = 0; i < iterations; i++ )
	{
		double f  = R0 * (1.0 + A * t + B * t * t + C * (t - 100.0) * t * t * t) - resistance;
		double df = R0 * (A + 2.0 * B * t + C * (4.0 * t * t * t - 300.0 * t * t));
		t -= f / df;
	}

	return t;
}

struct ref_point { double temp; double resistance; };

//	IEC 60751 reference table for Pt100 (temperature[degC], resistance[ohm])
static const ref_point reference_table[] = {
	{ -50.0,  80.31 }, { -40.0,  84.27 }, { -30.0,  88.22 }, { -20.0,  92.16 },
	{ -10.0,  96.09 }, {   0.0, 100.00 }, {  10.0, 103.90 }, {  20.0, 107.79 },
	{  30.0, 111.67 }, {  40.0, 115.54 }, {  50.0, 119.40 }, {  60.0, 123.24 },
	{  70.0, 127.07 }, {  80.0, 130.89 }, {  90.0, 134.70 }, { 100.0, 138.50 },
	{ 110.0, 142.29 }, { 120.0, 146.06 }, { 130.0, 149.82 }, { 140.0, 153.58 },
	{ 150.0, 157.31 },
};

int main()
{
	//	Test 1: CVD tracks the IEC 60751 table tightly; linear approximation drifts.
	double max_cvd_error	= 0.0;
	double max_linear_error	= 0.0;

	for ( const auto &p : reference_table )
	{
		double cvd_error	= std::fabs( get_temp_cvd( p.resistance ) - p.temp );
		double linear_error	= std::fabs( get_temp( p.resistance ) - p.temp );

		max_cvd_error		= std::max( max_cvd_error, cvd_error );
		max_linear_error	= std::max( max_linear_error, linear_error );
	}

	std::printf( "max |CVD error|    = %.5f degC\n", max_cvd_error );
	std::printf( "max |linear error| = %.5f degC\n", max_linear_error );

	assert( max_cvd_error < 0.05 );		//	CVD should track the table within 0.05 degC
	assert( max_linear_error > 1.00 );		//	linear approx. is known to drift by >1 degC at the extremes

	//	Test 2: Newton-Raphson (T < 0 degC branch) converges within the fixed 5 iterations.
	for ( double r : { 80.31, 88.22, 96.09 } )
	{
		double converged	= get_temp_cvd( r, 5 );
		double one_less		= get_temp_cvd( r, 4 );

		assert( std::fabs( converged - one_less ) < 1e-6 );
	}

	std::printf( "test_cvd: all assertions passed\n" );
	return 0;
}
