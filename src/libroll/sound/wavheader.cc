/***************************************************************************
    copyright            : (C) 2002-2004 by Denis Rivière
    email                : nudz@free.fr
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "wavheader.h"
#include <fstream>
#include <stdexcept>

using namespace audiq;
using namespace std;


WavHeader::WavHeader() 
  : channels( 0 ), size( 0 ), sampleSize( 0 ), sign( true ), headerOffset( 44 )
{
}


WavHeader::~WavHeader()
{
}


void WavHeader::read( const std::string & fname )
{
  ifstream	file( fname.c_str(), ios::in | ios::binary );
  if( !file )
    throw runtime_error( string( "cannot open file " ) + fname );
  read( file, fname );
}


void WavHeader::read( istream & file, const string & fname )
{
  filename = fname;
  headerOffset = 44;

  char	buf[10];
  file.read( buf, 4 );
  buf[4] = '\0';
  if( string( "RIFF" ) != buf )
    throw runtime_error( fname + " : not a .WAV file " );
  unsigned char	c1, c2, c3, c4;
  file.read( (char *) &c1, 1 );
  file.read( (char *) &c2, 1 );
  file.read( (char *) &c3, 1 );
  file.read( (char *) &c4, 1 );
  size_t	len = (((size_t) c4) << 24 ) | (((size_t) c3) << 16 ) 
    | (((size_t) c2) << 8 ) | c1;
  cout << "len (bytes): " << len << endl;
  file.read( buf, 8 );
  buf[8] = '\0';
  if( string( "WAVEfmt " ) != buf )
    throw runtime_error( fname + " : not a .WAV file " );
  file.read( (char *) &c1, 1 );
  file.read( (char *) &c2, 1 );
  file.read( (char *) &c3, 1 );
  file.read( (char *) &c4, 1 );
  unsigned short	x = (((size_t) c4) << 24 ) | (((size_t) c3) << 16 ) 
    | (((size_t) c2) << 8 ) | c1;
  if( x != 0x10 )
    cerr << "warning: WAV length of FORMAT chunk is incorrect\n";
  file.read( (char *) &c1, 1 );
  file.read( (char *) &c2, 1 );
  x = (((unsigned short) c2) << 8 ) | c1;
  if( x != 1 )
    cerr << "warning: WAV format malformed, FORMAT chunk, offset 8-9 value: "
	 << x << ", should be 1\n";
  file.read( (char *) &c1, 1 );
  file.read( (char *) &c2, 1 );
  x = (((unsigned short) c2) << 8 ) | c1;
  channels = x;
  cout << "channels: " << channels << endl;
  file.read( (char *) &c1, 1 );
  file.read( (char *) &c2, 1 );
  file.read( (char *) &c3, 1 );
  file.read( (char *) &c4, 1 );
  len = (((size_t) c4) << 24 ) | (((size_t) c3) << 16 ) 
    | (((size_t) c2) << 8 ) | c1;
  rate = len;
  cout << "rate: " << rate << endl;
  file.read( (char *) &c1, 1 );
  file.read( (char *) &c2, 1 );
  file.read( (char *) &c3, 1 );
  file.read( (char *) &c4, 1 );
  len = (((size_t) c4) << 24 ) | (((size_t) c3) << 16 ) 
    | (((size_t) c2) << 8 ) | c1;
  cout << "bytes/sec: " << len << endl;
  file.read( (char *) &c1, 1 );
  file.read( (char *) &c2, 1 );
  x = (((unsigned short) c2) << 8 ) | c1;
  cout << "bytes/sample: " << x << endl;
  file.read( (char *) &c1, 1 );
  file.read( (char *) &c2, 1 );
  x = (((unsigned short) c2) << 8 ) | c1;
  cout << "bits/channel: " << x << endl;
  sampleSize = x/8;
  file.read( buf, 4 );
  buf[4] = '\0';
  if( string( "data" ) != buf )
    throw runtime_error( fname + " : not a .WAV file " );
  file.read( (char *) &c1, 1 );
  file.read( (char *) &c2, 1 );
  file.read( (char *) &c3, 1 );
  file.read( (char *) &c4, 1 );
  len = (((size_t) c4) << 24 ) | (((size_t) c3) << 16 ) 
    | (((size_t) c2) << 8 ) | c1;
  len /= sampleSize * channels;
  cout << "len in samples: " << len << endl;
  size = len;
}


void WavHeader::write( const std::string & fname )
{
  ofstream	file( fname.c_str(), ios::out | ios::binary );
  if( !file )
    throw runtime_error( string( "cannot write file " ) + fname );
  write( file );
}


void WavHeader::write( ostream & file )
{
  file << "RIFF";
  unsigned	len = size * sampleSize * channels + 36;
  file.put( len & 0xff );
  file.put( ( len >> 8 ) & 0xff );
  file.put( ( len >> 16 ) & 0xff );
  file.put( len >> 24 );
  file << "WAVEfmt ";
  file.put( 0x10 );
  file.put( 0 );
  file.put( 0 );
  file.put( 0 );
  file.put( 1 );
  file.put( 0 );
  file.put( channels & 0xff );
  file.put( ( channels >> 8 ) & 0xff );	// should be 0
  len = rate;
  file.put( len & 0xff );
  file.put( ( len >> 8 ) & 0xff );
  file.put( ( len >> 16 ) & 0xff );
  file.put( len >> 24 );
  len = rate * sampleSize * channels;	// bytes / sec
  file.put( len & 0xff );
  file.put( ( len >> 8 ) & 0xff );
  file.put( ( len >> 16 ) & 0xff );
  file.put( len >> 24 );
  len = sampleSize * channels;		// bytes / sample
  file.put( len & 0xff );
  file.put( ( len >> 8 ) & 0xff );
  len = sampleSize * 8;			// bits / sample
  file.put( len & 0xff );
  file.put( ( len >> 8 ) & 0xff );
  file << "data";
  len = size * sampleSize * channels;	// len of samples
  file.put( len & 0xff );
  file.put( ( len >> 8 ) & 0xff );
  file.put( ( len >> 16 ) & 0xff );
  file.put( len >> 24 );
}

