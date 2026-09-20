/*
 * Minimal host-side stand-in for the Arduino SPI library. transfer()
 * leaves the buffer untouched -- callers that need specific register
 * read-back values must not rely on this stub's SPI to provide them.
 */
#ifndef ARDUINO_STUB_SPI_H
#define ARDUINO_STUB_SPI_H

#include <cstddef>
#include <cstdint>

constexpr int MSBFIRST	= 0;
constexpr int SPI_MODE1	= 1;

struct SPISettings
{
	SPISettings( unsigned long, int, int ) {}
};

struct SPIClass
{
	void begin( void ) {}
	void beginTransaction( SPISettings ) {}
	void endTransaction( void ) {}
	void usingInterrupt( int ) {}
	void transfer( uint8_t *, size_t ) {}
};

extern SPIClass SPI;

#endif	//	ARDUINO_STUB_SPI_H
