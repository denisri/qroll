/***************************************************************************
                          qsound.cc
                             -------------------
    begin                : 2004
    copyright            : (C) 2004-2006 by Denis Rivière
    email                : nudz@free.fr
                           http://nudz.free.fr
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qsound.h"
#include "soundslot.h"
#include <qsound.h>
#include <memory>
#include <vector>

using namespace roll;
using namespace std;


struct RRQSound::Private
{
  Private() : initialized( false ) {}

  vector<QSound *>	sounds;
  bool			initialized;
};


RRQSound::RRQSound()
  : RRSoundProcessor(), d( new Private )
{
}


RRQSound::~RRQSound()
{
  unsigned	i, n = d->sounds.size();
  for( i=0; i<n; ++i )
    delete d->sounds[i];
  delete d;
}

void RRQSound::process( int type )
{
  unsigned nsnd = soundBank().sounds().size();
  if( (unsigned) type >= nsnd || !isOK() )
    return;

  if( d->sounds.size() < nsnd )
  {
    unsigned i;
    string filename;
    d->sounds.reserve( nsnd );
    for( i=d->sounds.size(); i<nsnd; ++i )
    {
      filename = soundBank().sound( i ).filename;
      d->sounds.push_back( new QSound( filename.c_str() ) );
    }
  }

  if( d->sounds[ type ] )
  {
    if( _inuse.size() <= (unsigned) type )
    {
      _inuse.reserve( nsnd );
      _inuse.insert( _inuse.end(), nsnd - _inuse.size(), 0 );
    }
    d->sounds[ type ]->play();
    ++_inuse[ type ];
  }
}


void RRQSound::stop()
{
  if( !isOK() )
    return;

  unsigned	i, n = d->sounds.size();
  for( i=0; i<n; ++i )
    d->sounds[i]->stop();
}


void RRQSound::stop( int type )
{
  if( !isOK() )
    return;

  d->sounds[ type ]->stop();
  --_inuse[ type ];
}


void RRQSound::close()
{
  stop();
}


bool RRQSound::isOK() const
{
  const_cast<RRQSound *>( this )->init();
  return QSound::isAvailable();
}


void RRQSound::init()
{
  if( d->initialized || !QSound::isAvailable() )
    return;
  d->initialized = true;

  RRSoundProcessor::init();
  unsigned	i, n = d->sounds.size();
  for( i=0; i<n; ++i )
    delete d->sounds[i];
  d->sounds.clear();
  loadSounds();
}


void RRQSound::loadSounds()
{
  unsigned	i, n = soundBank().sounds().size();
  string	filename;


  for( i=0; i<n; ++i )
  {
    filename = soundBank().sound( i ).filename;
    if( d->sounds.size() < i )
      d->sounds.push_back( new QSound( filename.c_str() ) );
    else if( !d->sounds[i] )
      d->sounds[i] = new QSound( filename.c_str() );
  }
}


string RRQSound::name() const
{
  return "Qt sound";
}


float RRQSound::priorityRating() const
{
  return 10;
}


