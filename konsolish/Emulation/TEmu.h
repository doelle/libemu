/* -------------------------------------------------------------------------- */
/*                                                                            */
/* [TEmu.h]                  Terminal Emulation                               */
/*                                                                            */
/* -------------------------------------------------------------------------- */
/*                                                                            */
/* Copyright (c) 2006 by Lars Doelle <lars.doelle@on-line.de>                 */
/*                                                                            */
/* -------------------------------------------------------------------------- */

#ifndef EMU_H
#define EMU_H

// This is the abstract emulation class

class TEmu
{
public:

  TEmu();
  virtual ~TEmu();

public:

  virtual void onRcvChar(int cc);
  virtual void changeSize(int columns, int lines);
};

#endif // ifndef EMU_H
