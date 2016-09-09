/*****************************************************************************
 File:   main.cc
 Author: Son D. Ngo
 Date:   September 2016
 
 Description:

 
 Running instructions:
 Run this program with one argument on the command line
 the input file and the algorithm
 
 g++ -Wall -o main main.cpp
 ./main [input file name]
 
 Running on dover: 
 g++ -std=c++11 -Wall -o main main.cpp
 ./main [input file name]
  
 ******************************************************************************/

#include "header.h"

int main(int argc, char* argv[]) {

	if (argc < 3) {
		exit(0);
	}

	multiplier = atoi(argv[2]);

	string input = argv[1];
	string line;

	ifstream myFile;
	myFile.open(input);

	if (myFile.is_open() == false) {
		exit(0);
	}

	// stringstream ss;

	// getline(myFile,line);
	// stringstream(line) >> nCols;
	// getline(myFile,line);
	// stringstream(line) >> nRows;
	// getline(myFile,line);
	// stringstream(line) >> xllCorner;
	// getline(myFile,line);
	// stringstream(line) >> yllCorner;
	// getline(myFile,line);
	// stringstream(line) >> cellSize;
	// getline(myFile,line);
	// stringstream(line) >> NODATA_value;

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



	//start storing grids
	float **data = new float*[nRows];
	for (int i = 0; i < nRows; i++) {
		data[i] = new float[nCols];
	}

	int r = 0;
	while (getline(myFile,line)) {
		// size_t start = line.find_first_not_of(" "), end;

		stringstream ss(line);

		for (int c = 0; c < nCols; c++) {
			// end = line.find_first_not_of(" ", start + 1);
			// cout << line.substr(start + 1, end - start - 1) << endl;
			// data[r][c] = stof(line.substr(start + 1, end - start - 1));
			// start = end;
			ss >> data[r][c];
		}

		// cout << "here" << endl;
		r++;
	}

	cout << nCols << endl;
	cout << nRows << endl;
	cout << xllCorner << endl;
	cout << yllCorner << endl;
	cout << cellSize << endl;
	cout << NODATA_value << endl;

	for (int r = 0; r < nRows; r++) {
		for (int c = 0; c < nCols; c++) {
			cout << data[r][c] * (data[r][c] == NODATA_value ? 1 : multiplier) << " ";
		}
		cout << endl;
	}

	return 1;

}

string numberTokenize(const string &input) {
	return input.substr(input.find_first_of("-123456789"));
}