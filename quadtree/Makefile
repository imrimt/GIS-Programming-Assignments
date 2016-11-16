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

POINTS = 1

mylastoqdt: clean
	$(CC) $(LDFLAGS) -o mylastoqdt viewqdt.cpp mylastoqdt.cpp quadtree.cpp

default: $(PROGS)

france:
	./mylastoqdt $(FILEPATH)/france.txt $(POINTS)

lake:
	./mylastoqdt $(FILEPATH)/lake.txt $(POINTS)

fusa:
	./mylastoqdt $(FILEPATH)/fusa.txt $(POINTS)

sample:
	./mylastoqdt $(FILEPATH)/sample.txt $(POINTS)

zurich:
	./mylastoqdt $(FILEPATH)/zurich.txt $(POINTS)

house:
	./mylastoqdt $(FILEPATH)/house.txt $(POINTS)

autzen:
	./mylastoqdt $(FILEPATH)/autzen.txt $(POINTS)

lincoln:
	./mylastoqdt $(FILEPATH)/lincoln.txt $(POINTS)

radiohead_data2:
	./mylastoqdt $(FILEPATH)/radiohead_data2.txt $(POINTS)

serpent:
	./mylastoqdt $(FILEPATH)/serpent.txt $(POINTS)

test:
	./mylastoqdt $(FILEPATH)/test.txt $(POINTS)

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


