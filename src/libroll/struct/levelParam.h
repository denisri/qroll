/***************************************************************************
                          levelParam.h  -  description
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


#ifndef QROLL_STRUCT_LEVELPARAM_H
#define QROLL_STRUCT_LEVELPARAM_H

#include <vector>


namespace roll
{

  /**	Level header parameters
   */
  struct LevelParam
  {
    enum ParamCode
      {
	Explosions = 3, 
	Blobs = 4
      };

    typedef std::pair<ParamCode, std::vector<unsigned short> > ParamBlock;

    LevelParam( unsigned sx = 0, unsigned sy = 0 );
    LevelParam( const LevelParam & x );

    LevelParam & operator = ( const LevelParam & );

    unsigned sizeX() const { return( _sx ); }
    unsigned sizeY() const { return( _sy ); }

    ///	Needed diams
    unsigned short	_diams;
    unsigned short	_time;
    ///	Walls permebility
    unsigned char	_permeability;
    ///	Blobs growing speed
    unsigned char	_speed;
    ///	Roll strength
    unsigned char	_strength;
    ///	Generators speed
    unsigned char	_genspeed;
    ///	General blobs max. size before transformation
    unsigned short	_blobmaxsize;
    ///	Bombs delay before blowing
    unsigned short	_bombdelay;
    ///	Walls color (normlally gray)
    unsigned short	_colwall;
    ///	Diams color (normally pink)
    unsigned short	_coldiam;
    ///	Roll color (normally blue)
    unsigned short	_colroll;
    ///	Left monsters color (normally red)
    unsigned short	_colleftmonster;
    ///	Grass color (normally green)
    unsigned short	_colgrass;
    ///	Level-specific game flags
    unsigned short	_flags;

  protected:
    unsigned	_sx;
    unsigned	_sy;
  };


  inline 
  LevelParam::LevelParam( unsigned sx, unsigned sy )
    : _diams( 50 ), _time( 100 ), _permeability( 50 ), _speed( 50 ), 
      _strength( 50 ), _genspeed( 50 ), _blobmaxsize( 256 ), _bombdelay( 2 ), 
      _colwall( 0 ), _coldiam( 0 ), _colroll( 0 ), _colleftmonster( 0 ), 
      _colgrass( 0 ), _flags( 0 ), _sx( sx ), _sy( sy )
  {
  }

  inline 
  LevelParam::LevelParam( const LevelParam & x )
    : _diams( x._diams ), _time( x._time ), _permeability( x._permeability ), 
      _speed( x._speed ), _strength( x._strength ), _genspeed( x._genspeed ), 
      _blobmaxsize( x._blobmaxsize ), _bombdelay( x._bombdelay ), 
      _colwall( x._colwall ), _coldiam( x._coldiam ), _colroll( x._colroll ), 
      _colleftmonster( x._colleftmonster ), _colgrass( x._colgrass ), 
      _flags( x._flags ), _sx( x._sx ), _sy( x._sy )
  {
  }

  inline 
  LevelParam & LevelParam::operator = ( const LevelParam & x )
  {
    if( this != &x )
      {
	_diams= x._diams;
	_time = x._time;
	_permeability = x._permeability;
	_speed = x._speed;
	_strength = x._strength;
	_genspeed = x._genspeed;
	_blobmaxsize = x._blobmaxsize;
	_bombdelay = x._bombdelay;
	_colwall = x._colwall;
	_coldiam = x._coldiam;
	_colroll = x._colroll;
	_colleftmonster = x._colleftmonster;
	_colgrass = x._colgrass;
	_flags = x._flags;
	_sx = x._sx;
	_sy = x._sy;
      }
    return( *this );
  }

}

#endif


