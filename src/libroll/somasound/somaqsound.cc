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

#include "somaqsound.h"
#include "soundbank.h"
#include "soundslot.h"
#include <qsound.h>
#include <memory>
#include <vector>

using namespace soma;
using namespace std;


struct SomaQSound::Private
{
  Private() : initialized( false ) {}

  vector<QSound *>	sounds;
  bool			initialized;
};


SomaQSound::SomaQSound()
  : SomaSoundProcessor(), d( new Private )
{
}


SomaQSound::~SomaQSound()
{
  unsigned	i, n = d->sounds.size();
  for( i=0; i<n; ++i )
    delete d->sounds[i];
  delete d;
}

void SomaQSound::process( int type )
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


void SomaQSound::stop()
{
  if( !isOK() )
    return;

  unsigned	i, n = d->sounds.size();
  for( i=0; i<n; ++i )
    d->sounds[i]->stop();
}


void SomaQSound::stop( int type )
{
  if( !isOK() )
    return;

  d->sounds[ type ]->stop();
  --_inuse[ type ];
}


void SomaQSound::close()
{
  stop();
}


bool SomaQSound::isOK() const
{
  const_cast<SomaQSound *>( this )->init();
  return QSound::isAvailable();
}


void SomaQSound::init()
{
  if( d->initialized || !QSound::isAvailable() )
    return;
  d->initialized = true;

  SomaSoundProcessor::init();
  unsigned	i, n = d->sounds.size();
  for( i=0; i<n; ++i )
    delete d->sounds[i];
  d->sounds.clear();
  loadSounds();
}


void SomaQSound::loadSounds()
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


string SomaQSound::name() const
{
  return "Qt sound";
}


float SomaQSound::priorityRating() const
{
  return 10;
}


