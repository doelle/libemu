// -------------------------------------------------------------------------
//
// [emuTest.C] preparing some regression material
//
// -------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string>

#include "TEmuVt102.h"
#include "Bmp.h"

using namespace std;

// -------------------------------------------------------------------------
//
// Screen Shots
//
// -------------------------------------------------------------------------

static string screenShot(const TEmuVt102* emu, int shrink)
{
  // convert screen

  ca* img = emu->getCookedImage();
  Bmp bmp(emu->columns(),emu->lines(),img,shrink);
  free(img);

  return string((char*)bmp.bytes(),bmp.length());
}

static void putString(string filename, string data)
{
  FILE* f = fopen(filename.c_str(),"w"); assert(f);
  fwrite(data.data(),data.size(),1,f);
  fclose(f);
}

__attribute__((unused))
static void largeScreenShot(string filename, const TEmuVt102* emu)
{
  putString(filename,screenShot(emu,0));
}

__attribute__((unused))
static void smallScreenShot(string filename, const TEmuVt102* emu)
{
  putString(filename,screenShot(emu,1));
}

// -------------------------------------------------------------------------
//
// MAIN
//
// -------------------------------------------------------------------------

int main()
{
  FILE*      aud = fopen("xaudit","r"); assert(aud);
  TEmuVt102* emu = new TEmuVt102();

  int cc;
  for (int i = 0; fread(&cc,sizeof(cc),1,aud) == 1; i++)
  {
    emu->onRcvChar(cc);
    if (i % 100 == 0)
    {
      char filename[20]; sprintf(filename,"%05d.bmp",i);
      largeScreenShot(filename,emu);
    }
  }

  delete emu;
  fclose(aud);

  return 0;
}
