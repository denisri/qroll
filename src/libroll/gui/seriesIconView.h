/***************************************************************************
                          seriesIconView.h  -  description
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


#ifndef QROLL_GUI_SERIESICONVIEW_H
#define QROLL_GUI_SERIESICONVIEW_H

#include <qglobal.h>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMimeData>
#include <roll/struct/simpleLevel.h>
#include <set>
#include <map>

class SeriesIconView;
class QMimeSource;

namespace roll
{

  class LevelsDrag : public QMimeData
  {
  public:
    LevelsDrag( QWidget * dragSource = 0, const char * name = 0 );
    virtual ~LevelsDrag();
//     virtual const char* format ( int n = 0 ) const;
    virtual void append( const QIcon &, unsigned n, SimpleLevel* );

//     static bool canDecode( QMimeSource* );
    static bool decode( const QMimeData*, std::map<unsigned, SimpleLevel> & );
    virtual QByteArray encodedData( const char* ) const;

  private:
    std::map<unsigned, SimpleLevel>	_levels;
  };


  class SeriesIconViewItem : public QListWidgetItem
  {
  public:
    SeriesIconViewItem( SeriesIconView* parent, const QString & text,
                        const QIcon & icon );
    SeriesIconViewItem( SeriesIconView* parent, QListWidgetItem* after,
                        const QString & text, const QIcon & icon );
    virtual ~SeriesIconViewItem();

  protected:
    virtual void dragEnterEvent( QDragEnterEvent* e );
    virtual void dropEvent( QDropEvent* e );

  private:
    SeriesIconView	*_parent;
  };

}


class SeriesIconView : public QListWidget
{
  Q_OBJECT

  friend class SeriesArranger;
  friend class roll::SeriesIconViewItem;

public:
  SeriesIconView( QWidget * parent = 0 );
  virtual ~SeriesIconView();
  void levelsMoved( unsigned, const std::set<unsigned> & );
  void levelsDropped( QListWidgetItem *, QDropEvent* e );

signals:
  void dropped( QDropEvent * );

protected:
  virtual QMimeData* dragObject();
  virtual QMimeData* copySelection( QWidget* source = 0 );
  virtual void mousePressEvent( QMouseEvent* event );
  virtual void mouseMoveEvent( QMouseEvent* event );
  virtual void dragEnterEvent( QDragEnterEvent* e );
  virtual void dropEvent( QDropEvent* e );

private:
  QPoint _dragStart;
};


#endif


