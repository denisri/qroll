/***************************************************************************
                          qsound.cc
                             -------------------
    begin                : 2022
    copyright            : (C) 2004-2022 by Denis Rivière
    email                : nudz@free.fr
                           http://nudz.free.fr
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "somaqsoundeffect.h"

#ifdef SOMA_SOUND_QSOUNDEFFECT

#include "soundbank.h"
#include "soundslot.h"
#include <QSoundEffect>
#if QT_VERSION >= 0x060000
#include <QAudioDevice>
#include <QMediaDevices>
#else
#include <QAudioDeviceInfo>
#endif
#include <memory>
#include <vector>

using namespace soma;
using namespace std;


struct SomaQSoundEffect::Private
{
  Private() : initialized( false ), device_ok( false ) {}

  vector<QSoundEffect *>  sounds;
  bool                    initialized;
  bool                    device_ok;
#if QT_VERSION >= 0x060000
  QAudioDevice            device;
#else
  QAudioDeviceInfo        device;
#endif
};


SomaQSoundEffect::SomaQSoundEffect()
  : SomaSoundProcessor(), d( new Private )
{
}


SomaQSoundEffect::~SomaQSoundEffect()
{
  unsigned	i, n = d->sounds.size();
  for( i=0; i<n; ++i )
    delete d->sounds[i];
  delete d;
}

void SomaQSoundEffect::process( int type )
{
  unsigned nsnd = soundBank().sounds().size();
  if( (unsigned) type >= nsnd || !isOK() )
    return;

  if( d->sounds.size() < nsnd )
  {
    unsigned i;
    string filename;
    d->sounds.reserve( nsnd );
    for( i=d->sounds.size(); i<nsnd; ++i )
    {
      filename = soundBank().sound( i ).filename;
      QSoundEffect *se = new QSoundEffect( d->device );
      se->setSource( QUrl::fromLocalFile( filename.c_str() ) );
      d->sounds.push_back( se );
    }
  }

  if( d->sounds[ type ] )
  {
    if( _inuse.size() <= (unsigned) type )
    {
      _inuse.reserve( nsnd );
      _inuse.insert( _inuse.end(), nsnd - _inuse.size(), 0 );
    }
    d->sounds[ type ]->play();
    cout << "play sound " << type << ", status: " << d->sounds[type]->status() << endl;
    ++_inuse[ type ];
  }
}


void SomaQSoundEffect::stop()
{
  if( !isOK() )
    return;

  unsigned	i, n = d->sounds.size();
  for( i=0; i<n; ++i )
    d->sounds[i]->stop();
}


void SomaQSoundEffect::stop( int type )
{
  if( !isOK() || d->sounds.size() <= type || _inuse.size() <= type )
    return;

  d->sounds[ type ]->stop();
  --_inuse[ type ];
}


void SomaQSoundEffect::close()
{
  stop();
}


bool SomaQSoundEffect::isOK() const
{
  const_cast<SomaQSoundEffect *>( this )->init();
  return d->initialized && d->device_ok;
}



void SomaQSoundEffect::init()
{
  if( d->initialized )
    return;
  d->initialized = true;

#if QT_VERSION >= 0x060000
  cout << "audio devices:\n";
  const auto deviceInfos = QMediaDevices::audioOutputs();
  for (const QAudioDevice &deviceInfo : deviceInfos)
    cout << "Device: " << deviceInfo.description().toStdString() << endl;
  cout << "default: " << QMediaDevices::defaultAudioOutput().description().toStdString() << endl;

  if( QMediaDevices::audioOutputs().size() == 0 )
    d->device_ok = false;
  else
  {
    d->device_ok = true;
    if( QMediaDevices::defaultAudioOutput().description() == "" )
      d->device = QMediaDevices::audioOutputs()[0];
    else
      d->device = QMediaDevices::defaultAudioOutput();
    cout << "using device: " << d->device.description().toStdString() << endl;
  }
#else
  cout << "audio devices:\n";
  const auto deviceInfos = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
  for (const QAudioDeviceInfo &deviceInfo : deviceInfos)
    cout << "Device: " << deviceInfo.deviceName().toStdString() << endl;
  cout << "default: " << QAudioDeviceInfo::defaultOutputDevice().deviceName().toStdString() << endl;

  if( QAudioDeviceInfo::availableDevices(QAudio::AudioOutput).size() == 0 )
    d->device_ok = false;
  else
  {
    d->device_ok = true;
    if( QAudioDeviceInfo::defaultOutputDevice().deviceName() == "" )
      d->device = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput)[0];
    else
      d->device = QAudioDeviceInfo::defaultOutputDevice();
    cout << "using device: " << d->device.deviceName().toStdString() << endl;
  }
#endif

  SomaSoundProcessor::init();
  unsigned	i, n = d->sounds.size();
  for( i=0; i<n; ++i )
    delete d->sounds[i];
  d->sounds.clear();
  loadSounds();
//   if( d->sounds.empty() )
//     d->initialized = false;
  cout << "initialized: " << d->initialized << endl;
}


void SomaQSoundEffect::loadSounds()
{
  unsigned	i, n = soundBank().sounds().size();
  string	filename;


  cout << "loadSounds: " << n << " to load\n";
  for( i=0; i<n; ++i )
  {
    filename = soundBank().sound( i ).filename;
    QSoundEffect *se = 0;
    if( d->sounds.size() <= i )
    {
      se = new QSoundEffect( d->device );
      d->sounds.push_back( se );
    }
    else if( !d->sounds[i] )
    {
      se = new QSoundEffect( d->device );
      d->sounds[i] = se;
    }
    if( se )
      se->setSource( QUrl::fromLocalFile( filename.c_str() ) );
  }
}


string SomaQSoundEffect::name() const
{
  return "Qt effect sound";
}


float SomaQSoundEffect::priorityRating() const
{
  return 20;
}

#endif

