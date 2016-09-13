/*****************************************************************************
 File:   main.cc
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

#include "header.h"

int main(int argc, char* argv[]) {

	if (argc < 3) {
		cout << "Invalid number of arguments. Please make sure the command is the following format: " << endl;
		cout << "[path to executable file] [grid file name] [multiplier]" << endl;
		exit(1);
	}

	string input = argv[1];

	if (isInteger(argv[2]) == false) {
		cout << "Invalid argument for multiplier. Please make sure it's a valid integer" << endl;
		cout << "Command format: [path to executable file] [grid file name] [multiplier]" << endl;
		exit(1);
	}

	multiplier = atoi(argv[2]);

	Grid grid;

	if (readGridFromFile(input, grid) == false) {
		cout << "Unable to read file. Exiting" << endl;
		exit(1);
	}

	printGrid(grid);

	cout << endl;

	multiply(grid, multiplier);

	printGrid(grid);

	return 0;

}

//perform reading the ascii file, return false if unsuccessful at
//any point
bool readGridFromFile(string gridFileName, Grid &grid) {
	string line;
	ifstream myFile;
	myFile.open(gridFileName);

	//return false if file path is invalid or file is unopenable
	if (myFile.is_open() == false) {
		return false;
	}

	//getting important information about the grid
	getline(myFile,line);
	nCols = stoi(numberTokenize(line));

	getline(myFile,line);
	nRows = stoi(numberTokenize(line));

	getline(myFile,line);
	xllCorner = stof(numberTokenize(line));

	getline(myFile,line);
	yllCorner = stof(numberTokenize(line));

	getline(myFile,line);
	cellSize = stof(numberTokenize(line));

	getline(myFile,line);
	NODATA_value = stof(numberTokenize(line));

	//start allocating memory to the grid data 2D-array
	float **data = new (nothrow) float*[nRows];
	if (data == NULL) {
		return false;
	}
	for (int i = 0; i < nRows; i++) {
		data[i] = new (nothrow) float[nCols];
		if (data[i] == NULL) {
			return false;
		}
	}

	//start filling the grid with data
	int r = 0;
	while (getline(myFile,line)) {
		stringstream ss(line);
		for (int c = 0; c < nCols; c++) {
			ss >> data[r][c];
		}
		r++;
	}

	//storing important information
	grid.rows = nRows;
	grid.cols = nCols;
	grid.xllCorner = xllCorner;
	grid.yllCorner = yllCorner;
	grid.cellSize = cellSize;
	grid.NODATA_value = NODATA_value;
	grid.data = data;

	return true;
}

/* GRID OPERATIONS FUNCTIONS */

//print both the header and the values of the grid
void printGrid(const Grid &grid) {
	printHeader(grid);
	printValues(grid);
	// printInfo(grid);
}

//print important info regarding the grid
void printInfo(const Grid &grid) {
	cout << "nCols\t" << grid.cols << endl;
	cout << "nRows\t" << grid.rows << endl;
	cout << "xllCorner\t" << xllCorner << endl;
	cout << "yllCorner\t" << yllCorner << endl;
	cout << "cellSize\t" << cellSize << endl;
	cout << "NODATA_value\t" << NODATA_value << endl;
	cout << "h_min\t" << grid.h_min << endl;
	cout << "h_max\t" << grid.h_max << endl;
}

//print the header of the grid, containing important info
void printHeader(const Grid &grid) {
	cout << "nCols\t" << grid.cols << endl;
	cout << "nRows\t" << grid.rows << endl;
	cout << "xllCorner\t" << grid.xllCorner << endl;
	cout << "yllCorner\t" << grid.yllCorner << endl;
	cout << "cellSize\t" << grid.cellSize << endl;
	cout << "NODATA_value\t" << grid.NODATA_value << endl;
}

//print the data of the grid
void printValues(const Grid &grid) {
	for (int r = 0; r < grid.rows; r++) {
		for (int c = 0; c < grid.cols; c++) {
			cout << (grid.data)[r][c] << " ";
		}
		cout << endl;
	}
}

//perform multiplying the data of the grid by a given multiplier
void multiply(Grid &grid, float multiplier) {
	for (int r = 0; r < grid.rows; r++) {
		for (int c = 0; c < grid.cols; c++) {
			(grid.data)[r][c] *= ((grid.data)[r][c] == NODATA_value ? 1 : multiplier);
		}
	}
}

/* UTILITY FUNCTIONS */

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

//scan for number in a given string
string numberTokenize(const string &input) {
	return input.substr(input.find_first_of("-123456789"));
}