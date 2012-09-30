PROGRAMS=emuTest
SUBDIRS=Render Emulation

all:
	$(recursive)
	$(MAKE) $(PROGRAMS)

emuTest: Emulation/emu.a Render/render.a

include Makefile.inc

# DO NOT DELETE THIS LINE - make depend depends on it

./Render/Bmp.o: ./Render/Bmp.h ./Emulation/TECommon.h ./Render/Font.h
./Render/Font.o: ./Render/Font.h
./Render/Bmp.o: ./Emulation/TECommon.h
./Emulation/TEmu.o: ./Emulation/TEmu.h
./Emulation/TEmuVt102.o: ./Emulation/TEmu.h ./Emulation/TEScreen.h
./Emulation/TEmuVt102.o: ./Emulation/TECommon.h
./Emulation/TEScreen.o: ./Emulation/TECommon.h
./Emulation/TEScreen.o: ./Emulation/TEScreen.h ./Emulation/TECommon.h
./Emulation/TEmuVt102.o: ./Emulation/config.h ./Emulation/Translation.h
./Emulation/TEmuVt102.o: ./Emulation/TEmuVt102.h ./Emulation/TEmu.h
./Emulation/TEmuVt102.o: ./Emulation/TEScreen.h ./Emulation/TECommon.h
