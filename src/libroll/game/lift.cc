/***************************************************************************
                          lift.cc  -  description
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


#include <roll/game/lift.h>
#include <roll/game/vars.h>
#include <roll/sound/soundProcessor.h>

using namespace roll;
using namespace std;


void RLift::realProcess( unsigned x, unsigned y )
{
  GElem	*&up = game.tbct.d[x][y-1];
  int	olddir = _dir;

  f &= ~( HAU | BA );

  if( up->canMove() )	// ascenseur chargÃ©
    {
      if( !_loaded )	// vient d'Ãªtre chargÃ©
	_dir = 0;	// on repart Ã  zÃ©ro

      _loaded = true;

      switch( _dir )
	{
	case 0:	// immobile
	  if( game.tbct.d[x][y-2]->isEmpty() )
	    _dir = -1;	// se met en marche vers le haut
	  else if( game.tbct.d[x][y+1]->isEmpty() )
	    _dir = 1;	// vers le bas
	  break;
	case 1:
	  if( game.tbct.d[x][y+1]->isEmpty() )
	    moveDown( x, y );
	  break;
	case -1:
	  if( game.tbct.d[x][y-2]->isEmpty() )
	    moveUp( x, y );
	  break;
	default:
	  cerr << "RLift in unknown direction\n";
	  _dir = 0;
	}
    }
  else if( !up->isTemporary() )
    {
      bool oldload = _loaded;

      _loaded = false;

      if( !oldload && _dir == 0 )
	return;

      if( oldload )
	_dir = 0;

      stop( x, y );	// plus chargÃ©
    }

  if( _dir != olddir )
    RRSoundProcessor::processor().process( RollSoundBank::LIFT );
}


void RLift::stop( unsigned, unsigned )
{
  _dir = 0;
}


void RLift::moveDown( unsigned x, unsigned y )
{
  GElem *&down = game.tbct.d[x][y+1];
  GElem *&up = game.tbct.d[x][y-1];

  delete down;
  down = this;
  up-> f |= BA;
  game.tbct.d[x][y] = up;
  up = new EmptyElem;
  f |= BUSY | BA;
  _dir = 1;
}


void RLift::moveUp( unsigned x, unsigned y )
{
  GElem *&up = game.tbct.d[x][y-1];
  GElem *&up2 = game.tbct.d[x][y-2];

  delete up2;
  up->f |= HAU;
  up2 = up;
  up = this;
  game.tbct.d[x][y] = new EmptyElem;
  f |= HAU;
  _dir = -1;
}


void RAutoLift::stop( unsigned x, unsigned y )
{
  GElem	*&down = game.tbct.d[x][y+1];
  GElem	*&up = game.tbct.d[x][y-1];

  switch( _dir )
    {
    case 0:
      if( down->isEmpty() )	// peut descendre
	{
	  delete down;
	  down = this;
	  game.tbct.d[x][y] = new EmptyElem;
	  f |= BUSY | BA;
	  _dir = 1;
	}
      else if( !down->isTemporary() )
      {
	if( up->isEmpty() && game.tbct.d[x][y-2]->isEmpty() )	// peut monter
	  moveUp( x, y );
	else	// non: on s'arrÃªte vraiment
	  _dir = 0;
      }
      break;
    case -1:
      if( up->isEmpty() && game.tbct.d[x][y-2]->isEmpty() )	// peut monter
	moveUp( x, y );
      break;
    case 1:
      if( down->isEmpty() )	// peut descendre
	{
	  delete down;
	  down = this;
	  game.tbct.d[x][y] = new EmptyElem;
	  f |= BUSY | BA;
	  _dir = 1;
	}
      else if( !down->isTemporary() )
	_dir = 0;
      break;
    }
}








