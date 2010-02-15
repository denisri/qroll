/***************************************************************************
                          seriesAtari.h  -  description
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


#ifndef ROLL_STRUCT_SERIESATARI_H
#define ROLL_STRUCT_SERIESATARI_H


#include <roll/struct/series.h>


namespace roll
{
  class LevelAtari;

  /**	Levels seris, format used since 1987 on Atari ST version of Rock'n'Roll
   */
  class SeriesAtari : public Series
  {
  public :
    SeriesAtari();
    SeriesAtari( const std::string & name );
    virtual ~SeriesAtari();

    virtual const Level* level( unsigned x ) const;
    virtual Level* level( unsigned x );
    virtual void size( unsigned level, unsigned & sx, unsigned & sy ) const;
    virtual bool load( const std::string & name );
    virtual unsigned numLevels() const { return( nt ); }
    virtual bool pack( const SimpleLevel &, unsigned level );
    virtual void unpack( unsigned level, SimpleLevel & ) const;

  private:
    void convList();

    // (later...) protected:
  public:

    ///	Nb de diams différents
    unsigned short	diam;
    ///	Nb de sprites qui explosent
    unsigned short	expl;
    ///	Nb de bébettes
    unsigned short	beb;
    ///	Valeur de la série (nb d'étoiles)
    unsigned short	val;
    ///	Adresse des codes de blocs tableaux
    unsigned long 	cod;
    ///	Sprites de la page courante
    unsigned short	spr[240];
    ///	Nb de tableaux
    unsigned short	nt;
    ///	Adresses des tableaux
    unsigned long	*fnt;
    ///	Tableaux
    LevelAtari		*tb;
  };

}

#endif


