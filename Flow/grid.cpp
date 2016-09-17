/*****************************************************************************
 File:   grid.cpp
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

Grid::Grid() {

}

Grid::~Grid() {
	delete[] data;
}

//perform reading the ascii file, return false if unsuccessful at
//any point
bool Grid::readGridFromFile(string gridFileName) {
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
	data = new (nothrow) float*[nRows];
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
	// grid.rows = nRows;
	// grid.cols = nCols;
	// grid.xllCorner = xllCorner;
	// grid.yllCorner = yllCorner;
	// grid.cellSize = cellSize;
	// grid.NODATA_value = NODATA_value;
	// grid.data = data;

	return true;
}

/* FUNCTIONALITIES WITH GRID */

//perform multiplying the data of the grid by a given multiplier
void Grid::multiply(float multiplier) {
	for (int r = 0; r < nRows; r++) {
		for (int c = 0; c < nCols; c++) {
			data[r][c] *= (data[r][c] == NODATA_value ? 1 : multiplier);
		}
	}
}

Grid Grid::computeFD(Grid &FDGrid) {
	
}

Grid Grid::computeFA(Grid &FAGrid) {

}

int Grid::setData(int row, int col, int value) {
	if (row < 0 || row >= nRows || col < 0 || col >= nCols) {
		return NODATA_value;
	}
	else {
		data[row][col] = value;
	}
	return value;
}

/* GRID HELPER FUNCTIONS */

void Grid::copyInfo(Grid &toCopy) {
	toCopy.data 
}

//print both the header and the values of the grid
void Grid::printGrid() {
	printHeader();
	printValues();
	// printInfo(grid);
}

//print important info regarding the grid
void Grid::printInfo() {
	cout << "nCols\t" << nCols << endl;
	cout << "nRows\t" << nRows << endl;
	cout << "xllCorner\t" << xllCorner << endl;
	cout << "yllCorner\t" << yllCorner << endl;
	cout << "cellSize\t" << cellSize << endl;
	cout << "NODATA_value\t" << NODATA_value << endl;
	cout << "h_min\t" << h_min << endl;
	cout << "h_max\t" << h_max << endl;
}

//print the header of the grid, containing important info
void Grid::printHeader() {
	cout << "nCols\t" << nCols << endl;
	cout << "nRows\t" << nRows << endl;
	cout << "xllCorner\t" << xllCorner << endl;
	cout << "yllCorner\t" << yllCorner << endl;
	cout << "cellSize\t" << cellSize << endl;
	cout << "NODATA_value\t" << NODATA_value << endl;
}

//print the data of the grid
void Grid::printValues() {
	for (int r = 0; r < nRows; r++) {
		for (int c = 0; c < nCols; c++) {
			cout << data[r][c] << " ";
		}
		cout << endl;
	}
}

/* UTILITY FUNCTIONS */

//scan for number in a given string
string Grid::numberTokenize(const string &input) {
	return input.substr(input.find_first_of("-123456789"));
}