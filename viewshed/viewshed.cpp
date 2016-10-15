/*****************************************************************************
 File:   viewshed.cpp
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
 
 g++ -Wall -o viewshed viewshed.cpp grid.cpp
 ./viewshed [grid-file-to-read] [file-to-write] x-coord y-coord
 
 Running on dover: 
 g++ -std=c++11 -Wall -o viewshed viewshed.cpp grid.cpp
 ./viewshed [grid-file-to-read] [file-to-write] x-coord y-coord
  
 ******************************************************************************/

#include "grid.h"

bool isInteger(string str);
void testFunction();

int main(int argc, char* argv[]) {

	//for testing purpose only
	if (argc == 2 && strcmp(argv[1], "test") == 0) {
		testFunction();
		return 0;
	}

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

	Grid viewshedGrid(grid.getNRows(), grid.getNCols(), grid.getNODATA_value());

	cout << "middle point: (" << grid.getNRows()/2 << "," << grid.getNCols()/2 << ")" << endl;

	float start, end;

	start = clock();

	grid.compute_viewshed(viewshedGrid, vprow, vpcol);

	end = clock(); 

	cout << "time to compute viewshed: " << (float)(end - start) / CLOCKS_PER_SEC << endl;

	cout << "write to " << viewshedGrid.writeToFile(inputPath + output) << endl;

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
	for (unsigned int i = 1; i < str.length(); i++) {
		if (isdigit(str[i]) == false) return false;
	}

	return true;
}

//function is written for testing purpose
void testFunction() {
	string path = "/Users/sngo/Desktop/GIS-TestGrids/";
	string tests[] = {"brunsdem", "kaweah", "sierra", "portland_me", "usadem2", "eelriver", "washington"};
	// string tests[] = {"brunsdem", "kaweah"};

	Grid grid;

	for (unsigned int i = 0; i < sizeof(tests)/sizeof(string); i++) {
		grid.readGridFromFile(path + tests[i] + ".asc");
		Grid viewshedGrid(grid.getNRows(), grid.getNCols(), grid.getNODATA_value());
		cout << "middle point: (" << grid.getNRows()/2 << "," << grid.getNCols()/2 << ")" << endl;
		float start, end;
		start = clock();
		grid.compute_viewshed(viewshedGrid, grid.getNRows()/2, grid.getNCols()/2);
		end = clock(); 
		cout << "time to compute viewshed " << tests[i] << ": " << (float)(end - start) / CLOCKS_PER_SEC << endl;
		// cout << "write to " << viewshedGrid.writeToFile(path + tests[i] + "vis.asc") << endl;
		grid.freeGridData();
	}

	// grid.freeGridData();
}