/***************************************************************************
                          qdatastreamWrapper.h  -  description
                             -------------------
    begin                : 1999
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

#ifndef ROLL_GUI_QDATASTREAMWRAPPER_H
#define ROLL_GUI_QDATASTREAMWRAPPER_H


#include <qdatastream.h>
#include <roll/struct/simpleLevel_d.h>
#include <roll/struct/general.h>

namespace roll
{

  template<> inline 
  bool istream_traits<QDataStream>::operator ! () const
  {
    return( /*_stream.atEnd() ||*/ !_stream.device()->isOpen() 
	    || _stream.device()->status() != (int) IO_Ok );
  }

  template<> /*template<>*/ inline 
  istream_traits<QDataStream> & 
  operator >> ( istream_traits<QDataStream> & stm, std::string & x )
  {
    char	*s;
    stm._stream >> s;
    x = s;
    delete[] s;
    return stm;
  }

  template<> inline 
  void istream_traits<QDataStream>::read( char* buf, unsigned len )
  {
    _stream.readRawBytes( buf, len );
  }

  template<> inline void istream_traits<QDataStream>::seekg( int pos )
  {
    _stream.device()->at( _stream.device()->at() + pos );
  }

  template<> inline 
  void ostream_traits<QDataStream>::write( const char* buf, unsigned len )
  {
    _stream.writeRawBytes( buf, len );
  }

  template<> inline
  void istream_traits<QDataStream>::bin()
  {
    char	*tmp;
    _stream >> tmp;
    delete[] tmp;
  }

}


#endif

