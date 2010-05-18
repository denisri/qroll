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


#ifndef SOMA_SOMASOUND_ROLLSOUNDBANK_H
#define SOMA_SOMASOUND_ROLLSOUNDBANK_H

#include "../somasound/soundbank.h"

namespace roll
{

  class RollSoundBank : public soma::SoundBank
  {
  public:
    enum SNDLIST
    {
      BALL,
      BLOB,
      BOMB,
      BONUSLIFE,
      CONVEYLEVER,
      CONVEYORBELT,
      DIAM,
      DOOR,
      EXPLO,
      GRASS,
      HUNGRY,
      KEY,
      KEY_FALL,
      LIFT,
      LOCK,
      ROCK,
      ROLL_PUSH,
      ROLL_YAWN,
      SPRING,
      STRENGTH,
      TIME,
      DONG1,
      DONG2,
      DONG3,
      DONG4,
      DONG5,
      DONG6,
      DONG7,
      DONG8,
      DONG9,
      NO_SOUND
    };

    RollSoundBank();
    virtual ~RollSoundBank();

    virtual void init();
  };

}

#endif

