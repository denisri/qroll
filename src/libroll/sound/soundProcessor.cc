/***************************************************************************
                          soundProcessor.cc  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000 by Denis RiviÃ¨re
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


#include "soundProcessor.h"
#include "soundslot.h"
#include <iostream>
#include <list>

using namespace soma;
using namespace std;


RRSoundProcessor::RRSoundProcessor()
  : _sounds( 0 )
{
  registerProcessor( this );
}


RRSoundProcessor::~RRSoundProcessor()
{
  unregisterProcessor( this );
  delete _sounds;
}


void RRSoundProcessor::init()
{
}


void RRSoundProcessor::process( int )
{
}


void RRSoundProcessor::processIfNotUsed( int type )
{
  if( (unsigned) type < soundBank().sounds().size() && !inuse( type ) )
    process( type );
}


namespace
{

  struct StaticSound
  {
    StaticSound();
    ~StaticSound();

    typedef list<RRSoundProcessor *>::iterator	iterator;

    RRSoundProcessor		*processor;
    list<RRSoundProcessor *>	allproc;
  };


  StaticSound & staticSound()
  {
    static StaticSound	s;
    return s;
  }


  StaticSound::StaticSound()
    : processor( 0 )
  {
  }


  StaticSound::~StaticSound()
  {
    while( !allproc.empty() )
      delete allproc.front();
  }

}


void RRSoundProcessor::registerProcessor( RRSoundProcessor* p )
{
  StaticSound	& s = staticSound();
  s.allproc.push_back( p );
  if( s.processor && p->priorityRating() >= s.processor->priorityRating() )
    {
      s.processor->close();
      s.processor = 0;
    }
}


void RRSoundProcessor::unregisterProcessor( RRSoundProcessor* p )
{
  StaticSound	& s = staticSound();
  StaticSound::iterator	i, n = s.allproc.end();

  if( s.processor == p )
    s.processor = 0;

  for( i=s.allproc.begin(); i!=n && *i != p; ++i );
  if( i != n )
    s.allproc.erase( i );
}


RRSoundProcessor & RRSoundProcessor::processor()
{
  StaticSound	& s = staticSound();
  if( s.processor == 0 )
    {
      StaticSound::iterator	i, n = s.allproc.end(), j = n;
      RRSoundProcessor		*lastok = 0;
      for( i=s.allproc.begin(); i!=n; ++i )
        if( (*i)->isOK() )
          if( !lastok || (*i)->priorityRating() >= lastok->priorityRating() )
            lastok = *i;
      if( !lastok )
        s.processor = new RRSoundProcessor;
      else
        s.processor = lastok;
      cout << "Sound processor : " << s.processor->name() << endl;
    }
  return *s.processor;
}


void RRSoundProcessor::setSoundBank( SoundBank* sb )
{
  if( _sounds )
    delete _sounds;
  _sounds = sb;
}


SoundBank & RRSoundProcessor::soundBank()
{
  if( !_sounds )
    _sounds = new SoundBank;
  return *_sounds;
}

