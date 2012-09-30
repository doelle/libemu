// [Bmp.h] Incore renderer
//
// Copyright (c) 2003 by Lars Doelle <lars.doelle@on-line.de>

#ifndef BMP_H
#define BMP_H

typedef unsigned int   u32;
typedef unsigned short u16;
typedef unsigned char  u8 ;

#include "TECommon.h"

class Bmp
{
public:
  Bmp(int w, int h, ca* image, int shrink=0);
 ~Bmp();
public:
  const u8* bytes();
  int length();
protected:
  void set(int x, int y, int c, int fg, int bg, int re);
  u8* data;
  int cols;
  int lins;
  int fact;
};

#endif
