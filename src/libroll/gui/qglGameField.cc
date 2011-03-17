
/***************************************************************************
                          qglGameField.cc  -  description
                             -------------------
    begin                : 2001
    copyright            : (C) 2001 by Denis RiviÃ¯Â¿Â½e
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

#ifndef RR_NO_OPENGL

//#define RR_DEBUG

#include <roll/game/vars.h>
#include <roll/gui/qglGameField.h>
#include <roll/game/constantes.h>
#include <qtimer.h>
#include <qpainter.h>
#include <qimage.h>
#include <qevent.h>
#if defined(__APPLE__)
/* OpenGL on Mac uses non-standard include paths, it would have been
   too simple and too much like all other systems, they definitely
   needed to "think different"... (even Windows is more standard !)
*/
# include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#include <iostream>
#include <math.h>

using namespace roll;
using namespace std;


QRGLGameField::GLMode	QRGLGameField::_glmode = QRGLGameField::DrawPixels;

static map<unsigned short, QImage>	glsprites;
static GLuint				*gltexmap = 0, *gltexmap2 = 0;
static QGLWidget			*sharedwid = 0;

#ifdef RR_DEBUG
// temp, debug
static bool				nonres[240+256];
static bool				usedres[240+256];
#endif


namespace roll
{

  struct QRGLGameField_Private
  {
    QRGLGameField_Private();

    bool		colorsChanged;
    WorkLevel::RGB	color[6];
  };

  QRGLGameField_Private::QRGLGameField_Private() : colorsChanged( true ) {}

}


QRGLGameField::QRGLGameField( const QPixmap * const * sprites, 
			      QWidget *parent, const char *name ) 
  : GLWidget( parent, name, sharedwid ), RGameField(), 
    _sprite( sprites ), d( new QRGLGameField_Private )
{
  resize( 512, 384 );
  setBackgroundMode( Qt::NoBackground );
  if( !sharedwid )
    sharedwid = new QGLWidget( 0, "shared widget", this );
}


QRGLGameField::~QRGLGameField()
{
  delete d;
}


void QRGLGameField::resizeGL( int, int )
{
  //cout << "resizeGL\n";
  displayFull();
}


void QRGLGameField::keyPressedEvent( QKeyEvent* key )
{
  cout << "GameField keyPressed\n";
  key->ignore();
}


void QRGLGameField::keyReleasedEvent( QKeyEvent* key )
{
  cout << "GameField keyReleased\n";
  key->ignore();
}


void QRGLGameField::displayFull()
{
  RGameField::displayFull();
  emit displayFullDone( this );
}


void QRGLGameField::displayFullSlot()
{
  displayFull();
}


void QRGLGameField::displayHalfSlot()
{
  displayHalf();
}


void QRGLGameField::resetSlot()
{
  reset();
}


void QRGLGameField::setupScreen( unsigned, unsigned )
{
  makeCurrent();
  makeTextures();
  glClear( GL_COLOR_BUFFER_BIT );

  float	sc = scaleFactor();

  if( _glmode == DrawPixels && scalingEnabled() )
    {
      glPixelZoom( sc, sc );
    }
  else
    glPixelZoom( 1, 1 );

  /*glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glOrtho( 0, 0, w, h, 0.5, 1 );
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();*/

  if( scalingEnabled() )
  glViewport( 0, height() - (GLint) ( sHeight() * 32 * sc ),
              (GLint) ( sWidth() * 32 * sc ),
              (GLint) ( sHeight() * 32 * sc ) );
  else
    glViewport( 0, height() - (GLint) ( sHeight() * 32 * sc ),
              (GLint) ( sWidth() * 32 * sc ),
              (GLint) ( sHeight() * 32 * sc ) );
//     glViewport( 0, height() & 31 ? ( height() & 31 ) - 32 : 0,
//                 ( ( width() >> 5 ) << 5 ) + ( width() & 31 ? 32 : 0 ),
//                 ( ( height() >> 5 ) << 5 ) + ( height() & 31 ? 32 : 0 ) );

  if( _glmode == Texture )
    {
      glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glEnable( GL_TEXTURE_2D );
    }
  else
    {
      glDisable( GL_TEXTURE_2D );
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glPixelStoref( GL_UNPACK_SKIP_PIXELS, 0 );
      glPixelStoref( GL_UNPACK_SKIP_ROWS, 0 );
    }
}


void QRGLGameField::copySprite( unsigned spr, int posx, int posy )
{
  float	x = float( posx ) / (16 * sWidth()) - 1.;
  float	y = 1. - float( posy+32 ) / (16 * sHeight());
  float	sx = 2.F / sWidth();
  float	sy = 2.F / sHeight();

  if( _glmode == Texture )
    {
      glBindTexture( GL_TEXTURE_2D, gltexmap[spr] );
      glBegin( GL_QUADS );
      glTexCoord2f( 0, 0 );
      glVertex2f( x, y );
      glTexCoord2f( 0, 1 );
      glVertex2f( x, y+sy );
      glTexCoord2f( 1, 1 );
      glVertex2f( x+sx, y+sy );
      glTexCoord2f( 1, 0 );
      glVertex2f( x+sx, y );
      glEnd();

#ifdef RR_DEBUG
      /*GLint	tr = 0;
	glGetTexParameteriv( GL_TEXTURE_2D, GL_TEXTURE_RESIDENT, &tr );
	cout << tr << " ";*/

      GLboolean	tr2 = GL_FALSE;
      glAreTexturesResident( 1, &gltexmap[spr], &tr2 );

      if( tr2 )
	usedres[spr] = true;
      else
	nonres[spr] = true;
#endif
    }
  else
    {
      glRasterPos2f( x, y );
      glDrawPixels( 32, 32, GL_RGBA, GL_UNSIGNED_BYTE, glsprites[spr].bits() );
    }
}


