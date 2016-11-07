#!/bin/bash
#$ -cwd
#$ -j y
#$ -S /bin/bash
#$ -M sngo@bowdoin.edu -m b -m e

./parviewshed ../testGrids/portland_me.asc portland_mevis.asc 5346 5994 32