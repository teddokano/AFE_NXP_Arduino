/*
 * Minimal host-side stand-in for the Arduino core, just enough for the
 * library's headers and sources to compile and link on a desktop
 * g++/clang so their non-hardware logic can be unit tested. Every
 * function here is a no-op or a trivial fake -- nothing here talks to
 * real hardware.
 */
#ifndef ARDUINO_STUB_ARDUINO_H
#define ARDUINO_STUB_ARDUINO_H

#include <cstdint>
#include <cstdio>
#include <cstring>

using std::uint8_t;
using std::uint16_t;
using std::uint32_t;
using std::int32_t;

constexpr int HIGH			= 1;
constexpr int LOW			= 0;
constexpr int INPUT		= 0;
constexpr int OUTPUT		= 1;
constexpr int INPUT_PULLUP	= 2;
constexpr int CHANGE		= 1;
constexpr int SS			= 10;

inline void pinMode( int, int )				{}
inline void digitalWrite( int, int )			{}
inline int  digitalRead( int )					{ return LOW; }
inline void delay( unsigned long )				{}
inline void delayMicroseconds( unsigned int )	{}
inline unsigned long micros( void )			{ return 0; }
inline unsigned long millis( void )			{ return 0; }
inline int  digitalPinToInterrupt( int pin )	{ return pin; }
inline void attachInterrupt( int, void (*)( void ), int ) {}

/* Fake Serial: keeps stub self-contained; writes go to stdout so a
 * host test can still see AFE_DBG()/Serial.print() output if wanted. */
struct SerialStub
{
	void begin( unsigned long ) {}
	explicit operator bool() const { return true; }

	void print( const char *s )				{ std::fputs( s, stdout ); }
	void print( int v )						{ std::printf( "%d", v ); }
	void print( unsigned int v )				{ std::printf( "%u", v ); }
	void print( long v )						{ std::printf( "%ld", v ); }
	void print( unsigned long v )				{ std::printf( "%lu", v ); }
	void print( double v, int digits = 2 )		{ std::printf( "%.*f", digits, v ); }
	void print( double v, const char * )		{ std::printf( "%f", v ); }

	void println( void )						{ std::fputs( "\n", stdout ); }
	void println( const char *s )				{ std::fputs( s, stdout ); std::fputs( "\n", stdout ); }
	void println( int v )						{ std::printf( "%d\n", v ); }
	void println( unsigned int v )				{ std::printf( "%u\n", v ); }
	void println( long v )						{ std::printf( "%ld\n", v ); }
	void println( unsigned long v )			{ std::printf( "%lu\n", v ); }
	void println( double v, int digits = 2 )	{ std::printf( "%.*f\n", digits, v ); }
};

extern SerialStub Serial;

#endif	//	ARDUINO_STUB_ARDUINO_H
