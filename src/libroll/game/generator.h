/***************************************************************************
                          generator.h  -  description
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


#ifndef ROLL_GAME_GENERATOR_H
#define ROLL_GAME_GENERATOR_H


#include <roll/game/foregroundElem.h>


namespace roll
{

  /**	Générateur de monstres */
  class RGenerator : public RForegroundElem
  {
  public:
    RGenerator( unsigned num = 32 ) : RForegroundElem( num ) {}
    virtual ~RGenerator() {}

    virtual bool canBeInBorder() const { return( true ); }
  protected:
    ///
    virtual void realProcess( unsigned x, unsigned y );
    ///
    virtual void tryCreate( unsigned x, unsigned y, unsigned flags );
    ///
    virtual GElem* create();
  };



  /**	Générateur de monstres 2	*/
  class RGenerator2 : public RGenerator
  {
  public:
    /**@name	Constructeur - destructeur */
    //@{
    ///
    RGenerator2( unsigned num = 72 ) : RGenerator( num ) {}
    ///
    virtual ~RGenerator2() {}
    //@}

  protected:
    ///
    virtual GElem* create();
  };


  /**	Générateur de monstres 3	*/
  class RGenerator3 : public RGenerator
  {
  public:
    /**@name	Constructeur - destructeur */
    //@{
    ///
    RGenerator3( unsigned num = 112 ) : RGenerator( num ) {}
    ///
    virtual ~RGenerator3() {}
    //@}

  protected:
    ///
    virtual GElem* create();
  };


  /**	Générateur de monstres 4	*/
  class RGenerator4 : public RGenerator
  {
  public:
    /**@name	Constructeur - destructeur */
    //@{
    ///
    RGenerator4( unsigned num = 152 ) : RGenerator( num ) {}
    ///
    virtual ~RGenerator4() {}
    //@}

  protected:
    ///
    virtual GElem* create();
  };

}

#endif


