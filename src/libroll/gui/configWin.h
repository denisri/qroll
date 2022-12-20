/***************************************************************************
                          configWin.h  -  description
                             -------------------
    begin                : 2001
    copyright            : (C) 2001 by Denis Riviï¿½e
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

#ifndef ROLL_GUI_CONFIGWIN
#define ROLL_GUI_CONFIGWIN

#include <qwidget.h>


namespace roll
{
  struct QRConfigWin_Private;
}


/// Configuration window
class QRConfigWin : public QWidget
{
  Q_OBJECT

public:
  QRConfigWin( QWidget* parent = 0, const char* name = 0, 
               Qt::WindowFlags f = Qt::WindowFlags() );
  virtual ~QRConfigWin();

public slots:
  void rollExit( bool );
  void useOpenGL( bool );
  void glModeChanged( int );
  void useCustomTiming( bool );
  void setTiming( int );
  void saveConfig();

signals:
  void closed();

protected:

private:
  roll::QRConfigWin_Private	*d;
};


#endif

