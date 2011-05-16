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

#include <q3iconview.h>
#include <Q3IconDrag>
#include <Q3IconDragItem>
#include <q3valuelist.h>
typedef Q3IconView QIconView;
typedef Q3IconViewItem QIconViewItem;
typedef Q3IconDrag QIconDrag;
typedef Q3IconDragItem QIconDragItem;
typedef Q3ValueList<QIconDragItem> QValueList_QIconDragItem;
typedef Q3DragObject QDragObject;
#include <roll/struct/simpleLevel.h>
#include <set>
#include <map>

#ifndef QROLL_OLD
#define QROLL_OLD
#endif


class SeriesIconView;

namespace roll
{

#ifdef QROLL_OLD
  class LevelsDrag : public QIconDrag
#else
  class LevelsDrag : public QMimeData
#endif
  {
  public:
    LevelsDrag( QWidget * dragSource = 0, const char * name = 0 );
    virtual ~LevelsDrag();
    virtual const char* format ( int n = 0 ) const;
    virtual void append( const QIconDragItem &, unsigned n, SimpleLevel*,
			 const QRect &, const QRect & );

    static bool canDecode( QMimeSource* );
    static bool decode( QMimeSource*, std::map<unsigned, SimpleLevel> & );
    virtual QByteArray encodedData( const char* ) const;

  private:
    std::map<unsigned, SimpleLevel>	_levels;
  };


#ifdef QROLL_OLD
  class SeriesIconViewItem : public QIconViewItem
#else
  class SeriesIconViewItem : public QListWidgetItem
#endif
  {
  public:
#ifdef QROLL_OLD
    SeriesIconViewItem( SeriesIconView* parent, const QString & text,
                        const QPixmap & icon );
    SeriesIconViewItem( SeriesIconView* parent, QIconViewItem* after,
                        const QString & text, const QPixmap & icon );
#else
    SeriesIconViewItem( SeriesIconView* parent, const QString & text,
                        const QIcon & icon );
    SeriesIconViewItem( SeriesIconView* parent, QIconViewItem* after,
                        const QString & text, const QIcon & icon );
#endif
    virtual ~SeriesIconViewItem();

    virtual bool acceptDrop( const QMimeSource * mime ) const;

  protected:
    virtual void dropped( QDropEvent* e, 
			  const QValueList_QIconDragItem & lst );

  private:
    SeriesIconView	*_parent;
  };

}


#ifdef QROLL_OLD
class SeriesIconView : public QIconView
#else
class SeriesIconView : public QListWidget
#endif
{
  Q_OBJECT

  friend class SeriesArranger;
  friend class roll::SeriesIconViewItem;

public:
#ifdef QROLL_OLD
  SeriesIconView( QWidget * parent = 0, const char * name = 0,
                  Qt::WFlags f = 0 );
#else
  SeriesIconView( QWidget * parent = 0 );
#endif
  virtual ~SeriesIconView();
  void levelsMoved( unsigned, const std::set<unsigned> & );
  void levelsDropped( int index, QDropEvent* e, 
		      const QValueList_QIconDragItem & lst );

protected:
  virtual QDragObject* dragObject();
  virtual QDragObject* copySelection( QWidget* source = 0 );
};


#endif


