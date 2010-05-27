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


#include "somasoundprocessor.h"
#include "soundbank.h"
#include "soundslot.h"
#include <iostream>
#include <list>

using namespace soma;
using namespace std;


SomaSoundProcessor::SomaSoundProcessor()
  : _sounds( 0 )
{
  registerProcessor( this );
}


SomaSoundProcessor::~SomaSoundProcessor()
{
  unregisterProcessor( this );
  delete _sounds;
}


void SomaSoundProcessor::init()
{
}


void SomaSoundProcessor::process( int )
{
}


void SomaSoundProcessor::processIfNotUsed( int type )
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

    typedef list<SomaSoundProcessor *>::iterator	iterator;

    SomaSoundProcessor		*processor;
    list<SomaSoundProcessor *>	allproc;
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


void SomaSoundProcessor::registerProcessor( SomaSoundProcessor* p )
{
  StaticSound	& s = staticSound();
  s.allproc.push_back( p );
  if( s.processor && p->priorityRating() >= s.processor->priorityRating() )
    {
      s.processor->close();
      s.processor = 0;
    }
}


void SomaSoundProcessor::unregisterProcessor( SomaSoundProcessor* p )
{
  StaticSound	& s = staticSound();
  StaticSound::iterator	i, n = s.allproc.end();

  if( s.processor == p )
    s.processor = 0;

  for( i=s.allproc.begin(); i!=n && *i != p; ++i );
  if( i != n )
    s.allproc.erase( i );
}


SomaSoundProcessor & SomaSoundProcessor::processor()
{
  StaticSound	& s = staticSound();
  if( s.processor == 0 )
    {
      StaticSound::iterator	i, n = s.allproc.end(), j = n;
      SomaSoundProcessor		*lastok = 0;
      for( i=s.allproc.begin(); i!=n; ++i )
        if( (*i)->isOK() )
          if( !lastok || (*i)->priorityRating() >= lastok->priorityRating() )
            lastok = *i;
      if( !lastok )
        s.processor = new SomaSoundProcessor;
      else
        s.processor = lastok;
      cout << "Sound processor : " << s.processor->name() << endl;
    }
  return *s.processor;
}


void SomaSoundProcessor::setSoundBank( SoundBank* sb )
{
  if( _sounds )
    delete _sounds;
  _sounds = sb;
}


SoundBank & SomaSoundProcessor::soundBank()
{
  if( !_sounds )
    _sounds = new SoundBank;
  return *_sounds;
}

