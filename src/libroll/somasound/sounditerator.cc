/***************************************************************************
                          sounditerator.cc
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

#include "sounditerator.h"
#include "soundslot.h"
#include <map>
#include <fstream>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


using namespace soma;
using namespace audiq;
using namespace std;


SoundIterator::SoundIterator( SoundSlot & sslot )
  : _sound( &sslot )
{
}


SoundIterator::~SoundIterator()
{
}


SoundIterator::size_type SoundIterator::size() const
{
  WavHeader & hdr = _sound->compressinfo.hdr;
  return hdr.size * hdr.sampleSize * hdr.channels;
}


SoundIterator::size_type SoundIterator::read( size_type nbytes, char* buffer )
{
  size_type pos0 = pos();
  for( size_type i=0; i<nbytes; ++i )
    *buffer++ = getch();
  return pos() - pos0;
}


// ---

namespace
{

  map<string, SoundIterator::CreationFunction> & soundIteratorFactory()
  {
    static map<string, SoundIterator::CreationFunction> creators;

    if( creators.empty() )
    {
      creators[ ":memory:" ] = &MemorySoundIterator::create;
      creators[ "WAVE" ] = &WavFileSoundIterator::create;
      creators[ "AudiqDiffCode" ] = &DiffCodedFileSoundIterator::create;
    }

    return creators;
  }

}


SoundIterator* SoundIterator::makeIterator( SoundSlot & sound )
{
  string format;
  if( sound.loaded )
    format = ":memory:";
  else if( sound.diffcoded )
    format = "AudiqDiffCode";
  else
    format = "WAVE";
  map<string, CreationFunction>::const_iterator
    i = soundIteratorFactory().find( format );
  if( i == soundIteratorFactory().end() )
    return 0;
  return i->second( sound );
}


void SoundIterator::registerIteratorType( const std::string & format,
                                          CreationFunction creator )
{
  soundIteratorFactory()[ format ] = creator;
}


// ---

MemorySoundIterator::MemorySoundIterator( SoundSlot & sound )
  : SoundIterator( sound ), _pos( 0 )
{
}


MemorySoundIterator::~MemorySoundIterator()
{
}


SoundIterator::size_type MemorySoundIterator::pos() const
{
  return _pos;
}


SoundIterator::size_type MemorySoundIterator::read( size_type nbytes,
                                                    char* buffer )
{
  size_type n = nbytes, sz = size(), m = 0;
  if( n + _pos >= sz )
    n = sz - _pos;
  if( n < 0 )
    return 0;
  if( _pos < 0 )
  {
    if( n < -_pos )
    {
      for( size_type i=0; i<n; ++i )
        *buffer++ = 0;
      _pos += n;
      return n;
    }
    n += _pos;
    m = -_pos;
    for( ; _pos<0; ++_pos )
      *buffer++ = 0;
  }
  if( n == 0 )
    return m;
  memcpy( buffer, (char *) &_sound->buffer[_pos], n );
  return n + m;
}


char MemorySoundIterator::getch()
{
  char c;
  if( _pos < 0 )
    c = 0;
  else if( _pos >= size() )
  {
    c = 0;
    return c;
  }
  else
    c = (char) _sound->buffer[_pos];
  ++_pos;
  return c;
}


void MemorySoundIterator::seek( size_type nbytes )
{
  _pos = nbytes;
}


SoundIterator *MemorySoundIterator::create( SoundSlot & sound )
{
  return new MemorySoundIterator( sound );
}


// -----

struct WavFileSoundIterator::Private
{
  Private() : pos(0), fd(-1) {}

  size_type pos;
  int fd;
};


WavFileSoundIterator::WavFileSoundIterator( SoundSlot & sl )
  : SoundIterator( sl ), d( new Private )
{
}


WavFileSoundIterator::~WavFileSoundIterator()
{
  if( d->fd >= 0 )
    ::close( d->fd );
  delete d;
}


bool WavFileSoundIterator::open()
{
  if( d->fd < 0 && sound().valid )
  {
    d->fd = ::open( sound().filename.c_str(), O_RDONLY );
    lseek( d->fd, sound().compressinfo.hdr.headerOffset, SEEK_CUR );
  }
  if( d->fd >= 0 )
    return true;
  sound().valid = false;
  return false;
}


SoundIterator::size_type
WavFileSoundIterator::read( size_type nbytes, char* buffer )
{
  if( open() )
  {
    size_type n = ::read( d->fd, buffer, nbytes );
    d->pos += n;
    return n;
  }
  return -1;
}


char WavFileSoundIterator::getch()
{
  if( open() )
  {
    char c;
    if( ::read( d->fd, &c, 1 ) == 1 )
    {
      ++d->pos;
      return c;
    }
  }
  return 0;
}


void WavFileSoundIterator::seek( size_type nbytes )
{
  if( open() )
  {
    size_type n = lseek( d->fd, nbytes, SEEK_CUR );
    d->pos += n;
  }
}


SoundIterator::size_type WavFileSoundIterator::pos() const
{
  return d->pos;
}


SoundIterator* WavFileSoundIterator::create( SoundSlot & sl )
{
  return new WavFileSoundIterator( sl );
}


// ---

struct DiffCodedFileSoundIterator::Private
{
  audiq::DiffCode::CompressedPos pos;
  ifstream sndstr;
};


DiffCodedFileSoundIterator::DiffCodedFileSoundIterator( SoundSlot & sl )
  : SoundIterator( sl ), d( new Private )
{
}


DiffCodedFileSoundIterator::~DiffCodedFileSoundIterator()
{
  delete d;
}


bool DiffCodedFileSoundIterator::open()
{
  if( sound().valid )
  {
    if( !d->sndstr.is_open() )
    {
      d->sndstr.open( sound().filename.c_str(), ios::in | ios::binary );
      d->sndstr.unsetf( ios::skipws );
      sound().compressinfo.read( d->sndstr, sound().filename );
      d->pos = DiffCode::CompressedPos( 0, sound().compressinfo.hdr.channels );
    }
    if( d->sndstr.is_open() )
      return true;
  }
  sound().valid = false;
  return false;
}


SoundIterator::size_type
DiffCodedFileSoundIterator::read( size_type nbytes, char* buffer )
{
  if( open() )
  {
    size_type p0 = d->pos.pos;
    d->pos = DiffCode::uncompress( d->sndstr, sound().compressinfo, buffer,
                                   d->pos, nbytes );
    return d->pos.pos - p0;
  }
  return -1;
}


char DiffCodedFileSoundIterator::getch()
{
  if( open() )
  {
    size_type p0 = d->pos.pos;
    char c;
    d->pos = DiffCode::uncompress( d->sndstr, sound().compressinfo, &c,
                                   d->pos, 1 );
    return d->pos.pos - p0;
  }
  return -1;
}


void DiffCodedFileSoundIterator::seek( size_type nbytes )
{
  if( open() )
  {
    vector<char> buffer( nbytes );
    d->pos = DiffCode::uncompress( d->sndstr, sound().compressinfo, &buffer[0],
                                   d->pos, nbytes );
  }
}


SoundIterator::size_type DiffCodedFileSoundIterator::pos() const
{
  return d->pos.pos;
}


SoundIterator* DiffCodedFileSoundIterator::create( SoundSlot & sl )
{
  return new DiffCodedFileSoundIterator( sl );
}


