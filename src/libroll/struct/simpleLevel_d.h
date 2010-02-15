/***************************************************************************
                          simpleLevel_d.h  -  description
                             -------------------
    begin                : 2001
    copyright            : (C) 2000-2004 by Denis Rivi�e
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

#ifndef ROLL_STRUCT_SIMPLELEVEL_D_H
#define ROLL_STRUCT_SIMPLELEVEL_D_H

#include <roll/struct/simpleLevel.h>
#include <roll/struct/general.h>
#include <string>
#include <iostream>
#include <fstream>
#include <stdint.h>


namespace roll
{

  template<class IStream> class istream_traits;
  template <typename IStream> istream_traits<IStream> &
  operator >> ( istream_traits<IStream> &, uint32_t & );
  template <typename IStream> istream_traits<IStream> &
  operator >> ( istream_traits<IStream> &, uint16_t & );
  template <typename IStream> istream_traits<IStream> &
  operator >> ( istream_traits<IStream> &, uint8_t & );
  template <typename IStream> istream_traits<IStream> &
  operator >> ( istream_traits<IStream> &, std::string & );

  /**	Helper class for SimpleLevel stream IO operations. 
	It is useful for using SimpleLevel::read and SimpleLevel::write 
	functions on non-standard streams (like QDataStream from Qt). 
	SimpleLevel IO functions use a istream_traits wrapper for its 
	stream class.
  */
  template<class IStream> class istream_traits
  {
  public:
    istream_traits( IStream & s ) : _stream( s ) {}
    void seekg( int pos ) { _stream.seekg( pos, std::ios::cur ); }
    void read( char* buf, unsigned len ) { _stream.read( buf, len ); }
    /**	Stream-reading operators. 
	SimpleLevel::read will only require the following types:
	- operator >> ( unsigned )
	- operator >> ( unsigned short )
	- operator >> ( unsigned char )
	- operator >> ( std::string )
	Here we avoid template functions, not well supported by all compilers 
	(Microsoft VC++...)
    */
    /*template<typename T> 
    istream_traits<IStream> & operator >> ( T & x )
    { _stream >> x; return( *this ); }*/
    bool operator ! () const { return( !_stream ); }
    void bin()
    {
      //out << "bin() pos: " << _stream.tellg() << endl;
      char	c;
      _stream.get( c );
      if( c == 0x0d )	// Dos/Windows world
	_stream.get( c );
      //out << "-> " << _stream.tellg() << endl;
    }
    void ascii() {}

  private:
    friend istream_traits<IStream> &
      operator >> <> ( istream_traits<IStream> &, uint32_t & );
    friend istream_traits<IStream> &
      operator >> <> ( istream_traits<IStream> &, uint16_t & );
    friend istream_traits<IStream> &
      operator >> <> ( istream_traits<IStream> &, uint8_t & );
    friend istream_traits<IStream> &
      operator >> <> ( istream_traits<IStream> &, std::string & );
    IStream	& _stream;
  };


  template <typename IStream>
  istream_traits<IStream> & operator >> ( istream_traits<IStream> & s,
                                          uint32_t & x )
  { s._stream >> x; return s; }
  template <typename IStream>
  istream_traits<IStream> & operator >> ( istream_traits<IStream> & s,
                                          uint16_t & x )
  { s._stream >> x; return s; }
  template <typename IStream>
  istream_traits<IStream> & operator >> ( istream_traits<IStream> & s,
                                          uint8_t & x )
  { s._stream >> x; return s; }
  template <typename IStream>
  istream_traits<IStream> & operator >> ( istream_traits<IStream> & s,
                                          std::string & x )
  { s._stream >> x; return s; }


  /**	Helper class for SimpleLevel stream IO operations. 
	It is useful for using SimpleLevel::read and SimpleLevel::write 
	functions on non-standard streams (like QDataStream from Qt). 
	SimpleLevel IO functions use a istream_traits wrapper for its 
	stream class.
  */
  template<class OStream> class ostream_traits
  {
  public:
    ostream_traits( OStream & s ) : _stream( s ) {}
    void write( const char* buf, unsigned len );
    /**	Stream-writing operators. 
	SimpleLevel::write will only require the following types:
	- operator << ( unsigned )
	- operator << ( unsigned short )
	- operator << ( unsigned char )
	- operator << ( const char* )
   */
    template<typename T> inline
    ostream_traits<OStream> & operator << ( const T & x ) 
    { _stream << x; return( *this ); }

  private:
    OStream	& _stream;
  };


  template<typename T>
  inline void ostream_traits<T>::write( const char* buf, unsigned len )
  {
    _stream.write( buf, len );
  }


  template<typename T> static inline T bswap( T x );


  template<> /*static*/ inline unsigned short bswap( unsigned short x )
  {
    return( ( (x & 0xff) << 8 ) | ( x >> 8 ) );
  }


  template<> /*static*/ inline unsigned bswap( unsigned x )
  {
    return( ( (x & 0xff) << 24 ) | ( (x & 0xff00) << 8 ) 
	    | ( (x & 0xff0000) >> 8 ) | ( x >> 24 ) );
  }


  static void cleantag( std::string & s )
  {
    if( !s.empty() && ( s[ s.length()-1 ] == ' ' || s[ s.length()-1 ] == '\t' 
        || s[ s.length()-1 ] == '\n' ) )
      s.erase( s.length()-1, 1 );
  }

  /*	Member template functions don't work under Microsoft VC++ 6: they 
	are never instantiated. To work around this silly problem, we must 
	use a silly macro solution */
