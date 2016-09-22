/*****************************************************************************
 File:   flow.cpp
 Author: Son D. Ngo
 Date:   September 2016
 
 Description: Read in a grid in ascii format and compute its flow direction and 
 flow accumulation. Write each computation to a new file whose names are 
 specified by the user.
 
 Running instructions:
 Run this program with 3 arguments on the command line: the input file, output
 file for flow direction and output file for flow accumulation
 
 g++ -Wall -o flow flow.cpp grid.cpp
 ./flow [grid-file-to-read] [FD-file-to-write] [FA-file-to-write]
 
 Running on dover: 
 g++ -std=c++11 -Wall -o flow flow.cpp grid.cpp
 ./flow [grid-file-to-read] [FD-file-to-write] [FA-file-to-write]
  
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

	// cout << "filePath = " << inputPath << endl;
	// cout << "fileName = " << fileName << endl;

	Grid grid;

	if (grid.readGridFromFile(input) == false) {
		cout << "Unable to read file. Exiting" << endl;
		exit(1);
	}

	// grid.printGrid();

	Grid FDgrid = grid.computeFD();

	cout << endl;

	// cout << "write to: " << FDgrid.writeToFile(inputPath + argv[2]) << endl;

	// FDgrid.printGrid();

	cout << endl;

	// UNCOMMENT THE CODES BELOW TO COMPARE DYNAMIC PROGRAMMING WITH QUADRATIC RECURSION

	double start, end;

	start = clock();

	Grid badFAgrid = grid.computeFAslow(FDgrid);

	end = clock(); 

	cout << "Running time for inefficient quadratic recursion: " << (end-start)/CLOCKS_PER_SEC << " seconds" << endl;

	start = clock();

	Grid FAgrid = grid.computeFA(FDgrid);

	end = clock();

	cout << "Running time for recursion with dynamic programming: " << (end-start)/CLOCKS_PER_SEC << " seconds" << endl;

	// cout << "write to: " << FAgrid.writeToFile(inputPath + argv[3]) << endl;

	// FAgrid.printGrid();

	return 0;

}