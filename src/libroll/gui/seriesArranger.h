/***************************************************************************
                          seriesArranger.h  -  description
                             -------------------
    begin                : 1999
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


#ifndef QROLL_GUI_SERIESARRANGER_H
#define QROLL_GUI_SERIESARRANGER_H

#include <qmainwindow.h>
#include <QMimeData>
#include <QListWidgetItem>
#include <map>


namespace roll
{
  struct SimpleLevel;
}


class SeriesArranger : public QMainWindow
{
  Q_OBJECT

public:
  struct SeriesArranger_Private;

  SeriesArranger( QWidget * parent = 0, const char * name = 0, 
                  Qt::WindowFlags f = Qt::Window );
  virtual ~SeriesArranger();

  QPixmap levelPixmap( const roll::SimpleLevel & sl );

  static SeriesArranger	*instance() { return( _theSeriesArranger ); }

signals:
  // void arrangerDestroyed();
  void seriesChanged();
  void levelChanged( unsigned );

public slots:
  void updateView();
  void copy();
  void cut();
  void paste();
  void deleteSelection();
  void zoomMore();
  void zoomLess();

private slots:
  void changeLevel( QListWidgetItem* );
  void levelsDropped( QDropEvent* );

private:
  void insertLevels( unsigned pos, 
		     const std::map<unsigned, roll::SimpleLevel> & levels, 
		     QListWidgetItem* before );

  static SeriesArranger		*_theSeriesArranger;
  SeriesArranger_Private	*d;
};


#endif

