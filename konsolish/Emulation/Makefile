LIBRARIES=emu.a(TEmu.o TEmuVt102.o TEScreen.o Translation.o)
PROGRAMS=

all: $(LIBRARIES) $(PROGRAMS)

include Makefile.inc

# DO NOT DELETE THIS LINE - make depend depends on it

TEScreen.o: TEScreen.h TECommon.h
TEmu.o: TEmu.h
TEmuVt102.o: config.h Translation.h TEmuVt102.h TEmu.h TEScreen.h TECommon.h
TEScreen.o: TECommon.h
TEmuVt102.o: TEmu.h TEScreen.h TECommon.h
