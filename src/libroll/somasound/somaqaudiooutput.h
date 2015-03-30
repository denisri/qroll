/***************************************************************************
                          somaqaudiooutput.h
                          -------------------
    begin                : 2015
    copyright            : (C) 2015 by Denis Rivière
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


#ifndef SOMA_SOUND_SOMAQAUDIOOUTPUT_H
#define SOMA_SOUND_SOMAQAUDIOOUTPUT_H

/*  WARNING: UNFINISHED...
    This sound class implementation was an attempt to
    try "something else" for the Android platform,
    where I couldn't make any other solution to work
    (alsa/oss not present, phonon not here also,
    and QSound did not work).
    But after all I realized that a typo in the qmake file
    resulted in sound files not being installed in the package,
    so when I fixed it, QSound happened to work perfectly.
    Thus, I gave up this QAudioOutput implementation...
 */

#include "somasoundprocessor.h"
#include <QObject>
#include <QAudio>

namespace soma
{

  /// sound processor implementation using QSound
  class SomaQAudioOutput : public QObject, public SomaSoundProcessor
  {
    Q_OBJECT

  public:
    SomaQAudioOutput();
    virtual ~SomaQAudioOutput();
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

  protected slots:
    void handleStateChanged( QAudio::State );

  private:
    struct Private;
    Private	*d;
  };

}

#endif // SOMA_SOUND_SOMAQAUDIOOUTPUT_H

