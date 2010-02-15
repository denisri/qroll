/***************************************************************************
                          seriesRaw.h  -  description
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


#ifndef ROLL_STRUCT_SERIESRAW_H
#define ROLL_STRUCT_SERIESRAW_H

#include <roll/struct/series.h>
#include <vector>


namespace roll
{
  class LevelRaw;
  struct AtariFormatInfo;

  class SeriesRaw : public Series
  {
  public:
    SeriesRaw();
    SeriesRaw( const std::string & name );
    virtual ~SeriesRaw();

    virtual const Level* level( unsigned x ) const;
    virtual Level* level( unsigned x );
    virtual bool load( const std::string & name );
    bool loadAtari( const std::string & name );
    virtual bool save( const std::string & = "", 
		       const std::string & format = "" ) const;
    virtual bool saveAtari( const std::string & ) const;
    virtual unsigned numLevels() const { return( levels.size() ); }
    virtual int levelLayers() const { return( 1 ); }
    virtual bool keepsElementFlags() const { return( false ); }
    virtual void size( unsigned level, unsigned & sx, unsigned & sy ) const;
    virtual bool pack( const SimpleLevel &, unsigned level );
    virtual void unpack( unsigned level, SimpleLevel & ) const;
    virtual int appendLevel( const SimpleLevel & sl );
    virtual int insertLevel( int pos, const SimpleLevel & sl );
    virtual bool deleteLevel( unsigned pos );
    void clear();

  protected:
    void convList( AtariFormatInfo & );

    std::vector<LevelRaw *>	levels;
  };

}


#endif


