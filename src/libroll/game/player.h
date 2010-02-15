/***************************************************************************
                          player.h  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000 by Denis Rivière
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


#ifndef ROLL_GAME_PLAYER_H
#define ROLL_GAME_PLAYER_H


#include <map>

namespace roll
{

  ///	Coordinates
  struct Coord
  {
    Coord( unsigned x0 = 0, unsigned y0 = 0 ) : x( x0 ), y( y0 ) {}
    unsigned	x;
    unsigned	y;
  };


  /**	Holds all needed info about a player */
  struct Player
  {
    Player();
    Player & operator = ( const Player & p );

    ///	Initialise pour un nouveau tableau (ne touche pas aux vie, score...)
    void init();
    /**	Initialise pour une nouvelle partie (vie, score...). 
	Ne remplace pas init(), il faut le faire en plus */
    void reset();

    unsigned				score;
    unsigned				lives;
    bool				win;
    bool				dead;
    ///	Comptage des temps morts
    unsigned				latency;
    ///	Clef (-1: pas de clef)
    int					key;
    /**	Bombes transportées, par type.
	map: type -> nombre de bombes de ce type
    */
    std::map<unsigned, unsigned>	bombs;
    ///	Touche courante
    unsigned				kbd;
    ///	Bouton de tir
    bool				fire;
    ///	No de bombe à déclencher à la prochaine pause de bombe
    unsigned				bombToLaunch;
    ///	Bombe déclenchée en attente d'être lancée (quand le Roll bougera).
    int					launchedBomb;
    ///	Position dans le tableau
    Coord				pos;
    ///	Balle en stock ?
    bool				ball;
    ///	Network connection number (origin of this player), 0 = local
    unsigned				network;
  };

}


#endif


