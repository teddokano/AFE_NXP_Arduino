/*
 * Host unit test for SPI_for_AFE::get_data24() (T-08 sign extension).
 */
#include "SPI_for_AFE.h"

#include <cassert>
#include <cstdio>

int32_t test_get_data24( SPI_for_AFE &obj, uint8_t *vp )
{
	return obj.get_data24( vp );
}

int main()
{
	SPI_for_AFE spi;

	uint8_t min_bytes[]  = { 0x80, 0x00, 0x00 };	//	most negative 24-bit value
	uint8_t max_bytes[]  = { 0x7F, 0xFF, 0xFF };	//	most positive 24-bit value
	uint8_t neg1_bytes[] = { 0xFF, 0xFF, 0xFF };	//	-1
	uint8_t zero_bytes[] = { 0x00, 0x00, 0x00 };	//	0

	assert( test_get_data24( spi, min_bytes )  == -8388608 );
	assert( test_get_data24( spi, max_bytes )  ==  8388607 );
	assert( test_get_data24( spi, neg1_bytes ) ==  -1 );
	assert( test_get_data24( spi, zero_bytes ) ==   0 );

	std::printf( "test_get_data24: all assertions passed\n" );
	return 0;
}
