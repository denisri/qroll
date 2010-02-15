/***************************************************************************
                          simpleLevel.h  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000-2010 by Denis Rivière
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

#ifndef ROLL_STRUCT_SIMPLELEVEL_H
#define ROLL_STRUCT_SIMPLELEVEL_H

#include <roll/struct/levelParam.h>
#include <string.h>

#ifdef _MSC_VER
class QDataStream;
#endif

namespace roll
{
  class Level;

  /**	A simple, almost clear representation for a Rock'n'roll level.

	This representation is independent from both the disk format and the 
	object representation used for gaming in WorkLevel. It's just an 
	intermediate form used for conversion from any file/Series format to 
	any object/WorkLevel format.

	It is, however, architexture-dependant since the byte ordering is that 
	of the machine which made the SimpleLevel. For network transmissions, 
	it can be necessary to reverse the byte ordering if it is different.

	The level has one or two planes, the background layer and (optionally) 
	the background layer. Elements of each layer are game element numbers 
	in global game numbering (currently 480 elements so 9 bits are used).

	In addition to these planes, a SimpleLevel can also carry a set of 
	"flags" for each location. They are actually arbitrary data that must 
	be encoded / decoded by each indivisuel element when converted from / 
	to WorkLevel objects. On the Series side, flags are just copied \e as 
	\e is or simply discarded if the series format doesn't support flags.
 */
  struct SimpleLevel : public LevelParam
  {
    /// Emty level
    SimpleLevel();
    SimpleLevel( unsigned sizex, unsigned sizey, bool hasback = true, 
		 bool hasflags = false );
    SimpleLevel( const SimpleLevel & x );
    ~SimpleLevel();
    SimpleLevel & operator = ( const SimpleLevel & x );

    unsigned long byteOrder() const { return( _byteOrder ); }
    void setByteOrder( unsigned long bo ) { _byteOrder = bo; }
    bool hasBackground() const { return( _background != 0 ); }
    unsigned short *foreground() const { return( _foreground ); }
    /// \return 0 if hasBackground() is false
    unsigned short *background() const { return( _background ); }
    bool hasElementsFlags() const { return( _elemflags != 0 ); }
    std::vector<unsigned> *elementsFlags() const { return( _elemflags ); }
#ifndef _MSC_VER
    /**	Writes the level in a stream in raw format.

	The template function allows to use this mechanism on various classes 
	of streams. Output stream types must have a ostream_traits wrapper 
	defined with appropriate specializations (it will be instantiated in 
	the SimpleLevel::write function.

	It is already compiled on the standard std::ostream class.
	In the Qt version the IO routines are also compiled on the QDataStream 
	streams, using a wrapper: ostream_traits<QDataStream>

	To compile for other stream types, include 
	\c <roll/struct/simpleLevel_d.h>
    */
    template<class OStream> 
    void write( OStream & ) const;
#else
    void write( std::ofstream & ) const;
    void write( QDataStream & ) const;
#endif
#ifndef _MSC_VER
    /**	Reads the level in raw format from a stream.

	The template function allows to use this mechanism on various classes 
	of streams. Input stream types must have a istream_traits wrapper 
	defined with appropriate specializations (it will be instantiated in 
	the SimpleLevel::read function.

	It is already compiled on the standard std::istream class. 
	In the Qt version the IO routines are also compiled on the QDataStream 
	streams, using a wrapper: istream_traits<QDataStream>

	To compile for other stream types, include 
	\c <roll/struct/simpleLevel_d.h>
    */
    template<class IStream>
    bool read( IStream &, unsigned byteorder = 0x41424344 );
#else
    bool read( std::ifstream &, unsigned byteorder = 0x41424344 );
    bool read( QDataStream &, unsigned byteorder = 0x41424344 );
#endif
    ///	fills the level with empty elements, standard borders & colors
    void makeEmpty();
    /// resizes the level, new contents are uninitialized (filled with random)
    void resize( unsigned sx, unsigned sy );

    std::vector<ParamBlock>	params;
    Level			*srcLevel;

  private:
    void clear();

    unsigned short		*_foreground;
    unsigned short		*_background;
    std::vector<unsigned>	*_elemflags;
    unsigned long		_byteOrder;
  };


  inline 
  SimpleLevel::SimpleLevel() : LevelParam()
  {
    _foreground = 0;
    _background = 0;
    _elemflags = 0;
  }


  inline
  SimpleLevel::SimpleLevel( unsigned sizex, unsigned sizey, bool hasback, 
			    bool flags ) 
    : LevelParam( sizex, sizey ), srcLevel( 0 ), 
      _foreground( new unsigned short[ sizex * sizey ] ), 
      _background( hasback ? new unsigned short[ sizex * sizey ] : 0 ), 
      _elemflags( flags ? new std::vector<unsigned>[ sizex * sizey ] : 0 ), 
      _byteOrder( 0x10203 )
  {
    //cout << "SimpleLevel::SimpleLevel " << sizex << " x " << sizey << endl;
  }


  inline
  SimpleLevel::SimpleLevel( const SimpleLevel & x )
    : LevelParam( x ), params( x.params ), srcLevel( x.srcLevel ), 
      _foreground( 0 ), _background( 0 ), _elemflags( 0 )
   {
     *this = x;
   }


  inline
  SimpleLevel::~SimpleLevel()
  {
    clear();
  }

  inline
  void SimpleLevel::clear()
  {
    delete[] _foreground;
    delete[] _background;
    delete[] _elemflags;
    _foreground = 0;
    _background = 0;
    _elemflags = 0;
    params.clear();
    srcLevel = 0;
  }


  inline
  SimpleLevel & SimpleLevel::operator = ( const SimpleLevel & x )
  {
    if( &x != this )
      {
	clear();
	*(LevelParam *) this = x;
	params = x.params;
	srcLevel = x.srcLevel;
	unsigned	sz = _sx * _sy;
	_foreground = new unsigned short[ sz ];
	memcpy( _foreground, x._foreground, sz * sizeof( unsigned short ) );
	if( x._background )
	  {
	    _background = new unsigned short[ sz ];
	    memcpy( _background, x._background, 
		    sz * sizeof( unsigned short ) );
	  }
	if( x._elemflags )
	  {
	    _elemflags = new std::vector<unsigned>[ sz ];
	    for( unsigned i=0; i<sz; ++i )
	      _elemflags[i] =  x._elemflags[i];
	  }
      }

    return( *this );
  }


  inline void SimpleLevel::resize( unsigned sx, unsigned sy )
  {
    bool hasback = hasBackground();
    bool flags = hasElementsFlags();
    clear();
    _sx = sx;
    _sy = sy;
    _foreground = new unsigned short[ sx * sy ];
    _background = ( hasback ? new unsigned short[ sx * sy ] : 0 );
    _elemflags = ( flags ? new std::vector<unsigned>[ sx * sy ] : 0 );
  }

}


#endif


