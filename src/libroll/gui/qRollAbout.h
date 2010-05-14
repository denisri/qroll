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


#ifndef ROLL_GUI_QROLLABOUT_H
#define ROLL_GUI_QROLLABOUT_H


#include <roll/gui/qAbout.h>


class QRollAbout : public QAbout
{
  Q_OBJECT

public:
  QRollAbout( QWidget *parent = 0, const char* name = 0 );
  virtual ~QRollAbout();

protected:
  virtual void music();
};


#endif

