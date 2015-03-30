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

#ifdef QT_PHONON_LIB   // compiled only of QT_PHONON_LIB is defined

#include "somasoundqtphonon.h"
#include "soundbank.h"
#include "soundslot.h"
#include <phonon/mediaobject.h>
#include <QBuffer>
#include <vector>
#include <map>
#include <list>

using namespace soma;
using namespace std;

const unsigned SomaSoundQtPhonon::MaxSameSample = 3;

struct SomaSoundQtPhonon::Private
{
  Private() : initialized( false ) {}

  vector<QByteArray *> sounddata;
  vector<QBuffer *> soundbank;
  vector<Phonon::MediaSource *> sounds;
  bool            initialized;
  map<int, list<Phonon::MediaObject *> > running;
};


SomaSoundQtPhonon::SomaSoundQtPhonon()
  : SomaSoundProcessor(), d( new Private )
{
  qDebug( "SomaSoundQtPhonon instantiated\n" );
}


SomaSoundQtPhonon::~SomaSoundQtPhonon()
{
  stop();
  vector<Phonon::MediaSource *>::iterator is, es = d->sounds.end();
  for( is=d->sounds.begin(); is!=es; ++is )
    delete *is;
  vector<QBuffer *>::iterator ib, eb = d->soundbank.end();
  for( ib=d->soundbank.begin(); ib!=eb; ++ib )
    delete *ib;
  vector<QByteArray *>::iterator id, ed = d->sounddata.end();
  for( id=d->sounddata.begin(); id!=ed; ++id )
    delete *id;
  delete d;
}


namespace
{

  Phonon::MediaObject* getOldest( int type,
                                  map<int, list<Phonon::MediaObject *> >
                                    & running )
  {
    list<Phonon::MediaObject *> & sm = running[ type ];
    Phonon::MediaObject * oldest = 0;
    list<Phonon::MediaObject *>::iterator i, e = sm.end();
    qint64 maxlen = 0;
    for( i=sm.begin(); i!=e; ++i )
      if( (*i)->state() == Phonon::StoppedState
        || (*i)->state() == Phonon::ErrorState )
        return *i;
    for( i=sm.begin(); i!=e; ++i )
      if( (*i)->currentTime() == 0 )
        return *i;
      else if( (*i)->currentTime() > maxlen )
      {
        maxlen = (*i)->currentTime();
        oldest = *i;
      }
    return oldest;
  }

}


void SomaSoundQtPhonon::process( int type )
{
  unsigned nsnd = d->sounds.size();
  if( (unsigned) type >= nsnd || !isOK() )
    return;

  if( _inuse.size() <= (unsigned) type )
  {
    _inuse.reserve( nsnd );
    _inuse.insert( _inuse.end(), nsnd - _inuse.size(), 0 );
  }
  Phonon::MediaObject *mo = getOldest( type, d->running );
  if( _inuse[ type ] >= MaxSameSample )
    stopOld( type );
  if( _inuse[ type ] > 0 )
  {
    // this one already plays : setup a little delay
    // TODO
  }
  cout << "play, mo: " << mo << endl;
  if( mo )
    cout << "state: " << mo->state() << ", time: " << mo->currentTime() << " / " << mo->totalTime() << endl;
  if( !mo
    || ( _inuse[ type ] < MaxSameSample
      && ( mo->state() == Phonon::PlayingState
        || mo->state() == Phonon::LoadingState ) ) )
  {
    mo = Phonon::createPlayer( Phonon::GameCategory, *d->sounds[ type ] );
    connect( mo, SIGNAL( finished() ), this, SLOT( soundFinished() ) );
    d->running[ type ].push_back( mo );
    cout << "created player " << mo << endl;
  }
  else
  {
    cout << "reusing existing mediaobject\n";
    mo->clear();
    cout << "cleared\n";
    mo->setCurrentSource( *d->sounds[ type ] );
    cout << "enqueued\n";
  }
  cout << "playing\n";
  mo->play();
  cout << "play started\n";
  ++_inuse[ type ];
}


