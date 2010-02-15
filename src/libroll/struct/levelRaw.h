/***************************************************************************
                          levelRaw.h  -  description
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


#ifndef ROLL_STRUCT_LEVELRAW_H
#define ROLL_STRUCT_LEVELRAW_H

#include <roll/struct/level.h>
#include <roll/struct/levelParam.h>
#include <vector>
#include <iostream>


namespace roll
{
  struct AtariFormatInfo;
  struct SimpleLevel;

  class LevelRaw : public Level, public LevelParam
  {
  public:
    LevelRaw();
    LevelRaw( unsigned sx, unsigned sy );
    virtual ~LevelRaw();

    bool load( std::istream & );
    bool loadAtari( std::istream &, AtariFormatInfo & );
    bool save( std::ostream & );
    bool saveAtari( std::ostream &, AtariFormatInfo & );

    unsigned size() const;
    virtual void resize( unsigned sx, unsigned sy );

    void unpack( SimpleLevel & sl );
    bool pack( const SimpleLevel & sl );

    virtual unsigned short diams() const { return( _diams ); }
    virtual void setDiams( unsigned short x ) { _diams = x; }
    virtual unsigned short time() const { return( _time ); }
    virtual void setTime( unsigned short x ) { _time = x; }
    virtual unsigned char permeability() const { return( _permeability ); }
    virtual void setPermeability( unsigned char x ) { _permeability = x; }
    virtual unsigned char speed() const { return( _speed ); }
    virtual void setSpeed( unsigned char x ) { _speed = x; }
    virtual unsigned char strength() const { return( _strength ); }
    virtual void setStrength( unsigned char x ) { _strength = x; }
    virtual unsigned char genSpeed() const { return( _genspeed ); }
    virtual void setGenSpeed( unsigned char x ) { _genspeed = x; }
    virtual unsigned short blobMaxSize() const { return( _blobmaxsize ); }
    virtual void setBlobMaxSize( unsigned short x ) { _blobmaxsize = x; }
    virtual unsigned short bombDelay() const { return( _bombdelay ); }
    virtual void setBombDelay( unsigned short x ) { _bombdelay = x; }
    virtual unsigned short wallColor() const { return( _colwall ); }
    virtual void setWallColor( unsigned short x ) { _colwall = x; }
    virtual unsigned short diamColor() const { return( _coldiam ); }
    virtual void setDiamColor( unsigned short x ) { _coldiam =  x; }
    virtual unsigned short rollColor() const { return( _colroll ); }
    virtual void setRollColor( unsigned short x ) { _colroll = x; }
    virtual unsigned short leftMonsterColor() const
    { return( _colleftmonster ); }
    virtual void setLeftMonsterColor( unsigned short x )
    { _colleftmonster = x; }
    virtual unsigned short grassColor() const { return( _colgrass ); }
    virtual void setGrassColor( unsigned short x ) { _colgrass = x; }
    virtual unsigned short flags() const { return( _flags ); }
    virtual void setFlags( unsigned short x ) { _flags = x; }

  protected:
    std::vector<ParamBlock>	_params;
    std::vector<unsigned short>	_data;
  };

}


#endif


