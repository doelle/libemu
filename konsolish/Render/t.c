#include <stdio.h>

typedef unsigned int   u32;
typedef unsigned short u16;
typedef unsigned char  u8;

union byteorder
{
  u32 i;
  u16 s;
  u8  b[4];
};

u32 u32toLSB(u32 v)
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

u16 u16toLSB(u16 v)
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

int main()
{ int i;
  union byteorder test;
  test.i = 0x00010203;
  for (i = 0; i < 4; i++)
  {
    printf("%d -> %d\n",i,test.b[i]);
  }
  test.s = 0x0001;
  for (i = 0; i < 2; i++)
  {
    printf("%d -> %d\n",i,test.b[i]);
  }
  printf("test u32 : 0x%08x\n",u32toLSB(0x12345678));
  printf("test u16 : 0x%08x\n",u16toLSB(0x1234));
  return 0;
}
