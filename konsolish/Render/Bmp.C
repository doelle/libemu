// [Bmp.C] Incore renderer
//
// Copyright (c) 2003 by Lars Doelle <lars.doelle@on-line.de>
//
// Note that the .bmp format is as easy to create as it huge
// compared with other formats. Do a libpng thingy, when there
// is ever time for such things.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Bmp.h"
#include "Font.h"
#include "TECommon.h"


union byteorder
{
  u32 i;
  u16 s;
  u8  b[4];
};

static u32 u32toLSB(u32 v)
{ int i;
  union byteorder map;
  union byteorder iv;
  union byteorder ov;
  map.i = 0x03020100;
  iv.i  = v;
  for (i = 0; i < 4; i++)
  {
    ov.b[map.b[i]] = iv.b[i];
  }
  return ov.i;
}

static u16 u16toLSB(u16 v)
{ int i;
  union byteorder map;
  union byteorder iv;
  union byteorder ov;
  map.s = 0x0100;
  iv.s  = v;
  for (i = 0; i < 2; i++)
  {
    ov.b[map.b[i]] = iv.b[i];
  }
  return ov.s;
}


struct Rgb
{
  Rgb(u8 r, u8 g, u8 b) : rgbBlue(b), rgbGreen(g), rgbRed(r), rgbReserved(0) {}
  u8 rgbBlue;
  u8 rgbGreen;
  u8 rgbRed;
  u8 rgbReserved;
} __attribute__ ((packed));

static const Rgb base_color_table[2+8+2+8] =
// The following are almost IBM standard color codes, with some slight
// gamma correction for the dim colors to compensate for bright X screens.
// It contains the 8 ansiterm/xterm colors in 2 intensities.
{
  // Fixme: could add faint colors here, also.
  // normal
  Rgb(0xB2,0xB2,0xB2), Rgb(0x00,0x00,0x00), // Dfore, Dback
  Rgb(0x00,0x00,0x00), Rgb(0xB2,0x18,0x18), // Black, Red
  Rgb(0x18,0xB2,0x18), Rgb(0xB2,0x68,0x18), // Green, Yellow
  Rgb(0x18,0x18,0xB2), Rgb(0xB2,0x18,0xB2), // Blue,  Magenta
  Rgb(0x18,0xB2,0xB2), Rgb(0xB2,0xB2,0xB2), // Cyan,  White
  // intensiv
  Rgb(0xFF,0xFF,0xFF), Rgb(0x00,0x00,0x00),
  Rgb(0x68,0x68,0x68), Rgb(0xFF,0x54,0x54),
  Rgb(0x54,0xFF,0x54), Rgb(0xFF,0xFF,0x54),
  Rgb(0x54,0x54,0xFF), Rgb(0xFF,0x54,0xFF),
  Rgb(0x54,0xFF,0xFF), Rgb(0xFF,0xFF,0xFF)
};

struct BmpHdr
{
  //
  // bmp_fileheader
  //
  u8  sig[2];          // 'BM'
  u32 filesize;
  u32 reserved;
  u32 data_offset;     // File offset to raster data
  //
  // bmp_infoheader
  //
  u32 infoheader_size;
  u32 width;
  u32 height;
  u16 planes;
  u16 bpp;
  u32 compression;      // 0 = none, 1 = 8bit RLE, 2 = 4bit RLE
  u32 image_size;       // Size of compressed image, or 0
  u32 xppm,yppm;        // Pixels per meter
  u32 colors_used;      // Number of colors actually used
  u32 colors_important; // Number of important colors
  //
  // color table
  //
  Rgb color_table[];
} __attribute__ ((packed));

// shrink may by 0-3 (1,2,4,8)

Bmp::Bmp(int columns, int lines, ca* image, int shrink)
: cols(columns), lins(lines),fact(1<<(shrink&3))
{
  int w =  FONT_WIDTH*cols/fact;
  int h = FONT_HEIGHT*lins/fact;
  data = (u8*)malloc(sizeof(BmpHdr)+sizeof(base_color_table)+w*h);
//memset(data+sizeof(BmpHdr)+sizeof(base_color_table),0,cols*FONT_WIDTH*lins*FONT_HEIGHT); // 0 = back
  BmpHdr* b = (BmpHdr*)data;
  b->sig[0]           = 'B';
  b->sig[1]           = 'M';
  b->filesize         = u32toLSB( sizeof(BmpHdr)+sizeof(base_color_table)+w*h );
  b->reserved         = u32toLSB( 0 );
  b->data_offset      = u32toLSB( sizeof(BmpHdr)+sizeof(base_color_table) );     // File offset to raster data
  //
  // bmp_infoheader
  //
  b->infoheader_size  = u32toLSB( 40 );
  b->width            = u32toLSB( w );
  b->height           = u32toLSB( h );
  b->planes           = u16toLSB( 1 );
  b->bpp              = u16toLSB( 8 );
  b->compression      = u32toLSB( 0 );               // 0 = none, 1 = 8bit RLE, 2 = 4bit RLE
  b->image_size       = u32toLSB( w*h );             // Size of compressed image, or 0
  b->xppm             = u32toLSB( 6000 );
  b->yppm             = u32toLSB( 6000 );  // Pixels per meter
  b->colors_used      = u32toLSB( 20 );    // Number of colors actually used
  b->colors_important = u32toLSB( 20 );    // Number of important colors
  //
  // color table
  //
  memcpy(data+sizeof(BmpHdr),base_color_table,sizeof(base_color_table));
  //
  // image
  //
  for (int y = 0; y < lins; y++)
  for (int x = 0; x < cols; x++)
  {
    ca cc = image[y*cols+x];
    set(x,y,cc.c,cc.f,cc.b,cc.r);
  }
}

Bmp::~Bmp()
{
  free(data);
}

void Bmp::set(int x, int y, int c, int fg, int bg, int re)
{
  if (x < 0 || x >= cols || y < 0 || y >= lins)
  { fprintf(stderr,"Bmp::set(%d,%d,...) - out of bounds\n",x,y); return; }

  u8* bits = getGlyph(c);
  for (int py = 0; py < FONT_HEIGHT; py+=fact)
  {
    for (int px = 0; px < FONT_WIDTH; px+=fact)
    {
      int loc0 = sizeof(BmpHdr)+sizeof(base_color_table);
      int loc  = loc0 + ((lins-y-1)*FONT_HEIGHT/fact+py/fact)*FONT_WIDTH/fact*cols + (x*FONT_WIDTH/fact+px/fact);
      int bit  = (bits[(FONT_HEIGHT-1-py)] >> (FONT_WIDTH-1-px))&1; // beware, swaps lurk all the way
      if ((re & RE_UNDERLINE) && py == 2) bit = 1;                  // underline rendition
      data[loc] = bit?fg:bg;
    }
  }
}

const u8* Bmp::bytes()
{
  return data;
}

int Bmp::length()
{
  return sizeof(BmpHdr)+sizeof(base_color_table)+cols*FONT_WIDTH*lins*FONT_HEIGHT/fact/fact;
}
