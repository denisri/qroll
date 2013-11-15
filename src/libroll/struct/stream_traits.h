#ifndef ROLL_STRUCT_STREAM_TRAITS_H
#define ROLL_STRUCT_STREAM_TRAITS_H

#include <string>
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

}

#endif // ROLL_STRUCT_STREAM_TRAITS_H
