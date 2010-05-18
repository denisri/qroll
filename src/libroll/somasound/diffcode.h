/***************************************************************************
    copyright            : (C) 2004 by Denis Rivière
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

#ifndef AUDIQ_CORE_DIFFCODE_H
#define AUDIQ_CORE_DIFFCODE_H

#include "wavheader.h"
#include <string>
#include <iostream>

namespace audiq
{
  struct WavHeader;

  class DiffCode
  {
  public:
    /// Compression information
    struct Compression
    {
      Compression( unsigned b = 0, size_t s = 0 );

      unsigned	bits;
      size_t	size;
    };


    /** Position in a compressed stream.
        This structure holds all needed information to allow decoding at the 
        selected position in the input stream. It includes data currently 
        in decoding phase for each audio track.
    */
    struct CompressedPos
    {
      CompressedPos( size_t p = 0, unsigned nchannels = 1 );
      CompressedPos( const CompressedPos & );
      ~CompressedPos();
      CompressedPos & operator = ( const CompressedPos & );

      size_t	pos;
      struct Internal;
      Internal	*d;
    };


    /// DiffCode compressed file information (including the .wav header)
    struct CompressInfo
    {
      CompressInfo( unsigned b = 0 );
      void read( std::istream & s, const std::string & filename = "" );

      unsigned	bits;
      WavHeader	hdr;
    };

    static Compression checkCompress( const std::string & filename, 
                                      bool verbose = false );
    static Compression checkCompress( const char* buf, const WavHeader & hdr, 
                                      bool verbose = false );
    static Compression checkCompress( std::istream & f, const WavHeader & hdr, 
                                      bool verbose = false );
    static Compression compress( const std::string & ifile, 
                                 const std::string & ofile, unsigned bits = 0, 
                                 bool verbose = false );
    static Compression compress( const char* buf, const WavHeader & hdr, 
                                 const std::string & ofile, unsigned bits = 0, 
                                 bool verbose = false );
    static Compression compress( std::istream & f, const WavHeader & hdr, 
                                 const std::string & ofile, unsigned bits = 0, 
                                 bool verbose = false );
    static WavHeader uncompress( const std::string & filename, char* buf, 
                                 size_t bufsize, size_t start = 0, 
                                 int len = -1 );
    static WavHeader uncompress( const std::string & filename, 
                                 const std::string & wavfile, 
                                 size_t start = 0, int len = -1 );
    static CompressedPos uncompress( const char* ibuf, 
                                     const CompressInfo & info, char* buf, 
                                     const CompressedPos & pos 
                                     = CompressedPos(), int len = -1 );
    static CompressedPos uncompress( std::istream & s, 
                                     const CompressInfo & info, 
                                     char* buf, const CompressedPos & pos = 
                                     CompressedPos(), int len = -1 );
  };

}

#endif