void QRGLGameField::updateScreen( bool, int, int )
{
#ifdef RR_DEBUG
  if( _glmode == Texture )
    {
      GLboolean	res[256+240];
      unsigned	i, n=0;

      glAreTexturesResident( 240*2+1, gltexmap2, res );
      for( i=0; i<240*2+1; ++i )
	if( res[i] )
	  ++n;
      cout << "resident textures : " << n << " ( ";
      for( i=0; i<240*2+1; ++i )
	if( res[i] )
	  cout << ( i<240 ? i : ( i==240 ? 255 : i+15) ) << " ";

      unsigned	m = 0;
      for( i=0; i<256+240; ++i )
	if( nonres[i] )
	  ++m;
      cout << "), used non-res : " << m << " ( ";
      for( i=0; i<256+240; ++i )
	if( nonres[i] )
	  cout << i << " ";
      cout << "), used res : ";
      for( i=0; i<256+240; ++i )
	if( usedres[i] )
	  cout << i << " ";
      cout << ")\n";
    }
#endif

  swapBuffers();
}


void QRGLGameField::initializeGL()
{
  glShadeModel(GL_FLAT);
  glDisable( GL_DEPTH_TEST );
  glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
  glPixelStoref( GL_UNPACK_SKIP_PIXELS, 0 );
  glPixelStoref( GL_UNPACK_SKIP_ROWS, 0 );

  makeTextures();
}


void QRGLGameField::makeTextures()
{
  unsigned	i, n = 240;
  GLenum	status;

  if( !gltexmap )
    {
      gltexmap = new GLuint[ 256 + n ];
      gltexmap2 = new GLuint[ n*2+1 ];
      glGenTextures( n*2+1, gltexmap2 );

      for( i=0; i<n; ++i )
	{
	  gltexmap[i] = gltexmap2[i];
	  gltexmap[i+256] = gltexmap2[n+1+i];
	}
      gltexmap[255] = gltexmap2[n];

      status = glGetError();
      if( status != GL_NO_ERROR )
	cerr << "OpenGL error tex: " << gluErrorString(status) << endl;

      setColorsChanged( true );
    }

  if( colorsChanged() )
    {
      for( i=0; i<n; ++i )
	{
	  glBindTexture( GL_TEXTURE_2D, gltexmap[i] );
	  glsprites[i] = convertToGLFormat( _sprite[i]->convertToImage() );
	  glTexImage2D( GL_TEXTURE_2D, 0, 4, 32, 32, 0, GL_RGBA, 
			GL_UNSIGNED_BYTE, glsprites[i].bits() );
	  status = glGetError();
	  if( status != GL_NO_ERROR )
	    cerr << "OpenGL error tex: " << gluErrorString(status) << endl;
	}

      glBindTexture( GL_TEXTURE_2D, gltexmap[255] );
      glsprites[255] = convertToGLFormat( _sprite[255]->convertToImage() );
      glTexImage2D( GL_TEXTURE_2D, 0, 4, 32, 32, 0, GL_RGBA, 
		    GL_UNSIGNED_BYTE, 
		    glsprites[255].bits() );
      status = glGetError();
      if( status != GL_NO_ERROR )
	cerr << "OpenGL error tex: " << gluErrorString(status) << endl;

      for( i=0; i<n; ++i )
	{
	  glBindTexture( GL_TEXTURE_2D, gltexmap[i+256] );
	  glsprites[i+256] 
	    = convertToGLFormat( _sprite[i+256]->convertToImage() );
	  glTexImage2D( GL_TEXTURE_2D, 0, 4, 32, 32, 0, GL_RGBA, 
			GL_UNSIGNED_BYTE, glsprites[i+256].bits() );
	  status = glGetError();
	  if( status != GL_NO_ERROR )
	    cerr << "OpenGL error tex: " << gluErrorString(status) << endl;
	}
      setColorsChanged( false );
    }
#ifdef RR_DEBUG
  for( i=0; i<256+240; ++i )
    {
      nonres[i] = false;
      usedres[i] = false;
    }
#endif
}


void QRGLGameField::paintGL()
{
  //updateScreen( true, _pscr->width(), _pscr->height() );
  displayFull();
}


bool QRGLGameField::colorsChanged() const
{
  if( d->colorsChanged )
    return( true );
  for( unsigned i=0; i<6; ++i )
    {
      WorkLevel::RGB	c1 = d->color[i];
      WorkLevel::RGB	c2 = game.tbct.color( (WorkLevel::PartColor) i );
      if( c1.r != c2.r || c1.g != c2.g || c1.b != c2.b )
	return( true );
    }
  return( false );
}


void QRGLGameField::setColorsChanged( bool x )
{
  d->colorsChanged = x;
  if( !x )
    {
      for( unsigned i=0; i<6; ++i )
	d->color[i] = game.tbct.color( (WorkLevel::PartColor) i );
    }
}


void QRGLGameField::mousePressEvent( QMouseEvent* e )
{
  emit mousePress( e );
}


void QRGLGameField::mouseReleaseEvent( QMouseEvent* e )
{
  emit mouseRelease( e );
}


void QRGLGameField::mouseMoveEvent( QMouseEvent* e )
{
  emit mouseMove( e );
}


#endif


