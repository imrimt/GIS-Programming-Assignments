PLATFORM = $(shell uname)


## Compilation flags
##comment out one or the other 
##debugging 
CFLAGS = -g 

##release
#CFLAGS = -O3 -DNDEBUG

LDFLAGS=

CFLAGS+= -Wall

ifeq ($(PLATFORM),Darwin)
## Mac OS X
CFLAGS += -m64 -isystem/usr/local/include  -Wno-deprecated 
LDFLAGS+= -m64 -lc -framework AGL -framework OpenGL -framework GLUT -framework Foundation
else
## Linux
CFLAGS += -m64
INCLUDEPATH  = -I/usr/include/GL/ 
LIBPATH = -L/usr/lib64 -L/usr/X11R6/lib
LDFLAGS+=  -lGL -lglut -lrt -lGLU -lX11 -lm  -lXmu -lXext -lXi
endif

CC = /usr/bin/g++ $(CFLAGS) $(INCLUDEPATH)

CCFLAGS = /usr/bin/g++

C11 = -std=c++11

PROGS = viewqdt

mylastoqdt: clean
	$(CCFLAGS) $(CFLAGS) $(C11) -o mylastoqdt mylastoqdt.cpp quadtree.cpp

default: $(PROGS)

france:
	./mylastoqdt data/france.txt 10

lake:
	./mylastoqdt data/lake.txt 10

fusa:
	./mylastoqdt data/fusa.txt 10

sample:
	./mylastoqdt data/sample.txt 10

zurich:
	./mylastoqdt data/zurich.txt 10

viewqdt: viewqdt.o  quadtree.o
	$(CC) -o $@ viewqdt.o   quadtree.o $(LDFLAGS)

viewqdt.o: viewqdt.c quadtree.h 
	$(CC) -c $(INCLUDEPATH)  viewqdt.c  -o $@

quadtree.o: quadtree.c quadtree.h
	$(CC) -c $(INCLUDEPATH)  quadtree.c -o $@

clean:
	rm -f *.o
	rm -f viewqdt
	rm -f mylastoqdt


