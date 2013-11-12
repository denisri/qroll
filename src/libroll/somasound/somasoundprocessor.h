/***************************************************************************
                          soundProcessor.h
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


#ifndef ROLL_SOUND_SOMASOUNDPROCESSOR_H
#define ROLL_SOUND_SOMASOUNDPROCESSOR_H

#include <string>
#include <vector>


namespace soma
{
  class SoundBank;


  /** Sound and noise player with multi-channel mixing.

      Specificities :

      - portable interface, independante from the underlying sound kit.
      - several implementations (ALSA / OSS / Sun audio / QSound)

      Additional implementations and extensions may be added as plugins.
      This way additional sound kits may be used.

      Can support sound files loaded in memory, or direct reading while playing.
  */
  class SomaSoundProcessor
  {
  public:
    SomaSoundProcessor();
    virtual ~SomaSoundProcessor();
    /// Play sound number "type"
    virtual void process( int type );
    virtual unsigned inuse( int type )
    {
      if( type >= (int) _inuse.size() )
        return false;
      return( _inuse[type] );
    }
    virtual void processIfNotUsed( int type );
    virtual void stop() {}
    virtual void stop( int type );
    /// Close the soundcard connection (and become mute)
    virtual void close() {}
    /// Disable sound
    virtual void disable() { close(); }
    /// Re-enable sound
    virtual void enable() { disable(); init(); }
    /// set buffer time in milliseconds
    virtual void setBufferTime( float ) {}
    virtual void setFrequency( int ) {}
    virtual void setChannels( int ) {}
    /// set freq/channels etc params from one of the sound slots
    virtual void setParams( int ) {}
    /// Tells if the implemention works on the current computer
    virtual bool isOK() const { return( false ); }
    virtual std::string name() const { return "no sound"; }
    /** Used to choose between several working sound processors.
      It should be related to a quality evaluation because when several
      processors are available, you want to select the best one.
      The base SomaSoundProcessor has priority 0, and is mute so it will
      normally have the lowest priority.
    */
    virtual float priorityRating() const { return 0; }
    /// sound banks actually store sound filenames and data when loaded
    void setSoundBank( soma::SoundBank* sbank );
    soma::SoundBank & soundBank();

    static SomaSoundProcessor & processor();

  protected:
    static void registerProcessor( SomaSoundProcessor* p );
    static void unregisterProcessor( SomaSoundProcessor* p );
    virtual void init();

    /// table of sounds currently playing
    std::vector<unsigned> _inuse;
    soma::SoundBank *_sounds;
  };


  inline void SomaSoundProcessor::stop( int ) {}

}

#endif