#ifdef _MSC_VER
#ifndef OStream
#define OStream std::ofstream
#endif
#else
  template<class OStream>
#endif
  void SimpleLevel::write( OStream & stream ) const
  {
    const unsigned short	*fg, *bg;
    const std::vector<unsigned>	*fl;
    uint32_t			s, n, i, m;
    ostream_traits<OStream>	file( stream );

    s =sizeX() * sizeY();
    fg = foreground();
    bg = background();
    fl = elementsFlags();

    // tag "Level: n" must be written before
    file << "Dim: " << sizeX() << (unsigned char) 'x' << sizeY() << "\n";
    file << "Layers: ";
    if( bg )
      file << (unsigned) 2 << "\n";
    else
      file << (unsigned) 1 << "\n";
    file << "Flags: ";
    if( fl )
      file << "on\n";
    else
      file << "off\n";

    // write params

    file << "[Params]\n";
    file << "Diams: " << _diams << "\n";
    file << "Time: " << _time << "\n";
    file << "Permeability: " << (unsigned) _permeability << "\n";
    file << "Speed: " << (unsigned) _speed << "\n";
    file << "Strength: " << (unsigned) _strength << "\n";
    file << "GenSpeed: " << (unsigned) _genspeed << "\n";
    file << "BlobMaxSize: " << _blobmaxsize << "\n";
    file << "BombDelay: " << _bombdelay << "\n";
    file << "ColWall: " << _colwall << "\n";
    file << "ColDiam: " << _coldiam << "\n";
    file << "ColRoll: " << _colroll << "\n";
    file << "CollLeftMonster: " << _colleftmonster << "\n";
    file << "ColGrass: " << _colgrass << "\n";
    file << "LevelFlags: " << _flags << "\n";

    n = params.size();
    if( n > 0 )
      {
	file << "[VariableParams]\n";
	file << "NumParams: " << n << "\n";
	for( i=0; i<n; ++i )
	  {
	    file << "ParamCode: " << params[i].first << "\n";
	    m = params[i].second.size();
	    file << "ParamSize: " << m << "\n";
	    file.write( (const char *) &params[i].second[0], 
			m * sizeof( uint16_t ) );
	  }
      }

    // write level data
    file << "[LevelData]\n";
    file << "Layer: " << (uint32_t) 0 << "\n";
    file << "Size: " << s << "\n";
    file.write( (const char *) fg, s * sizeof( uint16_t ) );

    if( bg )
      {
	file << "Layer: " << (uint32_t) 1 << "\n";
	file << "Size: " << s << "\n";
	file.write( (const char *) bg, s * sizeof( uint16_t ) );
      }

    if( fl )
      {
	file << "DataFlags: " << (uint32_t) fl->size() << "\n";
	file.write( (const char *) &(*fl)[0], 
		    fl->size() * sizeof( unsigned ) );
      }
  }


