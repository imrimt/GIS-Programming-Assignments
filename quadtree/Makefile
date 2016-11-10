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

CC = /usr/bin/g++ $(CFLAGS) $(C11) $(INCLUDEPATH)

CCFLAGS = /usr/bin/g++

C11 = -std=c++11

PROGS = viewqdt

FILEPATH = /Users/sngo/Desktop/GIS-Data/lasdata

mylastoqdt: clean
	$(CC) $(LDFLAGS) -o mylastoqdt viewqdt.cpp mylastoqdt.cpp quadtree.cpp

default: $(PROGS)

france:
	./mylastoqdt $(FILEPATH)/france.txt 30

lake:
	./mylastoqdt $(FILEPATH)/lake.txt 30

fusa:
	./mylastoqdt $(FILEPATH)/fusa.txt 30

sample:
	./mylastoqdt $(FILEPATH)/sample.txt 30

zurich:
	./mylastoqdt $(FILEPATH)/zurich.txt 30

house:
	./mylastoqdt $(FILEPATH)/house.txt 30

autzen:
	./mylastoqdt $(FILEPATH)/autzen.txt 30

lincoln:
	./mylastoqdt $(FILEPATH)/lincoln.txt 30

radiohead_data2:
	./mylastoqdt $(FILEPATH)/radiohead_data2.txt 30

serpent:
	./mylastoqdt $(FILEPATH)/serpent.txt 30

# viewqdt: viewqdt.o mylastoqdt.o quadtree.o
# 	$(CC) -o $@ viewqdt.o quadtree.o $(LDFLAGS) 

# viewqdt.o: viewqdt.cpp quadtree.h
# 	$(CC) -c $(INCLUDEPATH) viewqdt.cpp -o $@

# mylastoqdt.o: mylastoqdt.cpp quadtree.h
# 	$(CC) -c $(INCLUDEPATH) mylastoqdt.cpp -o $@

# quadtree.o: quadtree.cpp quadtree.h
# 	$(CC) -c $(INCLUDEPATH) quadtree.cpp -o $@

clean:
	rm -f *.o
	rm -f viewqdt
	rm -f mylastoqdt


