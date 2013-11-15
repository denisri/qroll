/***************************************************************************
                          qfileWrapper.h  -  description
                             -------------------
    begin                : 2013
    copyright            : (C) 2013 by Denis Rivière
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

#ifndef ROLL_STRUCT_QFILEWRAPPER_H
#define ROLL_STRUCT_QFILEWRAPPER_H

#include <roll/struct/general.h>
#include <roll/struct/stream_traits.h>
#include <QFile>

namespace roll
{

  // specialization of istream_traits for QFile

  template<> inline
  bool istream_traits<QFile>::operator ! () const
  {
    return !_stream.isOpen();
  }

  template<> inline
  istream_traits<QFile> &
  operator >> ( istream_traits<QFile> & stm, std::string & x )
  {
    std::string str;
    char c;
    while( !stm._stream.atEnd() )
    {
      if( !stm._stream.getChar( &c ) )
      {
        err << "error reading " << stm._stream.fileName().toStdString() << std::endl;
        break;
      }
      if( c == '\0' || c == ' ' || c == '\t' || c == '\n' || c == '\r' )
      {
        if( !str.empty() )
          break;
      }
      else
        str += c;
    }
    x = str;
    return stm;
  }

  template<> inline
  istream_traits<QFile> &
  operator >> ( istream_traits<QFile> & stm, uint32_t & x )
  {
    QString str;
    char c = '.';
    while( !stm._stream.atEnd() )
    {
      if( !stm._stream.getChar( &c ) )
      {
        err << "error reading number in " << stm._stream.fileName().toStdString()
            << std::endl;
        return stm;
      }
      if( c < '0' || c > '9' )
      {
        if( !str.isEmpty() )
        {
          if( c != '\0' ) // && c != '\n' && c != '\r' )
            stm._stream.ungetChar( c );
          break;
        }
      }
      else
        str += c;
    }
    bool ok = true;
    x = str.toUInt( &ok );
    return stm;
  }

  template<> inline
  istream_traits<QFile> &
  operator >> ( istream_traits<QFile> & stm, uint16_t & x )
  {
    QString str;
    char c = '.';
    while( !stm._stream.atEnd() )
    {
      if( !stm._stream.getChar( &c ) )
      {
        err << "error reading number in " << stm._stream.fileName().toStdString()
            << std::endl;
        return stm;
      }
      if( c < '0' || c > '9' )
      {
        if( !str.isEmpty() )
        {
          if( c != '\0' ) // && c != '\n' && c != '\r' )
            stm._stream.ungetChar( c );
          break;
        }
      }
      else
        str += c;
    }
    bool ok = true;
    x = str.toUShort( &ok );
    return stm;
  }

  template<> inline
  istream_traits<QFile> &
  operator >> ( istream_traits<QFile> & stm, uint8_t & x )
  {
    if( !stm._stream.atEnd() )
    {
      if( !stm._stream.getChar( (char *) &x ) )
      {
        err << "error reading series " << stm._stream.fileName().toStdString()
            << std::endl;
        return stm;
      }
    }
    return stm;
  }

  template<> inline
  void istream_traits<QFile>::read( char* buf, unsigned len )
  {
    _stream.read( buf, len );
  }

  template<> inline void istream_traits<QFile>::seekg( int pos )
  {
    _stream.seek( _stream.pos() + pos );
  }

  template<> inline
  void ostream_traits<QFile>::write( const char* buf, unsigned len )
  {
    _stream.write( buf, len );
  }

  template<> inline
  void istream_traits<QFile>::bin()
  {
    char	c;
    _stream.getChar( &c );
    if( c == 0x0d )	// Dos/Windows world
      _stream.getChar( &c );
  }

}

#endif // ROLL_STRUCT_QFILEWRAPPER_H
