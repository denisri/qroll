/***************************************************************************
                          sounditerator.h
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


#ifndef SOMA_SOMASOUND_SOUNDITERATOR_H
#define SOMA_SOMASOUND_SOUNDITERATOR_H

#include "diffcode.h"

namespace soma
{

  class SoundSlot;

  class SoundIterator
  {
  public:
    typedef long long size_type;
    SoundIterator( SoundSlot & );
    virtual ~SoundIterator();
    virtual size_type read( size_type nbytes, char* buffer );
    virtual char getch() = 0;
    virtual void seek( size_type nbytes ) = 0;
    virtual bool ended() const { return pos() >= size(); }
    virtual size_type pos() const = 0;
    virtual size_type size() const;
    SoundSlot & sound() { return *_sound; }

    /// factory
    static SoundIterator* makeIterator( SoundSlot & );

    typedef SoundIterator * (*CreationFunction)( SoundSlot & );
    static void registerIteratorType( const std::string & format,
                                      CreationFunction creator );

  protected:
    SoundSlot *_sound;
  };


  class MemorySoundIterator : public SoundIterator
  {
  public:
    MemorySoundIterator( SoundSlot & );
    virtual ~MemorySoundIterator();
    virtual size_type read( size_type nbytes, char* buffer );
    virtual char getch();
    virtual void seek( size_type nbytes );
    virtual size_type pos() const;

    static SoundIterator *create( SoundSlot & );

  private:
    size_type _pos;
  };


  class WavFileSoundIterator : public SoundIterator
  {
  public:
    WavFileSoundIterator( SoundSlot & );
    virtual ~WavFileSoundIterator();
    virtual size_type read( size_type nbytes, char* buffer );
    virtual char getch();
    virtual void seek( size_type nbytes );
    virtual size_type pos() const;
    bool open();

    static SoundIterator *create( SoundSlot & );

  private:
    struct Private;
    Private *d;
  };


  class DiffCodedFileSoundIterator : public SoundIterator
  {
  public:
    DiffCodedFileSoundIterator( SoundSlot & );
    virtual ~DiffCodedFileSoundIterator();
    virtual size_type read( size_type nbytes, char* buffer );
    virtual char getch();
    virtual void seek( size_type nbytes );
    virtual size_type pos() const;
    bool open();

    static SoundIterator *create( SoundSlot & );

  private:
    struct Private;
    Private *d;
  };

}

#endif

