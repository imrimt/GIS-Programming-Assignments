CC = /usr/bin/g++
CFLAGS = -Wall
DOVER = -std=c++11
OPENMP = -fopenmp
OPTFLAG = -O3
NUMTHREAD = 8
MOOSE = -q all.q@moose51 -l excl=true -pe smp

dover:
		$(CC) $(DOVER) $(CFLAGS) $(OPTFLAG) $(OPENMP) -o parviewshed parviewshed.cpp grid.cpp

local:
		$(CC) $(CFLAGS) $(OPTFLAG) $(OPENMP) -o parviewshed parviewshed.cpp grid.cpp

clean:
		$(RM) -f parviewshed

tester:
		./parviewshed ../testGrids/tester.asc testervis.asc 3 3 $(NUMTHREAD)

set1:
		./parviewshed ../testGrids/set1.asc set1vis.asc 100 100 $(NUMTHREAD)

brunsdem:
		./parviewshed ../testGrids/brunsdem.asc brunsdemvis.asc 266 193 $(NUMTHREAD)

kaweah:
		./parviewshed ../testGrids/kaweah.asc kaweahvis.asc 712 581 $(NUMTHREAD)

sierra:
		./parviewshed ../testGrids/sierra.asc sierravis.asc 1875 1336 $(NUMTHREAD)

portland_me:
		./parviewshed ../testGrids/portland_me.asc portland_mevis.asc 5346 5994 $(NUMTHREAD)

portland_meTest:
		./parviewshed ../testGrids/portland_me.asc portland_mevis.asc 5346 5994 1 && ./parviewshed ../testGrids/portland_me.asc portland_mevis.asc 5346 5994 2 && ./parviewshed ../testGrids/portland_me.asc portland_mevis.asc 5346 5994 4 && ./parviewshed ../testGrids/portland_me.asc portland_mevis.asc 5346 5994 8

usadem2:
		./parviewshed ../testGrids/usadem2.asc usadem2vis.asc 12750 5500 $(NUMTHREAD)

usadem2Test:
		./parviewshed ../testGrids/usadem2.asc usadem2vis.asc 12750 5500 1 && ./parviewshed ../testGrids/usadem2.asc usadem2vis.asc 12750 5500 2 && ./parviewshed ../testGrids/usadem2.asc usadem2vis.asc 12750 5500 4 && ./parviewshed ../testGrids/usadem2.asc usadem2vis.asc 12750 5500 8

eelriver:
		./parviewshed /mnt/research/gis/DATA/DEM/eelriver.asc eelrivervis.asc 12535 10587 $(NUMTHREAD)

eelriverTest:
		./parviewshed /mnt/research/gis/DATA/DEM/eelriver.asc eelrivervis.asc 12535 10587 1 && ./parviewshed /mnt/research/gis/DATA/DEM/eelriver.asc eelrivervis.asc 12535 10587 2 && ./parviewshed /mnt/research/gis/DATA/DEM/eelriver.asc eelrivervis.asc 12535 10587 4 && ./parviewshed /mnt/research/gis/DATA/DEM/eelriver.asc eelrivervis.asc 12535 10587 8

washington:
		./parviewshed /mnt/research/gis/DATA/DEM/washington.asc washingtonvis.asc 16727 15993 $(NUMTHREAD)

washingtonTest:
		./parviewshed /mnt/research/gis/DATA/DEM/washington.asc washingtonvis.asc 16727 15993 1 && ./parviewshed /mnt/research/gis/DATA/DEM/washington.asc washingtonvis.asc 16727 15993 2 && ./parviewshed /mnt/research/gis/DATA/DEM/washington.asc washingtonvis.asc 16727 15993 4 && ./parviewshed /mnt/research/gis/DATA/DEM/washington.asc washingtonvis.asc 16727 15993 8

test:
		make brunsdemGrid && make kaweahGrid && make sierraGrid && make portland_meGrid && make usadem2Grid && make eelriverGridGrid// The bars,text and buttons are redrawn. && make washingtonGrid

testerGrid:
		qsub $(MOOSE) $(NUMTHREAD) tester.sh

