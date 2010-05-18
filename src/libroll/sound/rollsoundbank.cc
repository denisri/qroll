/***************************************************************************
                          rollSoundBank.h
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

#include "rollsoundbank.h"
#include <roll/struct/general.h>

using namespace soma;
using namespace roll;
using namespace std;


RollSoundBank::RollSoundBank()
  : SoundBank()
{
}


RollSoundBank::~RollSoundBank()
{
}


void RollSoundBank::init()
{
  string fnbase = roll::qRollSharePath() + "/sounds/";
  string filename;

  vector<string> sf;
  sf.reserve( NO_SOUND );
  sf.push_back( "ball" );
  sf.push_back( "blob" );
  sf.push_back( "bomb" );
  sf.push_back( "bonuslife" );
  sf.push_back( "conveylever" );
  sf.push_back( "conveyorbelt" );
  sf.push_back( "diam" );
  sf.push_back( "door" );
  sf.push_back( "explo" );
  sf.push_back( "grass" );
  sf.push_back( "hungry" );
  sf.push_back( "key" );
  sf.push_back( "key_fall" );
  sf.push_back( "lift" );
  sf.push_back( "lock" );
  sf.push_back( "rock" );
  sf.push_back( "roll_push" );
  sf.push_back( "roll_yawn" );
  sf.push_back( "spring" );
  sf.push_back( "strength" );
  sf.push_back( "time" );
  sf.push_back( "dong1" );
  sf.push_back( "dong2" );
  sf.push_back( "dong3" );
  sf.push_back( "dong4" );
  sf.push_back( "dong5" );
  sf.push_back( "dong6" );
  sf.push_back( "dong7" );
  sf.push_back( "dong8" );
  sf.push_back( "dong9" );

  unsigned i;
  for( i=0; i<NO_SOUND; ++i )
  {
    filename = fnbase + sf[ i ] + ".wav";
    addSound( filename, true );
  }
}


