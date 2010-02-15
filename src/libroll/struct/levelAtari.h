/***************************************************************************
                          levelAtari.h  -  description
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


#ifndef ROLL_STRUCT_LEVELATARI_H
#define ROLL_STRUCT_LEVELATARI_H


#include <roll/struct/level.h>
#include <roll/struct/levelPar.h>
#include <fstream>


namespace roll
{

  /**	R&R level (file / storage format), old style Atari ST format
   */
  class LevelAtari : public Level, public LevelPar
  {
  public:
    LevelAtari();
    virtual ~LevelAtari();

    void load( std::ifstream & fich );

    virtual void resize( unsigned sx, unsigned sy );
    virtual unsigned short diams() const { return( diam ); }
    virtual void setDiams( unsigned short x ) { diam = x; }
    virtual unsigned short time() const { return( tps ); }
    virtual void setTime( unsigned short x ) { tps = x; }
    virtual unsigned char permeability() const { return( perm ); }
    virtual void setPermeability( unsigned char x ) { perm = x; }
    virtual unsigned char speed() const { return( vit ); }
    virtual void setSpeed( unsigned char x ) { vit = x; }
    virtual unsigned char strength() const { return( forc ); }
    virtual void setStrength( unsigned char x ) { forc = x; }
    virtual unsigned char genSpeed() const { return( vgen ); }
    virtual void setGenSpeed( unsigned char x ) { vgen = x; }
    virtual unsigned short blobMaxSize() const { return( tbeb ); }
    virtual void setBlobMaxSize( unsigned short x ) { tbeb = x; }
    virtual unsigned short bombDelay() const { return( dbom ); }
    virtual void setBombDelay( unsigned short x ) { dbom = x; }
    virtual unsigned short wallColor() const { return( cm ); }
    virtual void setWallColor( unsigned short x ) { cm = x; }
    virtual unsigned short diamColor() const { return( cd ); }
    virtual void setDiamColor( unsigned short x ) { cd =  x; }
    virtual unsigned short rollColor() const { return( cr ); }
    virtual void setRollColor( unsigned short x ) { cr = x; }
    virtual unsigned short leftMonsterColor() const { return( cm1 ); }
    virtual void setLeftMonsterColor( unsigned short x ) { cm1 = x; }
    virtual unsigned short grassColor() const { return( ch ); }
    virtual void setGrassColor( unsigned short x ) { ch = x; }
    virtual unsigned short flags() const { return( LevelPar::flags ); }
    virtual void setFlags( unsigned short x ) { LevelPar::flags = x; }

    ///	Taille du tableau
    unsigned long	size;
    ///	Taille des paramètres
    unsigned short	sparam;
    ///	Paramètres
    unsigned char	*param;
    ///	Données tableau
    unsigned char	*data;

  protected:

  private:
  };

}


#endif


