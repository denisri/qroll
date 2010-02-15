/***************************************************************************
                          telepWall.h  -  description
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


#ifndef ROLL_GAME_TELEPWALL_H
#define ROLL_GAME_TELEPWALL_H


#include <roll/game/transformWall.h>


namespace roll
{

  /**	Mur téléporteur */
  class RTelepWall : public RPassingWall
  {
  public:
    RTelepWall( unsigned num ) : RPassingWall( num ) {}
    virtual ~RTelepWall() {}

  protected:
    virtual bool testGoOut( unsigned x, unsigned y );
    virtual void goOut( unsigned x, unsigned y );
  };


  /**	Mur tépélorto-transformeur */
  class RTelepTransWall : public RTransformWall
  {
  public:
    RTelepTransWall( unsigned num ) : RTransformWall( num ) {}
    virtual ~RTelepTransWall() {}

  protected:
    virtual bool testGoOut( unsigned x, unsigned y );
    virtual void goOut( unsigned x, unsigned y );
  };


  /**	Mur tépélorto-filtreur (à attente) */
  class RTelepPermeabWall : public RTelepWall
  {
  public:
    RTelepPermeabWall( unsigned num ) : RTelepWall( num ) {}
    virtual ~RTelepPermeabWall() {}

  protected:
    virtual bool testGoOut( unsigned x, unsigned y );
  };

}

#endif


