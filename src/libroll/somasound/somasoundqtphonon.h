/***************************************************************************
                          somasoundqtphonon.h
                             -------------------
    begin                : 2011
    copyright            : (C) 2011 by Denis Rivière
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


#ifndef SOMA_SOUND_SOMASOUNDQTPHONON_H
#define SOMA_SOUND_SOMASOUNDQTPHONON_H

#include "somasoundprocessor.h"
#include <qobject.h>

#ifdef QT_PHONON_LIB   // compiled only of QT_PHONON_LIB is defined

namespace soma
{

  /// sound processor implementation using Qt Phonon
  class SomaSoundQtPhonon : public QObject, public SomaSoundProcessor
  {
    Q_OBJECT

  public:
    /// Max number of sounds of the same type playing at the same time
    static const unsigned MaxSameSample;
    SomaSoundQtPhonon();
    virtual ~SomaSoundQtPhonon();
    virtual void process( int type );
    virtual void stop();
    virtual void stop( int type );
    /// Stops the oldest sound of the given type
    virtual void stopOld( int type );
    virtual void close();
    virtual bool isOK() const;
    virtual std::string name() const;
    /** The Qt Phonon Sound has priority 200.
     */
    virtual float priorityRating() const;

  protected:
    virtual void init();
    virtual void loadSounds();

  private:
    struct Private;
    Private     *d;

  public slots:
    void soundFinished();
  };

}

#endif
#endif
