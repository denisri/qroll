/***************************************************************************
                          dsp.h  
                             -------------------
    begin                : 1999
    copyright            : (C) 1999-2004 by Denis Rivière
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


#ifndef SOMA_SOMASOUND_SOMASOUNDOSS_H
#define SOMA_SOMASOUND_SOMASOUNDOSS_H

#ifdef SOMA_SOUND_OSS // comiled only if SOMA_SOUND_OSS is defined

#include "somasoundprocessor.h"
#include <list>
#include <pthread.h>

namespace soma
{

  /** Direct usage of /dev/dsp, using a threaded mixer: it implements
    pseudo-real-time mixing of a variable number of channels.

    An advantage of (sadly obsolete) OSS was that it didn't require to link
    against any software library to use: only the device file and ioctls are
    enough.
  */
  class SomaSoundOSS : public SomaSoundProcessor
  {
  public:
    /// Sound request structure
    struct SndReq
    {
      /// sound number
      int type;
      /// Current position while reading (negative = not started)
      int pos;
    };
    /// Max number of sounds of the same type playing at the same time
    static const unsigned MaxSameSample;
    SomaSoundOSS();
    virtual ~SomaSoundOSS();
    virtual void process( int type );
    virtual void stop();
    virtual void stop( int type );
    virtual void close();
    virtual void disable() { SomaSoundProcessor::disable(); ok = false; }
    virtual unsigned inuse( int type );
    int dspFD() const { return( fd ); }
    bool isOK() const { return( ok ); }
    virtual std::string name() const;
    /** The OSS sound has priority 100 (I consider it rather good for its
        mixing capabilities)
    */
    virtual float priorityRating() const;

  protected:
    /// file descriptor of device (/dev/dsp)
    int fd;
    /// flag telling if the driver is OK (otherwise sound is disabled)
    bool ok;
    /// Size of the dsp buffer
    int bufferSize;
    /// Buffer
    unsigned char *buffer;
    /// Sounds currently playing
    std::list<SndReq> jobs;
    /// Corresponding mutex
    pthread_mutex_t listLock;
    /// ID of the mixing thread
    pthread_t updateThrd;
    bool threadRunning;
    /// Playback frequency
    unsigned freqDsp;
    virtual void init();
    virtual void loadSounds();
    /// static "callback" callback of the mixing thread
    static void * updateThread( void * );
    /// non-static mixing method, called in the mixing thread
    void update();
    /// Stops the oldest sound of the given type (Warning : NO MUTEX)
    virtual void stopOld( int type );
  };

}

#endif // #ifdef SOMA_SOUND_OSS
#endif
