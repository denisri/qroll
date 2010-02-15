/***************************************************************************
                          painter.h  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000-2002 by Denis Rivière
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


#ifndef QROLL_GUI_PAINTER_H
#define QROLL_GUI_PAINTER_H

#include <qobject.h>
#include <qpixmap.h>

class QRMainWin;
class QRPlayField;
class QWidget;

namespace roll
{
  struct SimpleLevel;

  class Painter
  {
  public:
    Painter( QRMainWin* parent ) : _parent( parent ) {}
    virtual ~Painter();

    virtual void start( int x, int y, bool inlevel, QRPlayField* source );
    virtual void move( int x, int y, bool inlevel, QRPlayField* source );
    virtual void stop( int x, int y, bool inlevel, QRPlayField* source );

    virtual QPixmap pixmap() const = 0;
    virtual QString name() const = 0;
    virtual QString description() const = 0;

  protected:
    QRMainWin  *_parent;
  };


  class DrawPainter : public Painter
  {
  public:
    DrawPainter( QRMainWin* parent );
    virtual ~DrawPainter();

    virtual void start( int x, int y, bool inlevel, QRPlayField* source );
    virtual void move( int x, int y, bool inlevel, QRPlayField* source );

    virtual QPixmap pixmap() const;
    virtual QString name() const;
    virtual QString description() const;
  };


  class FillPainter : public Painter
  {
  public:
    FillPainter( QRMainWin* parent );
    virtual ~FillPainter();

    virtual void start( int x, int y, bool inlevel, QRPlayField* source );

    virtual QPixmap pixmap() const;
    virtual QString name() const;
    virtual QString description() const;
  };


  class RectPainter : public QObject, public Painter
  {
    Q_OBJECT

  public:
    RectPainter( QRMainWin* parent );
    virtual ~RectPainter();

    virtual void start( int x, int y, bool inlevel, QRPlayField* source );
    virtual void move( int x, int y, bool inlevel, QRPlayField* source );
    virtual void stop( int x, int y, bool inlevel, QRPlayField* source );

    virtual QPixmap pixmap() const;
    virtual QString name() const;
    virtual QString description() const;

  public slots:
    void displayRect( QWidget* );

  protected:
    struct Private;
    Private  *d;
  };


  class CopyPainter : public RectPainter
  {
  public:
    CopyPainter( QRMainWin* parent );
    virtual ~CopyPainter();

    virtual void stop( int, int, bool, QRPlayField* );

    virtual QPixmap pixmap() const;
    virtual QString name() const;
    virtual QString description() const;

    static const SimpleLevel & buffer();

  private:
    struct Private;
    Private *c;
  };


  class PatternPainter : public Painter
  {
  public:
    PatternPainter( QRMainWin* );
    virtual ~PatternPainter();

    virtual void start( int, int, bool, QRPlayField* );
    virtual void move( int, int, bool, QRPlayField* );

    virtual QPixmap pixmap() const;
    virtual QString name() const;
    virtual QString description() const;
  };

}


#endif


