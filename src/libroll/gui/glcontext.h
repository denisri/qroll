/***************************************************************************
                          glcontext.h  -  description
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

#ifndef ROLL_GUI_GLCONTEXT_H
#define ROLL_GUI_GLCONTEXT_H

#ifndef RR_NO_OPENGL

#include <QtGlobal>
#if QT_VERSION >= 0x060000
#define USE_OPENGLWIDGET
#include <QOpenGLWidget>
#else
#include <QGLWidget>
#endif

#ifdef USE_OPENGLWIDGET
class GLWidget : public QOpenGLWidget
#else
class GLWidget : public QGLWidget
#endif
{
    Q_OBJECT
public:
#ifdef USE_OPENGLWIDGET
    GLWidget( QWidget* parent=0, const char* name=0,
              const QOpenGLWidget* shareWidget = 0,
              Qt::WindowFlags f = Qt::WindowFlags() );
#else
    GLWidget( QWidget* parent=0, const char* name=0,
              const QGLWidget* shareWidget = 0,
              Qt::WindowFlags f = Qt::WindowFlags() );
    GLWidget( const QGLFormat& format, QWidget* parent=0, const char* name=0,
              const QGLWidget* shareWidget = 0,
              Qt::WindowFlags f = Qt::WindowFlags() );
#endif
    virtual ~GLWidget();
};


#endif

#endif

