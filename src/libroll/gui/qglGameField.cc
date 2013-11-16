
/***************************************************************************
                          qglGameField.cc  -  description
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
# include <GL/glu.h>
#else
#ifdef ANDROID
# include <GLES/gl.h>
#else
# include <GL/gl.h>
# include <GL/glu.h>
#endif
#endif
#include <iostream>
#include <math.h>

using namespace roll;
using namespace std;


#ifdef ANDROID
QRGLGameField::GLMode	QRGLGameField::_glmode = QRGLGameField::Texture;
#else
QRGLGameField::GLMode	QRGLGameField::_glmode = QRGLGameField::DrawPixels;
#endif

static map<unsigned short, QImage>	glsprites;
static GLuint				*gltexmap = 0, *gltexmap2 = 0;
static vector<pair<GLfloat,GLfloat> >   gltexcoord;
static QGLWidget			*sharedwid = 0;

#ifdef RR_DEBUG
// temp, debug
static bool				nonres[512];
static bool				usedres[512];
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
  // setBackgroundMode( Qt::NoBackground );
  if( !sharedwid )
  {
    sharedwid = new QGLWidget( 0, this );
    sharedwid->setObjectName( "shared widget" );
  }
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

#ifndef GL_VERSION_ES_CM_1_0
  // no glDrawPixels in GLES
  // no glPixelZoom either
  if( _glmode == DrawPixels && scalingEnabled() )
  {
    glPixelZoom( sc, sc );
  }
  else
    glPixelZoom( 1, 1 );
#endif

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

#ifndef GL_VERSION_ES_CM_1_0
  if( _glmode == Texture )
  {
#endif
    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glEnable( GL_TEXTURE_2D );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                      GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR );

    // toy to test "real" 3D...
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    /*
    GLfloat m[16] = { 8.66025388e-01, 0.5, 0, 0,
      -0.5, 8.66025388e-01, 0, 0,
      0, 0, 1., 0,
      0, 0, 0, 1.,
    };
    glLoadMatrixf( m );
    */

#ifndef GL_VERSION_ES_CM_1_0
  }
  else
  {
    glDisable( GL_TEXTURE_2D );
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStoref( GL_UNPACK_SKIP_PIXELS, 0 );
    glPixelStoref( GL_UNPACK_SKIP_ROWS, 0 );
  }
#endif
}


void QRGLGameField::copySprite( unsigned spr, int posx, int posy )
{
  float	x = float( posx ) / (16 * sWidth()) - 1.;
  float	y = 1. - float( posy+32 ) / (16 * sHeight());
  float	sx = 2.F / sWidth();
  float	sy = 2.F / sHeight();

#ifndef GL_VERSION_ES_CM_1_0
  if( _glmode == Texture )
  {
#endif
    const pair<GLfloat,GLfloat> & tc = gltexcoord[ spr ];
    float tsz = 1. / 16.;
    glBindTexture( GL_TEXTURE_2D, gltexmap[spr] );
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    GLfloat vert[8];
    GLfloat tex[8];
    GLushort vpoly[4];
    vert[0] = x;
    vert[1] = y;
    vert[2] = x;
    vert[3] = y + sy;
    vert[4] = x + sx;
    vert[5] = y + sy;
    vert[6] = x + sx;
    vert[7] = y;
    tex[0] = tc.first;
    tex[1] = tc.second;
    tex[2] = tc.first;
    tex[3] = tc.second + tsz;
    tex[4] = tc.first + tsz;
    tex[5] = tc.second + tsz;
    tex[6] = tc.first + tsz;
    tex[7] = tc.second;
    vpoly[0] = 0;
    vpoly[1] = 1;
    vpoly[2] = 2;
    vpoly[3] = 3;
    glVertexPointer( 2, GL_FLOAT, 0, vert );
    glTexCoordPointer( 2, GL_FLOAT, 0, tex );
    glDrawElements( GL_TRIANGLE_FAN, 4, GL_UNSIGNED_SHORT, vpoly );

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

#ifndef GL_VERSION_ES_CM_1_0
  }
  else
  {
    glRasterPos2f( x, y );
    glPixelStorei( GL_UNPACK_ROW_LENGTH, 512 );
    const pair<GLfloat,GLfloat> & tc = gltexcoord[ spr ];
    glPixelStorei( GL_UNPACK_SKIP_PIXELS, int(round(tc.first*16))*32 );
    glPixelStorei( GL_UNPACK_SKIP_ROWS, int(round(tc.second*16))*32 );
    glDrawPixels( 32, 32, GL_RGBA, GL_UNSIGNED_BYTE,
                  glsprites[spr>>8].bits() );
    glPixelStorei( GL_UNPACK_SKIP_PIXELS, 0 );
    glPixelStorei( GL_UNPACK_SKIP_ROWS, 0 );
    glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 );
  }
#endif
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
#ifndef GL_VERSION_ES_CM_1_0
  glPixelStorei( GL_UNPACK_SKIP_PIXELS, 0 );
  glPixelStorei( GL_UNPACK_SKIP_ROWS, 0 );
#endif

  makeTextures();
}


void QRGLGameField::makeTextures()
{
  unsigned	i;
  GLenum	status;

  if( !gltexmap )
  {
    gltexmap = new GLuint[ 512 ];
    gltexmap2 = new GLuint[ 2 ];
    gltexcoord.resize( 512 );
    glGenTextures( 2, gltexmap2 );

    for( i=0; i<256; ++i )
    {
      gltexmap[i] = gltexmap2[0];
      gltexmap[i+256] = gltexmap2[1];
    }

#ifndef GL_VERSION_ES_CM_1_0
    status = glGetError();
    if( status != GL_NO_ERROR )
      err << "OpenGL error tex: " << gluErrorString(status) << endl;
#else
    status = glGetError();
    if( status != GL_NO_ERROR )
      err << "OpenGL error tex: " << status << endl;
#endif

    setColorsChanged( true );
  }

  if( colorsChanged() )
  {
    float xscl = 1. / 16, yscl = 1. / 16;
    for( i=0; i<1; ++i )
    {
      QImage im( 512, 512, QImage::Format_RGB32 );
      QPainter p;
      p.begin( &im );
      for( int j=0; j<256; ++j )
      {
        int ind = j+256*i;
        if( _sprite[ ind ] )
        {
          int x = j & 15;
          int y = j >> 4;
          p.drawPixmap( x * 32, y * 32, *_sprite[ ind ] );
          gltexcoord[ ind ] = make_pair( GLfloat( x * xscl),
                                          GLfloat( 15. / 16 - y * yscl ) );
        }
      }
      p.end();
      glsprites[i] = convertToGLFormat( im );

      glBindTexture( GL_TEXTURE_2D, gltexmap2[i] );
      glTexImage2D( GL_TEXTURE_2D, 0, 4, 512, 512, 0, GL_RGBA,
                    GL_UNSIGNED_BYTE, glsprites[i].bits() );
#ifndef GL_VERSION_ES_CM_1_0
      status = glGetError();
      if( status != GL_NO_ERROR )
        cerr << "OpenGL error tex: " << gluErrorString(status) << endl;
#else
      status = glGetError();
      if( status != GL_NO_ERROR )
        cerr << "OpenGL error tex: " << status << endl;
#endif
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


#endif // RR_NO_OPENGL


