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


#ifndef ROLL_SOUND_ALSAPROCESSOR_H
#define ROLL_SOUND_ALSAPROCESSOR_H

#ifdef RR_ALSA   // on compile seulement si RR_ALSA est définie

#include <roll/sound/soundProcessor.h>
#include <list>
#include <pthread.h>


/**     Sound using ALSA lib, using a threaded mixer: it implements
   pseudo-real-time mixing of a varaible number of channels.
 */
class RRAlsaSound : public RRSoundProcessor
{
public:
  ///   Structure de requête son
  struct SndReq
  {
    /// N° du son
    SNDLIST     type;
    /// Position où il en est dans sa lecture (négatif = pas commencé)
    int         pos;
  };
  ///   Nb max de sons du même type en même temps
  static const unsigned MaxSameSample;
  RRAlsaSound();
  virtual ~RRAlsaSound();
  virtual void process( SNDLIST type );
  virtual void stop();
  virtual void stop( SNDLIST type );
  virtual void close();
  virtual void disable() { RRSoundProcessor::disable(); ok = false; }
  virtual unsigned inuse( SNDLIST type );
  bool isOK() const { return( ok ); }
  virtual std::string name() const;
  /** The ALSA sound has priority 150 (better than old OSS/DSP API)
  */
  virtual float priorityRating() const;

protected:
  ///   flag qui dit si le driver est OK (sinon on ne fait pas de son)
  bool  ok;
  ///   Taille du buffer du dsp
  int   bufferSize;
  ///   Buffer en question
  unsigned char *buffer;
  ///   Mémoires pour les sons eux-mêmes
  unsigned char *sounds[NO_SOUND];
  ///   Leurs tailles
  unsigned sndLen[NO_SOUND];
  ///   Liste des sons en cours
  std::list<SndReq> jobs;
  ///   Mutex correspondant
  pthread_mutex_t listLock;
  ///   ID du thread de mixage des sons
  pthread_t     updateThrd;
  bool          threadRunning;
  ///   Fréquence de lecture des sons
  unsigned      freqDsp;
  virtual void init();
  virtual void loadSounds();
  ///   Fonction "callback" thread de mixage (statique)
  static void * updateThread( void * );
  ///   Fonction de mixage non-statique, appelée par le thread
  void update();
  ///   Arrête le plus ancien son du type donné (Warning : SANS MUTEX !!)
  virtual void stopOld( SNDLIST type );

private:
  struct Private;
  Private *d;
};


#endif  // #ifdef RR_ALSA
#endif
