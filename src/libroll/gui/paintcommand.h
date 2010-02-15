/***************************************************************************
                          paintcommand.h  -  description
                             -------------------
    begin                : 2002
    copyright            : (C) 2002 by Denis Rivière
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


#ifndef QROLL_GUI_PAINTCOMMAND_H
#define QROLL_GUI_PAINTCOMMAND_H

#include <stack>

namespace roll
{

  class PaintCommand
  {
  public:
    PaintCommand();
    virtual ~PaintCommand();
    virtual void size() const = 0;
    virtual void undoValid() const;
    virtual void undo() = 0;
  };


  class PaintStack
  {
  public:
    PaintStack();
    ~PaintStack();
    void push( PaintCommand* );
    /// pops the last command
    PaintCommand* pop();
    /// pops the oldest command
    PaintCommand* forget();
    /// approx. size (in bytes) of the command stack
    unsigned size() const;

  private:
    std::stack<PaintCommand *>	_stack;
  };
    
}

#endif

