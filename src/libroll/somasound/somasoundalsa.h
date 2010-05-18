/***************************************************************************
                          dsp.h
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


#ifndef SOMA_SOUND_SOMASOUNDALSA_H
#define SOMA_SOUND_SOMASOUNDALSA_H

#ifdef SOMA_SOUND_ALSA   // compiled only of SOMA_SOUND_ALSA is defined

#include <roll/sound/soundProcessor.h>
#include <list>
#include <pthread.h>

namespace soma
{

  /**     Sound using ALSA lib, using a threaded mixer: it implements
    pseudo-real-time mixing of a varaible number of channels.
  */
  class SomaSoundAlsa : public SomaSoundProcessor
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
    SomaSoundAlsa();
    virtual ~SomaSoundAlsa();
    virtual void process( int type );
    virtual void stop();
    virtual void stop( int type );
    virtual void close();
    virtual void disable() { SomaSoundProcessor::disable(); ok = false; }
    virtual unsigned inuse( int type );
    bool isOK() const { return( ok ); }
    virtual std::string name() const;
    /** The ALSA sound has priority 150 (better than old OSS API)
    */
    virtual float priorityRating() const;

  protected:
    /// flag telling if the driver is OK (otherwise sound is disabled)
    bool  ok;
    /// Size of the dsp buffer
    int   bufferSize;
    ///   Buffer en question
    unsigned char *buffer;
    /// Sounds currently playing
    std::list<SndReq> jobs;
    /// Corresponding mutex
    pthread_mutex_t listLock;
    /// ID of the mixing thread
    pthread_t     updateThrd;
    bool          threadRunning;
    /// Playback frequency
    unsigned      freqDsp;
    virtual void init();
    virtual void loadSounds();
    /// static "callback" callback of the mixing thread
    static void * updateThread( void * );
    /// non-static mixing method, called in the mixing thread
    void update();
    /// Stops the oldest sound of the given type (Warning : NO MUTEX)
    virtual void stopOld( int type );

  private:
    struct Private;
    Private *d;
  };

}

#endif  // #ifdef SOMA_SOUND_ALSA
#endif
