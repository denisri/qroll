/***************************************************************************
                          game.h  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000-2001 by Denis Rivière
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


#ifndef ROLL_GAME_GAME_H
#define ROLL_GAME_GAME_H


#include <stdlib.h>
#include <roll/struct/workLevel.h>
#include <map>


namespace roll
{

  /**	Rock'N'Roll game engine */
  class Game
  {
  public :
    enum KeyCode
    {
      K_NONE, 
      K_LEFT, 
      K_RIGHT, 
      K_UP, 
      K_DOWN, 
      K_BOMB, 
      K_ABORT, 
      /// change bomb type to launch
      K_CHANGEBOMB, 
      K_FIRE
    };

    Game();
    ~Game();

    void init( unsigned tab );
    void run( unsigned tab );
    void step();

    WorkLevel		tbct;
    ///	Current level number
    int			tb;
    ///	Game ended ?
    bool		fin;
    ///	Is there still anyone in the current level ?
    bool		mec;
    ///	Latency time betwin win/lose event and the level game end
    unsigned short	twon;
    ///	Is the game running or not ?
    bool		running;
    ///	Pause flag
    bool		paused;
    ///	Game changed ? (reinitialized)
    bool		changed;
    ///	When a new life has just been won
    int			lifeGained;
    bool		timeChanged;
    /** If true, all Rolls must go through a door before the level is won.
	If false, the level ends when the first Roll gets to a door */
    bool		allRollsMustExit;
  };

}

#endif



