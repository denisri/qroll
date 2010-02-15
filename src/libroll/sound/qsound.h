/***************************************************************************
                          qsound.h
                             -------------------
    begin                : 2004
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


#ifndef ROLL_SOUND_QSOUND_H
#define ROLL_SOUND_QSOUND_H

#include <roll/sound/soundProcessor.h>

namespace roll
{

  /// RR sound processor implementation using QSound
  class RRQSound : public RRSoundProcessor
  {
  public:
    RRQSound();
    virtual ~RRQSound();
    virtual void process( SNDLIST type );
    virtual void stop();
    virtual void stop( SNDLIST type );
    virtual void close();
    virtual bool isOK() const;
    virtual std::string name() const;
    /** The QSound has priority 10, which is rather poor (compared to 100 for 
        the DSP sound) because it doesn't allow sound mixing, and it's 
        not really real-time (sounds are queued so can be delayed)
     */
    virtual float priorityRating() const;

  protected:
    virtual void init();
    virtual void loadSounds();

  private:
    struct Private;
    Private	*d;
  };

}

#endif

