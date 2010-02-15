/***************************************************************************
                          qAbout.h  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 1999-2004 by Denis Riviï¿½e
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


#ifndef ROLL_GUI_QABOUT_H
#define ROLL_GUI_QABOUT_H


#include <qdialog.h>


class QAbout : public QDialog
{
  Q_OBJECT

public:
  QAbout( QWidget *parent = 0, const char* name = 0 );
  virtual ~QAbout();

public slots:
  void nextLine();

protected:
  void keyPressEvent( QKeyEvent* kev );

  static void * musicThread( void* caller );
  void music();

private:
  struct Private;
  Private	*d;
};


#endif

