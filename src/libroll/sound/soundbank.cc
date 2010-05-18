/***************************************************************************
                          soundBank.h
                        -------------------
    begin                : 1999
    copyright            : (C) 1999-2010 by Denis Rivière
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

#include "soundslot.h"
#include <unistd.h>

using namespace soma;
using namespace audiq;
using namespace std;


SoundBank::SoundSlot::SoundSlot( const string & filename, bool shouldbeloaded )
: filename( filename ), shouldbeloaded( shouldbeloaded ), valid( true ),
  loaded( false ), stream( 0 ), diffcoded( false )
{
}


SoundBank::SoundSlot::~SoundSlot()
{
  if( stream )
    delete stream;
  if( !tempfilename.empty() )
    unlink( tempfilename.c_str() );
}


SoundBank::SoundBank()
{
}


SoundBank::~SoundBank()
{
}


int SoundBank::addSound( const string & filename, bool shouldBeLoaded )
{
  _sounds.push_back( SoundSlot( filename, shouldBeLoaded ) );
  return _sounds.size() - 1;
}


void SoundBank::loadSounds()
{
  vector<SoundSlot>::iterator i, e = _sounds.end();
  for( i=_sounds.begin(); i!=e; ++i )
  {
    SoundSlot & sl = *i;
    if( !sl.valid )
      continue;
    if( sl.filename.length() >= 4
      && sl.filename.substr( sl.filename.length() - 4, 4 ) == ".adc" )
      sl.diffcoded = true;
    else
      sl.diffcoded = false;

    WavHeader & hdr = sl.compressinfo.hdr;

    if( !sl.loaded && sl.shouldbeloaded )
    {
      ifstream str( sl.filename.c_str() );
      if( !str )
      {
        sl.valid = false;
        continue;
      }
      if( sl.diffcoded )
      {
        sl.compressinfo.read( str, sl.filename );
        DiffCode::CompressedPos pos;
        pos = DiffCode::CompressedPos( 0, hdr.channels );
        int sz = hdr.size * hdr.channels * hdr.sampleSize;
        sl.buffer.reserve( sz );
        sl.buffer.insert( sl.buffer.end(), sz, 0 );
        pos = DiffCode::uncompress( str, sl.compressinfo,
                                    (char *) &sl.buffer[0], pos, sz );
      }
      else
      {
        hdr.read( str, sl.filename );
        int sz = hdr.size;
        sl.buffer.reserve( sz );
        sl.buffer.insert( sl.buffer.end(), sz, 0 );
        str.read( (char *) &sl.buffer[0], sz );
      }
      sl.loaded = true;
    }
  }
}


void SoundBank::init()
{
  // add suitable SoundSlots
}


void SoundBank::reserve( unsigned n )
{
  _sounds.reserve( n );
}


SoundBank::SoundSlot & SoundBank::sound( unsigned n )
{
  return _sounds[n];
}


std::vector<SoundBank::SoundSlot> & SoundBank::sounds()
{
  return _sounds;
}


