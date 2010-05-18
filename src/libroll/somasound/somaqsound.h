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


#ifndef SOMA_SOUND_SOMAQSOUND_H
#define SOMA_SOUND_SOMAQSOUND_H

#include "somasoundprocessor.h"

namespace soma
{

  /// sound processor implementation using QSound
  class SomaQSound : public SomaSoundProcessor
  {
  public:
    SomaQSound();
    virtual ~SomaQSound();
    virtual void process( int type );
    virtual void stop();
    virtual void stop( int type );
    virtual void close();
    virtual bool isOK() const;
    virtual std::string name() const;
    /** The QSound has priority 10, which is rather poor (compared to 100 for 
        the OSS sound) because it doesn't allow sound mixing, and it's
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

