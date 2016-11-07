#!/bin/bash
#$ -cwd
#$ -j y
#$ -S /bin/bash
#$ -M sngo@bowdoin.edu -m b -m e

./parviewshed /mnt/research/gis/DATA/DEM/washington.asc washingtonvis.asc 16727 15933 4