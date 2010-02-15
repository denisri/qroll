/***************************************************************************
                          dsp.h  
                             -------------------
    begin                : 1999
    copyright            : (C) 1999-2004 by Denis Rivi�re
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


#ifndef ROLL_SOUND_DSP_H
#define ROLL_SOUND_DSP_H

#ifdef RR_DSP	// on comile seulement si RR_DSP est d�finie

#include <roll/sound/soundProcessor.h>
#include <list>
#include <pthread.h>


/**	Direct usage of /dev/dsp, using a threaded mixer: it implements 
   pseudo-real-time mixing of a varaible number of channels.
 */
class RRDspSound : public RRSoundProcessor
{
public:
  ///	Structure de requ�te son
  struct SndReq
  {
    ///	N� du son
    SNDLIST	type;
    ///	Position o� il en est dans sa lecture (n�gatif = pas commenc�)
    int		pos;
  };
  ///	Nb max de sons du m�me type en m�me temps
  static const unsigned MaxSameSample;
  RRDspSound();
  virtual ~RRDspSound();
  virtual void process( SNDLIST type );
  virtual void stop();
  virtual void stop( SNDLIST type );
  virtual void close();
  virtual void disable() { RRSoundProcessor::disable(); ok = false; }
  virtual unsigned inuse( SNDLIST type );
  int dspFD() const { return( fd ); }
  bool isOK() const { return( ok ); }
  virtual std::string name() const;
  /** The DSP sound has priority 100 (I consider it rather good for its 
      mixing capabilities)
  */
  virtual float priorityRating() const;

protected:
  ///	file descriptor du device (/dev/dsp)
  int	fd;
  ///	flag qui dit si le driver est OK (sinon on ne fait pas de son)
  bool	ok;
  ///	Taille du buffer du dsp
  int	bufferSize;
  ///	Buffer en question
  unsigned char	*buffer;
  ///	M�moires pour les sons eux-m�mes
  unsigned char	*sounds[NO_SOUND];
  ///	Leurs tailles
  unsigned sndLen[NO_SOUND];
  ///	Liste des sons en cours
  std::list<SndReq> jobs;
  ///	Mutex correspondant
  pthread_mutex_t listLock;
  ///	ID du thread de mixage des sons
  pthread_t	updateThrd;
  bool		threadRunning;
  ///	Fr�quence de lecture des sons
  unsigned	freqDsp;
  virtual void init();
  virtual void loadSounds();
  ///	Fonction "callback" thread de mixage (statique)
  static void * updateThread( void * );
  ///	Fonction de mixage non-statique, appel�e par le thread
  void update();
  ///	Arr�te le plus ancien son du type donn� (Warning : SANS MUTEX !!)
  virtual void stopOld( SNDLIST type );
};


#endif	// #ifdef RR_DSP
#endif
