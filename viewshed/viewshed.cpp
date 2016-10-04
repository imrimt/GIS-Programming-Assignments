/*****************************************************************************
 File:   viewshet.cpp
 Author: Son D. Ngo
 Date:   September 2016
 
 Description: Read in a grid in ascii format and compute its flow direction and 
 flow accumulation. Write each computation to a new file whose names are 
 specified by the user.
 
 Running instructions:
 Run this program with 4 arguments on the command line: 
	input file 
	output file (path) 
	x-coord of source  
	y-coord of source
 
 g++ -Wall -o flow flow.cpp grid.cpp
 ./viewshed [grid-file-to-read] [file-to-write] x-coord y-coord
 
 Running on dover: 
 g++ -std=c++11 -Wall -o flow flow.cpp grid.cpp
 ./viewshed [grid-file-to-read] [file-to-write] x-coord y-coord
  
 ******************************************************************************/

#include "grid.h"

int main(int argc, char* argv[]) {

	if (argc != 4) {
		cout << "Invalid number of arguments. Please make sure the command is the following format: " << endl;
		cout << "[path-to-executable-file] [grid-file-to-read] [flow-direction-file-to-write] [flow-accumulation-file-to-write]" << endl;
		exit(1);
	}

	string input = argv[1];
	string inputPath;
	string fileName;

	if (input.find_last_of("/") != string::npos) {
		int pos = input.find_last_of("/");
		inputPath = input.substr(0, pos + 1);
		fileName = input.substr(input.find_last_of("/") + 1);
	} 
	else {
		fileName = input;
	}

	Grid grid;

	if (grid.readGridFromFile(input) == false) {
		cout << "Unable to read file. Exiting" << endl;
		exit(1);
	}

	//cleanup memory
	grid.freeGridData();

	return 0;

}