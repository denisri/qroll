/***************************************************************************
                          qglGameField.h  -  description
                             -------------------
    begin                : 2001
    copyright            : (C) 2001 by Denis Rivière
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

#ifndef ROLL_GUI_QGLGAMEFIELD_H
#define ROLL_GUI_QGLGAMEFIELD_H

#ifndef RR_NO_OPENGL	// Optional: define this macro if you don't have OpenGL

#include <roll/gui/gameField.h>
#include <roll/gui/glcontext.h>
#include <qpixmap.h>

namespace roll
{
  struct QRGLGameField_Private;
}

class QRGLGameField : public GLWidget, public RGameField
{
  Q_OBJECT

public:
  enum GLMode
  {
    DrawPixels,
    Texture
  };

  QRGLGameField( const QPixmap * const * sprites, QWidget *parent, 
	       const char *name = 0 );
  virtual ~QRGLGameField();

  virtual int width() const { return( QGLWidget::width() ); }
  virtual int height() const { return( QGLWidget::height() ); }

  static void setGLMode( GLMode m ) { _glmode = m; }
  static bool glMode() { return( _glmode ); }
  virtual void displayFull();

public slots:
  ///	Dessine le tableau, cases entières
  void displayFullSlot();
  ///	Dessine le demi-scroll
  void displayHalfSlot();
  void resetSlot();

signals:
  void mousePress( QMouseEvent* );
  void mouseRelease( QMouseEvent* );
  void mouseMove( QMouseEvent* );
  void displayFullDone( QWidget* );

protected:
  virtual void keyPressedEvent( QKeyEvent* key );
  virtual void keyReleasedEvent( QKeyEvent* key );
  virtual void mousePressEvent( QMouseEvent* e );
  virtual void mouseReleaseEvent( QMouseEvent* e );
  virtual void mouseMoveEvent( QMouseEvent* e );

  virtual void initializeGL();
  virtual void resizeGL( int w, int h );
  virtual void paintGL();

  ///	Prépare l'écran ou le Pixmap dans lequel le tableau doit être dessiné
  virtual void setupScreen( unsigned w, unsigned h );
  ///	Dessine le sprite spr à la position (i, j) dans la zone de dessin
  virtual void copySprite( unsigned spr, int posx, int posy );
  ///	Affiche effectivement la zone de dessin
  virtual void updateScreen( bool eraseBorder, int w = 0, int h = 0 );
  ///	Converts sprites to OpenGL textures
  virtual void makeTextures();
  bool colorsChanged() const;
  void setColorsChanged( bool );

private:
  ///	pointeur sur les sprites du jeu
  const QPixmap		* const *_sprite;
  roll::QRGLGameField_Private	*d;
  static GLMode		_glmode;
};


#endif

#endif


