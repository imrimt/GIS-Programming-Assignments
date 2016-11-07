#!/bin/bash
#$ -cwd
#$ -j y
#$ -S /bin/bash
#$ -M sngo@bowdoin.edu -m b -m e

./parviewshed /mnt/research/gis/DATA/DEM/eelriver.asc eelrivervis.asc 12535 10587 1