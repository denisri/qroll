/***************************************************************************
                          workLevel.h  -  description
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


#ifndef ROLL_STRUCT_WORKLEVEL_H
#define ROLL_STRUCT_WORKLEVEL_H


#include <roll/struct/levelParam.h>
#include <roll/struct/level.h>
#include <roll/game/player.h>
#include <vector>


namespace roll
{
  class PlayerServer;
  struct SimpleLevel;
  class GElem;
  class RBack;
  class PlayerServer;
  class Series;
  class Level;

  /**	Tableau courant
   */
  class WorkLevel : public Level, public LevelParam
  {
  public:
    /**	Codage des couleurs. 
	Les RGB sont signés pour pouvoir coder des différences */
    struct RGB
    {
      RGB() {}
      RGB( short red, short grn, short ble ) 
	: r( red ), g( grn ), b( ble ) {}
      short r;
      short g;
      short b;
    };

    ///	Indices des types de couleurs
    enum PartColor
    {
      ///	Noir / blanc
      UNCHANGED, 
      ROCK,
      DIAM,
      ROLL,
      MONSTER, 
      GRASS
    };

    WorkLevel();
    virtual ~WorkLevel();

    virtual void resize( unsigned sx, unsigned sy );
    virtual unsigned short diams() const { return( _diams ); }
    virtual void setDiams( unsigned short x ) { _diams = x; }
    virtual unsigned short time() const { return( _time ); }
    virtual void setTime( unsigned short x ) { _time = x; }
    virtual unsigned char permeability() const { return( _permeability ); }
    virtual void setPermeability( unsigned char x ) { _permeability = x; }
    virtual void setConvertedPermeability( unsigned char x );
    virtual unsigned char speed() const { return( _speed ); }
    virtual void setSpeed( unsigned char x ) { _speed = x; }
    virtual void setConvertedSpeed( unsigned char x );
    virtual unsigned char strength() const { return( _strength ); }
    virtual void setStrength( unsigned char x ) { _strength = x; }
    virtual void setConvertedStrength( unsigned char x );
    virtual unsigned char genSpeed() const { return( _genspeed ); }
    virtual void setGenSpeed( unsigned char x ) { _genspeed = x; }
    virtual void setConvertedGenSpeed( unsigned char x );
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

    ///	Conversion series level -> WorkLevel
    void make( SimpleLevel & sl );
    /** Convert to SimpleLevel format
	\param maxLayers nb max. of wanted layers in the converted level. 
	SimpleLevel actually handles 2 right now (foreground and background) 
	but this may be extended. If \c maxLayers is -1 (default), unlimited 
	number of layers is assumed.
	\param wantFlags If true, the level will try to keep each element state
	(moving elements directions, etc) in the special \c flags layer of 
	SimpleLevel. In fact it will be helpful in the future but it isn't 
	supported yet: this parameter is always discarded right now.
    */
    bool decode( SimpleLevel & sl, int maxLayers = -1, 
		 bool wantFlags = false ) const;
    ///	Couleurs des éléments
    RGB color( PartColor index ) { return( _colors[ index ] ); }
    void setColor( PartColor index, const RGB col );
    ///	Convertion code couleur du tableau disque -> RGB
    static RGB convertedColor( unsigned short code );
    ///	Remet les scores des joueurs à 0, les vies à fond etc
    void resetScores();
    bool modified() const { return( _modified ); }
    void setModified( bool x ) { _modified = x; }

    ///	Pointeur sur tableau stocké
    Level			*tab;
    ///	Level data: 2D array [][] of element pointers
    GElem			***d;
    ///	Level background data elements
    RBack			***b;
    ///	Coordonnées de l'arrivée de téléporteur
    unsigned			x_tel;
    unsigned			y_tel;
    ///	Directions des tapis roulants
    std::vector<int>		conveyDir;
    ///	Départs de Rolls
    std::vector<Coord>		startPos;
    ///	Joueurs figés (temps fini)
    bool			frozen;
    ///	Portes ouvertes ?
    bool			doorOpen;
    ///	Infos joueurs
    PlayerServer		*players;
    ///	Dynamites explosent ce tour-là ? 0: non, 2: préparation, 1: explose
    unsigned     		explodeDyna;
    /// variable parameters like in SimpleLevel
    std::vector<ParamBlock>	params;

  private:
    ///	Allocation des données
    void allocData( unsigned dimX, unsigned dimY );
    ///	Désallocation des données
    void deleteData();

    ///	Couleurs
    RGB		_colors[6];
    bool	_modified;
  };

}


#endif


