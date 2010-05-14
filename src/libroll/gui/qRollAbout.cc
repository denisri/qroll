/***************************************************************************
                          qAbout.h  -  description
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

#include <roll/gui/qRollAbout.h>
#include <roll/sound/dsp.h>

QRollAbout::QRollAbout( QWidget *parent, const char* name )
  : QAbout( parent, name )
{
}


QRollAbout::~QRollAbout()
{
#ifdef RR_DSP
  RRDspSound        *dsp
    = dynamic_cast<RRDspSound *>( &RRSoundProcessor::processor() );

  if( dsp )
    dsp->enable();
#endif // RR_DSP
}


void QRollAbout::music()
{
#if !defined( ABOUT_NO_SOUND ) && !defined( _WIN32 ) && defined( RR_DSP )
  RRDspSound    *dsp
    = dynamic_cast<RRDspSound *>( &RRSoundProcessor::processor() );

  if( dsp )
    dsp->disable();
#endif

  QAbout::music();
}


