/***************************************************************************
                          player.cc  -  description
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


#include <roll/game/player.h>

using namespace roll;


Player::Player() : score( 0 ), lives( 3 ), win( false ), dead( false ), 
  latency( 0 ), key( -1 ), kbd( 0 ), fire( false ), 
  bombToLaunch( 0 ), launchedBomb( -1 ), pos( Coord( 0, 0 ) ), 
  ball( false ), network( 0 )
{
}


void Player::init()
{
  win = false;
  dead = false;
  latency = 0;
  key = -1;
  bombs.erase( bombs.begin(), bombs.end() );
  kbd = 0;
  fire = false;
  bombToLaunch = 0;
  launchedBomb = -1;
  ball = false;
  //pos = (Coord) { 0, 0 };
}


void Player::reset()
{
  score = 0;
  lives = 3;
}


Player & Player::operator = ( const Player & p )
{
  if( this == &p )
    return( *this );

  score = p.score;
  lives = p.lives;
  win = p.win;
  dead = p.dead;
  latency = p.latency;
  key = p.key;
  bombs = p.bombs;
  kbd = p.kbd;
  fire = p.fire;
  bombToLaunch = p.bombToLaunch;
  launchedBomb = p.launchedBomb;
  pos = p.pos;
  ball = p.ball;
  network = p.network;

  return( *this );
}


