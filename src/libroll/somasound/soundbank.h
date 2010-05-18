/***************************************************************************
                          soundBank.h
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


#ifndef SOMA_SOMASOUND_SOUNDBANK_H
#define SOMA_SOMASOUND_SOUNDBANK_H

#include <vector>
#include <fstream>

namespace soma
{

  /// set of sounds, either loaded into memory, or accessed in files
  class SoundBank
  {
  public:
    struct SoundSlot;

    SoundBank();
    virtual ~SoundBank();

    int addSound( const std::string & filename, bool shouldBeLoaded=true );
    void loadSounds();
    virtual void init();
    void reserve( unsigned n );

    SoundSlot & sound( unsigned n );
    std::vector<SoundSlot> & sounds();

  protected:
    std::vector<SoundSlot> _sounds;
  };

}


#endif

