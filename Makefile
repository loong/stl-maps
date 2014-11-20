SRCS    = $(shell /bin/ls *.cc)
CFLAGS   = -DOP_ASSIGN
DEBUG = # -DL_DEBUG

.SUFFIXES: $(SUFFIXES) .cpp

%.o: %.cpp
#	g++ -c $(CFLAGS) $<
	g++ -c $(CFLAGS) -fno-elide-constructors $<

OBJS = main.o

main: $(OBJS)
	g++ -g $(CFLAGS) -o $@ $(OBJS) -lm

main.o: bstmap.hpp main.cpp
	g++ -c -g main.cpp

clean:
	rm -f core *~ $(OBJS) main main.exe testoutput.txt