void SomaSoundQtPhonon::stop()
{
  if( !isOK() )
    return;

  map<int, list<Phonon::MediaObject *> >::iterator ir, er = d->running.end();
  list<Phonon::MediaObject *>::iterator im, em;
  for( ir=d->running.begin(); ir!=er; ++ir )
    for( im=ir->second.begin(), em=ir->second.end(); im!=em; ++im )
    {
      (*im)->clear();
//       delete *im; // FIXME: memory leak !!
    }
//   d->running.clear();
  std::vector<unsigned>::iterator i, e = _inuse.end();
  for( i=_inuse.begin(); i!=e; ++ i )
    *i = 0;
}


void SomaSoundQtPhonon::stopOld( int type )
{
  if( !isOK() || d->sounds.size() <= type || _inuse.size() <= type )
    return;

  if( _inuse[ type ] == 0 )
    return;
  cout << "stopping\n";
  Phonon::MediaObject * oldest = getOldest( type, d->running );
  if( !oldest )
    return;
  oldest->clear();
//   cout << "stop 1\n";
//   // delete *sm.begin(); // FIXME: memory leak !!
//   cout << "erased 1\n";
//   sm.erase( sm.begin() );
//   if( sm.empty() )
//     d->running.erase( type );
  --_inuse[ type ];
  cout << "stopped\n";
}


void SomaSoundQtPhonon::stop( int type )
{
  if( !isOK() || d->sounds.size() <= type || _inuse.size() <= type )
    return;

  while( _inuse[ type ] != 0 )
    stopOld( type );
}


void SomaSoundQtPhonon::close()
{
  stop();
}


bool SomaSoundQtPhonon::isOK() const
{
  const_cast<SomaSoundQtPhonon *>( this )->init();
  return true;
}


void SomaSoundQtPhonon::init()
{
  if( d->initialized )
    return;
  d->initialized = true;

  SomaSoundProcessor::init();
  unsigned      i, n = d->sounds.size();
  d->sounds.clear();
  loadSounds();
  if( d->sounds.empty() )
    d->initialized = false;
}


void SomaSoundQtPhonon::loadSounds()
{
  unsigned      i, n = soundBank().sounds().size();
  string        filename;

  SoundBank & sb = soundBank();
  for( i=0; i<n; ++i )
  {
    filename = soundBank().sound( i ).filename;
    ifstream f( filename.c_str(), ios::in | ios::binary );
    f.seekg( 0, ios::end );
    size_t sz = f.tellg();
    f.seekg( 0, ios::beg );
    if( d->sounds.size() <= i )
    {
      d->sounddata.push_back( new QByteArray( sz, 0 ) );
      d->soundbank.push_back( new QBuffer( d->sounddata[i] ) );
    }
    else
      d->sounddata[i]->resize( sz );
    f.read( d->sounddata[i]->data(), sz );
    if( d->sounds.size() <= i )
      d->sounds.push_back( new Phonon::MediaSource( d->soundbank[i] ) );
  }
}


string SomaSoundQtPhonon::name() const
{
  return "Qt Phonon sound";
}


float SomaSoundQtPhonon::priorityRating() const
{
  return 200;
}


void SomaSoundQtPhonon::soundFinished()
{
  cout << "soundFinished\n";
  map<int, list<Phonon::MediaObject *> >::iterator ir, er = d->running.end();
  list<Phonon::MediaObject *>::iterator im, em; //, im2;
  for( ir=d->running.begin(); ir!=er; ++ir )
    for( im=ir->second.begin(), em=ir->second.end(); im!=em; )
    {
      cout << "remainingTime " << ir->first << " : " << (*im)->remainingTime() << ", currenttime: " << (*im)->currentTime() << ", total: " << (*im)->totalTime() << ", state: " << (*im)->state() << endl;
      if( (*im)->state() == Phonon::StoppedState
        || (*im)->state() == Phonon::ErrorState
        || (*im)->currentTime() == 0 ) // last cond is strange...
      {
//         (*im)->clear();
        --_inuse[ ir->first ];
//         im2 = im;
        ++im;
        // FIXME; delete
//         cout << "erasing MediaObject\n";
//         delete *im2;
//         ir->second.erase( im2 );
      }
      else
        ++im;
    }
}

#endif

