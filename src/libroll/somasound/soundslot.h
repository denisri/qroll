/***************************************************************************
                          soundslot.h
                       -------------------
    begin                : 1999
    copyright            : (C) 1999-2010 by Denis Rivière
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


#ifndef SOMA_SOMASOUND_SOUNDSLOT_H
#define SOMA_SOMASOUND_SOUNDSLOT_H

#include "diffcode.h"
#include <vector>

namespace soma
{

  struct SoundSlot
  {
    SoundSlot( const std::string & filename, bool shouldbeloaded=true );
    ~SoundSlot();

    std::string filename;
    bool shouldbeloaded;
    bool valid;
    bool loaded;
    std::vector<unsigned char> buffer;
    bool diffcoded;
    audiq::DiffCode::CompressInfo compressinfo;
    std::string tempfilename;
  };

}

#endif

