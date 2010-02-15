/***************************************************************************
                          series.h  -  description
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


#ifndef ROLL_STRUCT_SERIES_H
#define ROLL_STRUCT_SERIES_H


#include <string>


namespace roll
{
  class WorkLevel;
  struct SimpleLevel;
  class Level;

  /**	Levels Series (loaded from file)
   */
  class Series
  {
  public :
    virtual ~Series();

    virtual bool load( const std::string & name ) = 0;
    virtual bool save( const std::string & fileName= "", 
		       const std::string & format = "" ) const;
    /// save series in raw format (should be common to all series classes)
    virtual bool saveRaw( const std::string & = "" ) const;
    /// nmber of levels in current series
    virtual unsigned numLevels() const = 0;
    const std::string & filename() const { return( _filename ); }
    void setFilename( const std::string & fname ) { _filename = fname; }
    /// Max. number of layers of elements in each level (-1 for unlimited)
    virtual int levelLayers() const { return( 1 ); }
    /** \return true if the series format can handle and keep elements state 
	flags */
    virtual bool keepsElementFlags() const { return( false ); }
    /// retreives the size of level number \c level
    virtual void size( unsigned level, unsigned & sx, 
		       unsigned & sy ) const = 0;
    virtual const Level* level( unsigned x ) const = 0;
    virtual Level* level( unsigned x ) = 0;
    /** pack a WorlLevel (full working level in game format) in the compacted
	form of the series (close to disk format).
	(uses intermediate SimpleLevel format) */
    bool pack( const WorkLevel &, unsigned level );
    /** pack a SimpleLevel (level in independent format) in the compacted
	form of the series (close to disk format) */
    virtual bool pack( const SimpleLevel &, unsigned level ) = 0;
    /** packs a new level after stored ones
	\return new level number, -1 if insertion failed */
    virtual int appendLevel( const SimpleLevel & sl );
    /** packs a new level before position \c pos, -1: at end (append).
	The default implementation is unable to insert at selected position, 
	it just appends it. SeriesRaw can do it.
	\return new level number, -1 if insertion failed */
    virtual int insertLevel( int pos, const SimpleLevel & sl );
    virtual bool deleteLevel( unsigned pos );
    /** unpack a level of the current series to a WorkLevel for edition, game 
	or viewing.
	(uses intermediate SimpleLevel format) */
    void unpack( unsigned level, WorkLevel & ) const;
    /** unpack a level of the current series to a SimpleLevel */
    virtual void unpack( unsigned level, SimpleLevel & ) const = 0;
    bool modified() const { return( _modified ); }
    void setModified( bool x ) { _modified = x; }

  protected :
    Series();
    Series( const std::string & name );
    void clearModified() const { _modified = false; }
    /**	helper function; resizes \c sl to the size of level \c level and 
	sets appropriate background / flags. 
	Use this function in Series subclasses in your unpack() function */
    void resizeSimpleLevel( unsigned level, SimpleLevel & sl ) const;
    /// load raw format series (should be common to all series classes)
    virtual bool loadRaw( const std::string & name );

    std::string		_filename;
    mutable bool	_modified;
  };

}

#endif


