/***************************************************************************
                          seriesArranger.h  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2001 by Denis Rivi�e
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
#if QT_VERSION >= 0x040000
#include <q3valuelist.h>
#include <Q3IconDragItem>
#include <Q3IconViewItem>
typedef Q3IconDragItem QIconDragItem;
typedef Q3IconViewItem QIconViewItem;
typedef Q3ValueList<QIconDragItem> QValueList_QIconDragItem;
#else
#include <qvaluelist.h>
class QIconDragItem;
class QIconViewItem;
typedef QValueList<QIconDragItem> QValueList_QIconDragItem;
// these types are just to trick MOC
typedef QIconDragItem Q3IconDragItem;
typedef QIconViewItem Q3IconViewItem;
template <typename T> class Q3ValueList : public QValueList<T>
{
};
#endif
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
                  Qt::WFlags f = Qt::WType_TopLevel | Qt::WDestructiveClose );
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
#if QT_VERSION >= 0x040000
  void changeLevel( Q3IconViewItem* );
  void levelsDropped( QDropEvent*, const Q3ValueList<Q3IconDragItem> & );
#else
  void changeLevel( QIconViewItem* );
  void levelsDropped( QDropEvent*, const QValueList<QIconDragItem> & );
#endif

private:
  void insertLevels( unsigned pos, 
		     const std::map<unsigned, roll::SimpleLevel> & levels, 
		     QIconViewItem* before );

  static SeriesArranger		*_theSeriesArranger;
  SeriesArranger_Private	*d;
};


#endif

