/*****************************************************************************
 File:   flow.cc
 Author: Son D. Ngo
 Date:   September 2016
 
 Description: Read in a grid ascii file and multiply the grid by the given
 multiplier
 
 Running instructions:
 Run this program with two arguments on the command line: the input file and 
 	the multiplier
 
 g++ -Wall -o main main.cpp
 ./main [input file name] [multiplier]
 
 Running on dover: 
 g++ -std=c++11 -Wall -o main main.cpp
 ./main [input file name] [multiplier]
  
 ******************************************************************************/

#include "grid.h"

int main(int argc, char* argv[]) {

	if (argc < 4) {
		cout << "Invalid number of arguments. Please make sure the command is the following format: " << endl;
		cout << "[path-to-executable-file] [grid-file-to-read] [flow-direction-file-to-write] [flow-accumulation-file-to-write" << endl;
		exit(1);
	}

	string input = argv[1];

	Grid grid;

	if (grid.readGridFromFile(input) == false) {
		cout << "Unable to read file. Exiting" << endl;
		exit(1);
	}

	grid.printGrid();

	Grid FDgrid = grid.computeFD();

	cout << "write to: " << FDgrid.writeToFile(argv[2]);

	cout << endl;

	FDgrid.printGrid();

	return 0;

}