#ifdef _MSC_VER
#ifndef IStream
#define IStream std::ifstream
#endif
#else
  template<class IStream>
#endif
  bool SimpleLevel::read( IStream & stream, unsigned bo )
  {
    istream_traits<IStream>	file( stream );
    clear();

    std::string	tag;
    unsigned	sx = 0, sy = 0;
    unsigned	nlay = 0;
    bool	hasflags = false;
    unsigned char	c;

    if( !file )
      return( false );
    file >> tag;
    cleantag( tag );

    while( !!file && tag[0] != '[' )
      {
	//out << "tag : " << tag << std::endl;
	if( tag == "Dim:" )
	  file >> sx >> c >> sy;
	else if( tag == "Layers:" )
	  file >> nlay;
	else if( tag == "Flags:" )
	  {
	    file >> tag;
	    cleantag( tag );
	    if( tag == "on" || tag == "1" || tag == "true" )
	      hasflags = true;
	    else
	      hasflags = false;
	  }
	else if( !tag.empty() )
	  err << "unrecognized tag " << tag << std::endl;

	file >> tag;
	cleantag( tag );
      }
    if( !file || sx == 0 || sy == 0 || nlay == 0 )
      {
	err << "load failed\n";
	return( false );
      }

    /*out << "Level : " << sx << " x " << sy << ", " << nlay 
	<< " layers, " << ( hasflags ? "with flags" : "no flags" ) 
	<< std::endl;*/

    *this = SimpleLevel( sx, sy, nlay > 1, hasflags );

    unsigned	val;

    while( !!file && tag != "[LevelData]" )
      {
	if( tag == "[Params]" )
	  {
	    file >> tag;
	    cleantag( tag );
	    while( !!file && tag[0] != '[' )
	      {
		if( tag == "Diams:" )
		  file >> _diams;
		else if( tag == "Time:" )
		  file >> _time;
		else if( tag == "Permeability:" )
		  {
		    file >> val;
		    _permeability = (unsigned char) val;
		  }
		else if( tag == "Speed:" )
		  {
		    file >> val;
		    _speed = (unsigned char) val;
		  }
		else if( tag == "Strength:" )
		  {
		    file >> val;
		    _strength = (unsigned char) val;
		  }
		else if( tag == "GenSpeed:" )
		  {
		    file >> val;
		    _genspeed = (unsigned char) val;
		  }
		else if( tag == "BlobMaxSize:" )
		  file >> _blobmaxsize;
		else if( tag == "BombDelay:" )
		  file >> _bombdelay;
		else if( tag == "ColWall:" )
		  file >> _colwall;
		else if( tag == "ColDiam:" )
		  file >> _coldiam;
		else if( tag == "ColRoll:" )
		  file >> _colroll;
		else if( tag == "CollLeftMonster:" )
		  file >> _colleftmonster;
		else if( tag == "ColGrass:" )
		  file >> _colgrass;
		else if( tag == "LevelFlags:" )
		  file >> _flags;
		else if( !tag.empty() )
		  err << "unrecognized tag " << tag << std::endl;
		file >> tag;
		cleantag( tag );
	      }
	  }
	else if( tag == "[VariableParams]" )
	  {
	    file >> tag;
	    cleantag( tag );
	    if( tag == "NumParams:" )
	      {
		unsigned	np = 0, pc, sp, i;
		file >> np;
		for( i=0; i<np; ++i )
		  {
		    pc = 0;
		    sp = 0;
		    do
		      {
			file >> tag;
			cleantag( tag );
		      } while( tag.empty() && !!file );
		    if( tag != "ParamCode:" )
		      return( false );
		    file >> pc;
		    do
		      {
			file >> tag;
			cleantag( tag );
		      } while( tag.empty() && !!file );
		    if( tag != "ParamSize:" )
		      return( false );
		    file >> sp;

		    file.bin();
		    ParamBlock	pb( (ParamCode) pc, 
				    std::vector<unsigned short>( sp ) );
		    file.read( (char *) &pb.second[0], 
			       sp * sizeof( unsigned short ) );
		    file.ascii();
		    // byteswap data
		    if( bo != 0x41424344 )
		      for( unsigned z=0; z<sp; ++z )
			pb.second[z] = bswap( pb.second[z] );
		    params.push_back( pb );
		  }
	      }
	    else
	      err << "unrecognized tag " << tag << std::endl;
	    file >> tag;
	    cleantag( tag );
	  }
	else
	  {
	    err << "unrecognized tag " << tag << std::endl;
	    file >> tag;
	    cleantag( tag );
	  }
      }

    if( !file )
      return( false );

    //	read data

    unsigned 		i, j, sz;
    unsigned short	*dat;

    for( i=0; i<nlay; ++i )
      {
	do
	  {
	    file >> tag;
	    cleantag( tag );
	  } while( tag.empty() && !!file );
	if( tag != "Layer:" )
	  {
	    err << "expected tag \"Layer:\", got " << tag << std::endl;
	    return( false );
	  }
	file >> j;
	if( i != j )
	  err << "warning : wrong layer number: " << j << ", should be " 
	      << i << std::endl;
	do
	  {
	    file >> tag;
	    cleantag( tag );
	  } while( tag.empty() && !!file );
	if( tag != "Size:" )
	  {
	    err << "expected tag \"Size:\", got " << tag << std::endl;
	    return( false );
	  }
	file >> sz;
	if( sz != sizeX() * sizeY() )
	  {
	    err << "Level layer " << i << " Size mismatch: " << sz 
		<< ", should be " << sizeX() << " x " << sizeY() 
		<< " = " << sizeX() * sizeY() << std::endl;
	    return( false );
	  }
	//out << "data size: " << sz << std::endl;
	dat = (i == 0 ? foreground() : background() );
	file.bin();
	if( dat )
	  {
	    file.read( (char *) dat, sz * sizeof( unsigned short ) );
	    if( bo != 0x41424344 )
	      for( unsigned z=0; z<sz; ++z )
		dat[z] = bswap( dat[z] );
	    file.ascii();
	  }
	else
	  file.seekg( sz * sizeof( unsigned short ) );
      }
    //out << "level data read\n";
    if( !file )
      {
	err << "broken file\n";
	return( false );
      }

    if( hasflags )
      {
	file >> tag;
	cleantag( tag );
	if( tag != "DataFlags:" )
	  {
	    err << "expected tag \"DataFlags:\", got " << tag 
		<< std::endl;
	    return( false );
	  }
	file >> sz;
	if( _elemflags )
	  {
	    _elemflags->insert( _elemflags->end(), sz, 0 );
	    file.bin();
	    file.read( (char *) &(*_elemflags)[0], sz * sizeof( unsigned ) );
	    file.ascii();
	    if( bo != 0x41424344 )
	      for( unsigned z=0; z<sz; ++z )
		(*_elemflags)[z] = bswap( (*_elemflags)[z] );
	  }
	else
	  file.seekg( sz * sizeof( unsigned short ) );
      }
    return( true );
  }

}


#endif

