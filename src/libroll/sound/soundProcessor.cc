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


#include <roll/sound/soundProcessor.h>
#include <iostream>
#include <list>

using namespace std;


RRSoundProcessor::RRSoundProcessor()
{
  registerProcessor( this );
}


RRSoundProcessor::~RRSoundProcessor()
{
  unregisterProcessor( this );
}


void RRSoundProcessor::init()
{
  _sndFile[ BALL         ] = "ball";
  _sndFile[ BLOB         ] = "blob";
  _sndFile[ BOMB         ] = "bomb";
  _sndFile[ BONUSLIFE    ] = "bonuslife";
  _sndFile[ CONVEYLEVER  ] = "conveylever";
  _sndFile[ CONVEYORBELT ] = "conveyorbelt";
  _sndFile[ DIAM         ] = "diam";
  _sndFile[ DOOR         ] = "door";
  _sndFile[ EXPLO        ] = "explo";
  _sndFile[ GRASS        ] = "grass";
  _sndFile[ HUNGRY       ] = "hungry";
  _sndFile[ KEY          ] = "key";
  _sndFile[ KEY_FALL     ] = "key_fall";
  _sndFile[ LIFT         ] = "lift";
  _sndFile[ LOCK         ] = "lock";
  _sndFile[ ROCK         ] = "rock";
  _sndFile[ ROLL_PUSH    ] = "roll_push";
  _sndFile[ ROLL_YAWN    ] = "roll_yawn";
  _sndFile[ SPRING       ] = "spring";
  _sndFile[ STRENGTH     ] = "strength";
  _sndFile[ TIME         ] = "time";
  _sndFile[ DONG1        ] = "dong1";
  _sndFile[ DONG2        ] = "dong2";
  _sndFile[ DONG3        ] = "dong3";
  _sndFile[ DONG4        ] = "dong4";
  _sndFile[ DONG5        ] = "dong5";
  _sndFile[ DONG6        ] = "dong6";
  _sndFile[ DONG7        ] = "dong7";
  _sndFile[ DONG8        ] = "dong8";
  _sndFile[ DONG9        ] = "dong9";

  for( int i=0; i<NO_SOUND; ++i )
    _inuse[i] = 0;
}


void RRSoundProcessor::process( SNDLIST )
{
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


