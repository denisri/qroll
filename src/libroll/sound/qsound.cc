/***************************************************************************
                          qsound.cc
                             -------------------
    begin                : 2004
    copyright            : (C) 2004-2006 by Denis Rivi�re
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

#include <roll/sound/qsound.h>
#include <roll/struct/general.h>
#include <qsound.h>
#include <memory>
#include <vector>

using namespace roll;
using namespace std;


// namespace
// {
// 
//   bool dummy()
//   {
//     new RRQSound;
//     return true;
//   }
// 
//   bool	x = dummy();
// }


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

void RRQSound::process( SNDLIST type )
{
  if( type >= NO_SOUND || !isOK() )
    return;

  if( d->sounds[ type ] )
    {
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


void RRQSound::stop( SNDLIST type )
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
  d->sounds = vector<QSound *>( NO_SOUND );
  loadSounds();
}


void RRQSound::loadSounds()
{
  int		i;
  string	fnbase;
  string	filename;

  fnbase = qRollSharePath() + "/sounds/";

  for( i=0; i<NO_SOUND; ++i )
    if( !d->sounds[i] )
      {
	filename = fnbase + _sndFile[ i ] + ".wav";
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


