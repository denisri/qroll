/***************************************************************************
                          level.h  -  description
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


#ifndef ROLL_STRUCT_LEVEL_H
#define ROLL_STRUCT_LEVEL_H


namespace roll
{

  /**	R&R common level API
   */
  class Level
  {
  public:
    Level();
    virtual ~Level();

    virtual void resize( unsigned sx, unsigned sy ) = 0;
    virtual unsigned short diams() const = 0;
    virtual void setDiams( unsigned short x ) = 0;
    virtual unsigned short time() const = 0;
    virtual void setTime( unsigned short x ) = 0;
    virtual unsigned char permeability() const = 0;
    virtual void setPermeability( unsigned char x ) = 0;
    virtual unsigned char speed() const = 0;
    virtual void setSpeed( unsigned char x ) = 0;
    virtual unsigned char strength() const = 0;
    virtual void setStrength( unsigned char x ) = 0;
    virtual unsigned char genSpeed() const = 0;
    virtual void setGenSpeed( unsigned char x ) = 0;
    virtual unsigned short blobMaxSize() const = 0;
    virtual void setBlobMaxSize( unsigned short x ) = 0;
    virtual unsigned short bombDelay() const = 0;
    virtual void setBombDelay( unsigned short x ) = 0;
    virtual unsigned short wallColor() const = 0;
    virtual void setWallColor( unsigned short x ) = 0;
    virtual unsigned short diamColor() const = 0;
    virtual void setDiamColor( unsigned short x ) = 0;
    virtual unsigned short rollColor() const = 0;
    virtual void setRollColor( unsigned short x ) = 0;
    virtual unsigned short leftMonsterColor() const = 0;
    virtual void setLeftMonsterColor( unsigned short x ) = 0;
    virtual unsigned short grassColor() const = 0;
    virtual void setGrassColor( unsigned short x ) = 0;
    virtual unsigned short flags() const = 0;
    virtual void setFlags( unsigned short x ) = 0;

  protected:

  private:
  };

}


#endif


