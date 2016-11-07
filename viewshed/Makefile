CC = /usr/bin/g++
CFLAGS = -Wall
DOVER = -std=c++11
OPTFLAGS = -O3

dover:
		$(CC) $(DOVER) $(CFLAGS) $(OPTFLAGS) -o viewshed viewshed.cpp grid.cpp

local:
		$(CC) $(CFLAGS) $(OPTFLAGS) -o viewshed viewshed.cpp grid.cpp

clean:
		$(RM) -f viewshed

test:
		./viewshed test

brunsdem:
		./viewshed ../testGrids/brunsdem.asc brunsdemvis.asc 266 193

kaweah:
		./viewshed ../testGrids/kaweah.asc kaweahvis.asc 712 581

sierra:
		./viewshed ../testGrids/sierra.asc sierravis.asc 1875 1336

portland_me:
		./viewshed ../testGrids/portland_me.asc portland_mevis.asc 5346 5994

usadem2:
		./viewshed ../testGrids/usadem2.asc usadem2vis.asc 12750 5500

eelriver:
		./viewshed /mnt/research/gis/DATA/DEM/eelriver.asc eelrivervis.asc 12535 10587

washington:
		./viewshed /mnt/research/gis/DATA/DEM/washington.asc washingtonvis.asc 16727 15993

mooseTest:
		qsub -l excl=true test.sh

usadem2Grid:
		qsub -q all.q@moose50 -l excl=true usadem2.sh

eelriverGrid:
		qsub -q all.q@moose51 -l excl=true eelriver.sh

washingtonGrid:
		qsub -q all.q@moose52 -l excl=true washington.sh