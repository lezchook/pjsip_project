PJDIR = /home/lezchook/pjproject-2.14.1
include $(PJDIR)/build.mak

main: main.o
	$(PJ_CXX) -o $@ $< $(PJ_LDXXFLAGS) $(PJ_LDXXLIBS)

main.o: main.cpp
	$(PJ_CXX) -c -o $@ $< $(PJ_CXXFLAGS)

clean:
	rm -f main.o main
