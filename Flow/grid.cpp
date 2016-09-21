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

Grid::Grid(int nRows, int nCols, int NODATA_value) {
	this->nRows = nRows;
	this->nCols = nCols;
	this->NODATA_value = NODATA_value;
	data = new (nothrow) float*[nRows];
	if (data == NULL) {
		cout << "Unsuccessful memory allocation. Exiting";
		exit(1);
	}
	for (int i = 0; i < nRows; i++) {
		data[i] = new (nothrow) float[nCols];
		if (data[i] == NULL) {
			cout << "Unsuccessful memory allocation. Exiting";
			exit(1);
		}
	}
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

//resets all data of the grid to 0.0
void Grid::resetData() {
	for (int r = 0; r < nRows; r++) {
		for (int c = 0; c < nCols; c++) {
			data[r][c] = INITIAL_ACCUMULATION;
		}
	}
}

Grid Grid::computeFD() {

	Grid FDgrid(nRows, nCols, NODATA_value);

	for (int i = 0; i < nRows; i++) {
		for (int j = 0; j < nCols; j++) {
			if (data[i][j] == NODATA_value) {
				FDgrid.setData(i,j, NODATA_value);
				continue;
			}
			float max = 0;
			int maxR, maxC;
			maxR = maxC = 0;
			for (int dx = -1; dx <= 1; dx++) {
				for (int dy = -1; dy <= 1; dy++) {
					if (dx || dy) {
						if (inGrid(i + dx, j + dy)) {
							float temp = data[i+dx][j+dy];
							if (temp == NODATA_value || temp >= data[i][j]) {
								if (dx + dy == 2 && max == INT_MIN) {
									//point is a pit or nodata
									max = NOFLOW_VALUE; 
								}
							}
							else {
								if (data[i][j] - temp > max) {
									max = data[i][j] - temp;
									maxR = dx;
									maxC = dy;
								}
							}
						}
					}
				}
			}
			FDgrid.setData(i,j, encodingDirection(maxR, maxC));
		}
	}

	return FDgrid;
}

Grid Grid::computeFA(const Grid &FDgrid) {
	Grid FAgrid(nRows, nCols, NODATA_value);
	FAgrid.resetData();

	for (int i = 0; i < nRows; i++) {
		for (int j = 0; j < nCols; j++) {
			if (FDgrid.valueAt(i,j) == NODATA_value) {
				FAgrid.setData(i, j, NODATA_value);
			}
			else {
				FAgrid.setData(i,j, computeFAforPoint(i, j, FDgrid, FAgrid));
			}
		}
	}

	return FAgrid;
}

float Grid::computeFAforPoint(int i, int j, const Grid &FDgrid, const Grid &FAgrid) {
	if (FAgrid.valueAt(i,j) != INITIAL_ACCUMULATION) {
		return FAgrid.valueAt(i,j);
	}

	float flow = 1;
	for (int dx = -1; dx <= 1; dx++) {
		for (int dy = -1; dy <= 1; dy++) {
			if (dx || dy) {
				if (inGrid(i + dx, i + dy)) {
					if (FDgrid.valueAt(i + dx, j + dy) == encodingDirection(-1 * dx, -1 * dy)) {
						flow += computeFAforPoint(i + dx, j + dy, FDgrid, FAgrid);
					}
				}
			}
		}
	}

	return flow;
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

string Grid::writeToFile(string path) {
	ofstream outputFile(path);
	outputFile << "nCols\t" << nCols << endl;
	outputFile << "nRows\t" << nRows << endl;
	outputFile << "xllCorner\t" << xllCorner << endl;
	outputFile << "yllCorner\t" << yllCorner << endl;
	outputFile << "cellSize\t" << cellSize << endl;
	outputFile << "NODATA_value\t" << NODATA_value << endl;

	for (int r = 0; r < nRows; r++) {
		for (int c = 0; c < nCols; c++) {
			outputFile << data[r][c] << " ";
		}
		outputFile << endl;
	}

	// char buffer[MAX_PATH];
	// GetModuleFileName(NULL, buffer, MAX_PATH);
	// size_type pos = string(buffer).find_last_of("\\/");
	// return string(buffer).substr(0, pos);

	return path;
}


/* GRID HELPER FUNCTIONS */

void Grid::copyInfo(Grid &toCopy) {
 	
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

//check if the coordinate is within grid
bool Grid::inGrid(int x, int y) const {
	return (x >= 0 && x < nRows && y >= 0 && y < nCols);
}

//encode flow direction into an integer
int Grid::encodingDirection(int r, int c) { 
	if (r == -1) {
		if (c == -1) {
			return 32;
		}
		else if (c == 0) {
			return 64;
		}
		else {
			return 128;
		}
	}
	else if (r == 0) {
		if (c == -1) { 
			return 16;
		}
		else if (c == 0) {
			return NOFLOW_VALUE;
		}
		else {
			return 1;
		}
	}
	else {
		if (c == -1) {
			return 8;
		}
		else if (c == 0) { 
			return 4;
		}
		else {
			return 2;
		}
	}	
}