/***************************************************************************
                          lift.h  -  description
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


#ifndef ROLL_GAME_LIFT_H
#define ROLL_GAME_LIFT_H


#include <roll/game/gElem.h>


namespace roll
{

  /**	Ascenseur */
  class RLift : public GElem
  {
  public:
    RLift( unsigned num = 0x1e6 ) 
      : GElem( num ), _dir( 0 ), _loaded( false ) {}
    virtual ~RLift() {}

    int direction() const { return( _dir ); }
    virtual void stop( unsigned x, unsigned y );
    virtual void moveUp( unsigned x, unsigned y );
    virtual void moveDown( unsigned x, unsigned y );

  protected:
    virtual void realProcess( unsigned x, unsigned y );

    ///	Direction du mouvement (0: immobile, -1: haut, 1: bas)
    int	_dir;
    ///	chargé ?
    bool	_loaded;
  };



  /**	Ascenseur "automatique" (qui redescend quand il est vide) */
  class RAutoLift : public RLift
  {
  public:
    RAutoLift( unsigned num = 0x1be ) : RLift( num ) {}
    virtual ~RAutoLift() {}

    virtual void stop( unsigned x, unsigned y );
  protected:
  };

}

#endif