set1Grid:
		qsub $(MOOSE) $(NUMTHREAD) set1.sh

brunsdemGrid:
		qsub $(MOOSE) 1 brunsdem1.sh && qsub $(MOOSE) 2 brunsdem2.sh && qsub $(MOOSE) 4 brunsdem4.sh && qsub $(MOOSE) 8 brunsdem8.sh && qsub $(MOOSE) 10 brunsdem10.sh && qsub $(MOOSE) 12 brunsdem12.sh && qsub $(MOOSE) 16 brunsdem16.sh && qsub $(MOOSE) 32 brunsdem32.sh

kaweahGrid:
		qsub $(MOOSE) 1 kaweah1.sh && qsub $(MOOSE) 2 kaweah2.sh && qsub $(MOOSE) 4 kaweah4.sh && qsub $(MOOSE) 8 kaweah8.sh && qsub $(MOOSE) 10 kaweah10.sh && qsub $(MOOSE) 12 kaweah12.sh && qsub $(MOOSE) 16 kaweah16.sh && qsub $(MOOSE) 32 kaweah32.sh

sierraGrid:
		qsub $(MOOSE) 1 sierra1.sh && qsub $(MOOSE) 2 sierra2.sh && qsub $(MOOSE) 4 sierra4.sh && qsub $(MOOSE) 8 sierra8.sh && qsub $(MOOSE) 10 sierra10.sh && qsub $(MOOSE) 12 sierra12.sh && qsub $(MOOSE) 16 sierra16.sh && qsub $(MOOSE) 32 sierra32.sh

portland_meGrid:
		qsub $(MOOSE) 1 portland_me1.sh && qsub $(MOOSE) 2 portland_me2.sh && qsub $(MOOSE) 4 portland_me4.sh && qsub $(MOOSE) 8 portland_me8.sh && qsub $(MOOSE) 10 portland_me10.sh && qsub $(MOOSE) 12 portland_me12.sh && qsub $(MOOSE) 16 portland_me16.sh && qsub $(MOOSE) 32 portland_me32.sh

usadem2Grid:
		qsub $(MOOSE) 1 usadem21.sh && qsub $(MOOSE) 2 usadem22.sh && qsub $(MOOSE) 4 usadem24.sh && qsub $(MOOSE) 8 usadem28.sh && qsub $(MOOSE) 10 usadem210.sh && qsub $(MOOSE) 12 usadem212.sh && qsub $(MOOSE) 16 usadem216.sh && qsub $(MOOSE) 32 usadem232.sh

eelriverGrid:
		qsub $(MOOSE) 1 eelriver1.sh && qsub $(MOOSE) 2 eelriver2.sh && qsub $(MOOSE) 4 eelriver4.sh && qsub $(MOOSE) 8 eelriver8.sh && qsub $(MOOSE) 10 eelriver10.sh && qsub $(MOOSE) 12 eelriver12.sh && qsub $(MOOSE) 16 eelriver16.sh && qsub $(MOOSE) 32 eelriver32.sh

washingtonGrid:
		qsub $(MOOSE) 1 washington1.sh && qsub $(MOOSE) 2 washington2.sh && qsub $(MOOSE) 4 washington4.sh && qsub $(MOOSE) 8 washington8.sh && qsub $(MOOSE) 10 washington10.sh && qsub $(MOOSE) 12 washington12.sh && qsub $(MOOSE) 16 washington16.sh && qsub $(MOOSE) 32 washington32.sh

largeGrid:
		qsub $(MOOSE) 10 eelriver10.sh && qsub $(MOOSE) 12 eelriver12.sh && qsub $(MOOSE) 16 eelriver16.sh && qsub $(MOOSE) 32 eelriver32.sh && qsub $(MOOSE) 10 washington10.sh && qsub $(MOOSE) 12 washington12.sh && qsub $(MOOSE) 16 washington16.sh && qsub $(MOOSE) 32 washington32.sh

testGrid:
		qsub $(MOOSE) $(NUMTHREAD) test.sh

delo:
		rm -f *.sh.o*
