#!/bin/bash
#$ -cwd
#$ -j y
#$ -S /bin/bash
#$ -M sngo@bowdoin.edu -m b -m e

./viewshed ../testGrids/usadem2.asc usadem2vis.asc 12750 5500