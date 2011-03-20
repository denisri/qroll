/***************************************************************************
                          glcontext.cc  -  description
                             -------------------
    begin                : 2001
    copyright            : (C) 2001 by SHFJ - CEA Orsay (France)
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

/* These classes have been 'stolen' to my lab (SHFJ/CEA), made by 
   Dimitri Papadopoulos (papadopo@shfj.cea.fr) to work around a problem 
   with Sun X server at least on Creator3D and Elite3D cards: the first 
   (default) visual chosen by Qt has a strange alpha setting and is too 
   bright, which is especially ugly.
   With this hack we can choose the correct visual */

#ifndef RR_NO_OPENGL

#ifndef QT_CLEAN_NAMESPACE
#define QT_CLEAN_NAMESPACE
#endif

/* Uhum... it's not me ! It's not MY hack !!
   Actually Dimitri is the guy who makes the cleanest code on this planet, 
   but there was definitely no other way to do this... */
#if defined(_WS_X11_) || defined(Q_WS_X11)
#define private public
#define protected public
#endif

#include <qgl.h>

#if defined(_WS_X11_) || defined(Q_WS_X11)
#undef private
#undef protected
#endif
#include <iostream>


namespace shfj
{

  class GLContext : public QGLContext
  {
  public:
    GLContext( const QGLFormat& format, QPaintDevice* device );
    virtual ~GLContext();
#if defined(_WS_X11_) || defined(Q_WS_X11)
    virtual void *tryVisual( const QGLFormat& f, int bufDepth = 1 );
#endif
  };

}


using namespace shfj;


GLContext::GLContext( const QGLFormat& format, QPaintDevice* device )
    : QGLContext( format, device )
{
}


GLContext::~GLContext()
{
}


#if defined(_WS_X11_) || defined(Q_WS_X11)
#include <GL/glx.h>
#include <X11/Xlib.h>


void *GLContext::tryVisual( const QGLFormat& f, int bufDepth )
{
    XVisualInfo* info = (XVisualInfo*)QGLContext::tryVisual( f, bufDepth );

#if defined(_OS_SOLARIS_) || defined (Q_OS_SOLARIS)
//    qDebug( "default visual id: %ld", info->visualid );

    /* locate the visuals with the same class as 'info' */
    XVisualInfo r_info;
    r_info.c_class = info->c_class;
    r_info.screen = device()->x11Screen();
    long info_mask = VisualClassMask | VisualScreenMask;
    int visuals;
    XVisualInfo* info_list = XGetVisualInfo( device()->x11Display(),
					     info_mask, &r_info, &visuals );

    /* find similar visuals, stop on first match */
    int	i;
    for ( i = 0; i < visuals; ++i )
      {
//	qDebug( "\ttrying visual %ld", info_list[i].visualid );
	if ( info_list[i].depth == info->depth &&
	     info_list[i].c_class == info->c_class &&
	     info_list[i].red_mask == info->red_mask &&
	     info_list[i].green_mask == info->green_mask &&
	     info_list[i].blue_mask == info->blue_mask &&
	     info_list[i].colormap_size == info->colormap_size &&
	     info_list[i].bits_per_rgb == info->bits_per_rgb )
	  {
	    *info = info_list[i];
	    break;
	  }
      }

    /* found a match */
    if ( i < visuals ) {
	/* replace old visual with new similar one */
	*info = info_list[i];
    }

    XFree( info_list );

//    qDebug( "actual visual id: %ld", info->visualid );
#endif

    return info;
}


#endif


#include "roll/gui/glcontext.h"


GLWidget::GLWidget( QWidget* parent, const char* name,
                    const QGLWidget* shareWidget, Qt::WFlags f )
  : QGLWidget( parent, shareWidget, f )
{
  setObjectName( name );
  QGLFormat format = QGLFormat::defaultFormat();
  if ( shareWidget )
    setContext( new shfj::GLContext( format, this ), shareWidget->context() );
  else
      setContext( new shfj::GLContext( format, this ) );
  // setBackgroundMode( Qt::NoBackground );
}


GLWidget::GLWidget( const QGLFormat& format, QWidget* parent, 
		    const char* name, const QGLWidget* shareWidget, 
                    Qt::WFlags f )
  : QGLWidget( format, parent, shareWidget, f )
{
  setObjectName( name );
  if ( shareWidget )
    setContext( new shfj::GLContext( format, this ), shareWidget->context() );
  else
      setContext( new shfj::GLContext( format, this ) );
  // setBackgroundMode( Qt::NoBackground );
}


GLWidget::~GLWidget()
{
}

#endif

