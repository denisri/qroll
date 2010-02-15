/***************************************************************************
    copyright            : (C) 2002-2003 by Denis Rivière
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

#ifndef AUDIQ_CORE_WAVHEADER_H
#define AUDIQ_CORE_WAVHEADER_H

#include <string>
#include <iostream>

namespace audiq
{

  struct WavHeader
  {
    WavHeader();
    ~WavHeader();

    void read( const std::string & fname );
    void read( std::istream & file, const std::string & fname = "" );
    void write( const std::string & fname );
    /// write header into an existing file
    void write( std::ostream & file );

    std::string	filename;
    ///	mono (1) / stereo (2)
    int		channels;
    /// num of samples in file
    size_t	size;
    /// bytes / sample
    int		sampleSize;
    bool	sign;
    unsigned	rate;
    /// header length (in bytes): beginning of raw data in file
    size_t	headerOffset;
  };

}

#endif

