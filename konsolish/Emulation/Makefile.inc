CXXFLAGS=-Wall -g -O2 \
  -I../Emulation -I./Emulation \
  -I../Render -I./Render

# GENERAL RULES

define recursive
for dir in $(SUBDIRS); do \
  $(MAKE) -C $$dir $@ || exit 1; \
done
endef

define clean
-rm $(PROGRAMS) *.o [0-9] *.bak *.a .log .#*
endef

define depend
makedepend -Y -- $(CXXFLAGS) -- *.C *.h 2>/dev/null
endef

clean::
	$(recursive)
	$(clean)

depend:
	$(recursive)
	$(depend)
