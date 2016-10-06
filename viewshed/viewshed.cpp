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

bool isInteger(string str);

int main(int argc, char* argv[]) {

	if (argc != 5) {
		cout << "Invalid number of arguments. Please make sure the command is the following format: " << endl;
		cout << "./viewshed [grid-file-to-read] [file-to-write] x-coord y-coord" << endl;
		exit(1);
	}

	string input = argv[1];
	string output = argv[2];
	string inputPath;
	string fileName;

	if (isInteger(argv[3]) == false || isInteger(argv[4]) == false) {
		cout << "Invalid argument for coordinates. Please make sure it's a valid integer" << endl;
		exit(1);
	}

	int vprow = stoi(argv[3]), vpcol = stoi(argv[4]);

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

	// grid.printGrid();

	// cout << endl;

	Grid viewshedGrid(grid.getNRows(), grid.getNRows(), grid.getNODATA_value());

	grid.compute_viewshed(viewshedGrid, vprow, vpcol);

	//viewshedGrid.printGrid();

	cout << "write to " << viewshedGrid.writeToFile(inputPath + "/" + output) << endl;

	//cleanup memory
	grid.freeGridData();
	viewshedGrid.freeGridData();

	return 0;

}

//return true if a given string represents an integer, otherwise
//return false
bool isInteger(string str) {
	char start = str[0];
	if (isdigit(start) == false && start != '-') {
		return false;
	} 
	if (start == '0' && str.length() > 1) {
		return false;
	}
	for (int i = 1; i < str.length(); i++) {
		if (isdigit(str[i]) == false) return false;
	}

	return true;
}