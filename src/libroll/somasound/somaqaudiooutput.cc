/***************************************************************************
                          qsound.cc
                             -------------------
    begin                : 2004
    copyright            : (C) 2004-2006 by Denis Rivière
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

#include "somaqaudiooutput.h"
#include "soundbank.h"
#include "soundslot.h"
#include <QAudioOutput>
#include <QBuffer>
#include <QtGlobal>
#include <QDebug>

using namespace soma;
using namespace std;
using namespace audiq;


struct SomaQAudioOutput::Private
{
  Private() : initialized( false ), audiodeviceinfo( 0 )
  {}

  vector<QAudioOutput *>	sounds;
  bool			initialized;
  QAudioDeviceInfo *audiodeviceinfo;
};


SomaQAudioOutput::SomaQAudioOutput()
  : SomaSoundProcessor(), d( new Private )
{
}


SomaQAudioOutput::~SomaQAudioOutput()
{
  unsigned	i, n = d->sounds.size();
  for( i=0; i<n; ++i )
    delete d->sounds[i];
  delete d;
}

void SomaQAudioOutput::process( int type )
{
  unsigned nsnd = soundBank().sounds().size();
  if( (unsigned) type >= nsnd || !isOK() )
    return;

  SoundSlot & slot = soundBank().sound( type );
  WavHeader & hdr = slot.compressinfo.hdr;
  QAudioFormat format;
  format.setSampleRate( hdr.rate );
  qDebug( ("rate: " + QString::number(hdr.rate)).toStdString().c_str() );
  qDebug( ("channels: " + QString::number(hdr.channels)).toStdString().c_str() );
  qDebug( ("sample size: " + QString::number(hdr.sampleSize)).toStdString().c_str() );
  qDebug( ("signed: " + QString::number(hdr.sign)).toStdString().c_str() );

  format.setChannelCount( hdr.channels );
  format.setCodec( "audio/pcm" );
#if Q_BYTE_ORDER == Q_BIG_ENDIAN
  format.setByteOrder( QAudioFormat::BigEndian );
#else
  format.setByteOrder( QAudioFormat::LittleEndian );
#endif
//  if( hdr.sign )
    format.setSampleType( QAudioFormat::SignedInt );
//  else
//    format.setSampleType( QAudioFormat::UnSignedInt );
  format.setSampleSize( hdr.sampleSize * 8 );

  QAudioOutput *audio = new QAudioOutput( format, this );
  QByteArray *array = new QByteArray( (const char *) &(slot.buffer[0]), (int) slot.buffer.size() );
  QBuffer *buffer = new QBuffer( array, this );
  connect( audio, SIGNAL( stateChanged( QAudio::State ) ), this,
           SLOT( handleStateChanged( QAudio::State ) ) );
  audio->start( buffer );
}


void SomaQAudioOutput::stop()
{
  if( !isOK() )
    return;

  unsigned	i, n = d->sounds.size();
  for( i=0; i<n; ++i )
    d->sounds[i]->stop();
}


void SomaQAudioOutput::stop( int type )
{
  if( !isOK() || (int) d->sounds.size() <= type || (int) _inuse.size() <= type )
    return;
}


void SomaQAudioOutput::close()
{
  stop();
}


bool SomaQAudioOutput::isOK() const
{
  const_cast<SomaQAudioOutput *>( this )->init();
  return d->initialized;
}


void SomaQAudioOutput::init()
{
  if( d->initialized )
    return;
  d->initialized = true;

  qDebug( "Initializing QAudioOutout" );
  SomaSoundProcessor::init();
  loadSounds();
  d->audiodeviceinfo = new QAudioDeviceInfo( QAudioDeviceInfo::defaultOutputDevice() );
  qDebug( ("audio device isNull: " + QString::number( d->audiodeviceinfo->isNull() )).toStdString().c_str() );
  qDebug( "supported codecs: " );
  foreach ( QString codec, d->audiodeviceinfo->supportedCodecs())
  {
    qDebug( ("    " + codec).toStdString().c_str() );
  }
  qDebug( "supported rates:" );
  foreach ( int rate, d->audiodeviceinfo->supportedSampleRates() )
  {
    qDebug( ("    " + QString::number(rate)).toStdString().c_str() );
  }
  qDebug( "supported channels: " );
  foreach ( int channel, d->audiodeviceinfo->supportedChannelCounts() )
  {
    qDebug( ("    " + QString::number(channel)).toStdString().c_str() );
  }
  qDebug( "supported byte orders: " );
  foreach ( QAudioFormat::Endian endian, d->audiodeviceinfo->supportedByteOrders() )
  {
    if( endian == QAudioFormat::BigEndian )
      qDebug( "    BigEndian" );
    else
      qDebug( "    LittleEndian" );
  }
  qDebug( "supported sample size: " );
  foreach ( int ssize, d->audiodeviceinfo->supportedSampleSizes() )
  {
    qDebug( ("    " + QString::number(ssize)).toStdString().c_str() );
  }
  qDebug( "supported sample types: " );
  foreach ( QAudioFormat::SampleType stype, d->audiodeviceinfo->supportedSampleTypes() )
  {
    if( stype == QAudioFormat::UnSignedInt )
      qDebug( "    unsigned int" );
    else if( stype == QAudioFormat::SignedInt )
      qDebug( "    signed int" );
    else
      qDebug( ("    " + QString::number(stype)).toStdString().c_str() );
  }

  QAudioFormat format;
  format.setSampleRate( 8000 );
  format.setChannelCount( 1 );
  format.setCodec( "audio/pcm" );
#if Q_BYTE_ORDER == Q_BIG_ENDIAN
  qDebug( "I'm big endian" );
  format.setByteOrder( QAudioFormat::BigEndian );
#else
  qDebug( "I'm little endian" );
  format.setByteOrder( QAudioFormat::LittleEndian );
#endif
  format.setSampleType( QAudioFormat::SignedInt );
  format.setSampleSize( 8 );
  if( !d->audiodeviceinfo->isFormatSupported( format ) )
  {
    qWarning() << "Raw audio format not supported by backend, cannot play audio.";
    return;
  }
  qDebug( "SomaQAudioOutput initialized." );
}


void SomaQAudioOutput::loadSounds()
{
  unsigned	i, n = soundBank().sounds().size();
}


string SomaQAudioOutput::name() const
{
  return "QAudioOutput sound";
}


float SomaQAudioOutput::priorityRating() const
{
  return 15;
}


void SomaQAudioOutput::handleStateChanged( QAudio::State state )
{
}

