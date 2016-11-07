#!/bin/bash
#$ -cwd
#$ -j y
#$ -S /bin/bash
#$ -M sngo@bowdoin.edu -m b -m e

./parviewshed ../testGrids/brunsdem.asc brunsdemvis.asc 266 193 12