#!/bin/bash
#$ -cwd
#$ -j y
#$ -S /bin/bash
#$ -M sngo@bowdoin.edu -m b -m e

./viewshed ../testGrids/kaweah.asc kaweahvis.asc 712 581