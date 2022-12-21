/***************************************************************************
                          qsound.h
                             -------------------
    begin                : 2022
    copyright            : (C) 2022 by Denis Rivière
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


#ifndef SOMA_SOUND_SOMAQSOUNDEFFECT_H
#define SOMA_SOUND_SOMAQSOUNDEFFECT_H

#include "somasoundprocessor.h"
#include <QtGlobal>
#if QT_VERSION >= 0x050000
#define SOMA_SOUND_QSOUNDEFFECT

namespace soma
{

  /// sound processor implementation using QSoundEffect
  class SomaQSoundEffect : public SomaSoundProcessor
  {
  public:
    SomaQSoundEffect();
    virtual ~SomaQSoundEffect();
    virtual void process( int type );
    virtual void stop();
    virtual void stop( int type );
    virtual void close();
    virtual bool isOK() const;
    virtual std::string name() const;
    /** The QSoundEffect has priority 20, (compared to 100 for
        the OSS sound) because the audio device does not always work, and the
        current implementation only allows one sound of a given type at a time.
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
#endif

