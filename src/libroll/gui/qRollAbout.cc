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
#include "../somasound/somasoundprocessor.h"

using namespace soma;


QRollAbout::QRollAbout( QWidget *parent, const char* name )
  : QAbout( parent, name )
{
}


QRollAbout::~QRollAbout()
{
  SomaSoundProcessor & ssound = SomaSoundProcessor::processor();
  ssound.enable();
}


void QRollAbout::music()
{
  SomaSoundProcessor & ssound = SomaSoundProcessor::processor();
  ssound.disable();

  QAbout::music();
}


