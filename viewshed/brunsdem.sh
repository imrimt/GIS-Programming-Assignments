#!/bin/bash
#$ -cwd
#$ -j y
#$ -S /bin/bash
#$ -M sngo@bowdoin.edu -m b -m e

./viewshed ../testGrids/brunsdem.asc brunsdemvis.asc 266 193