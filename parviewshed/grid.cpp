/*****************************************************************************
 File:   grid.cpp
 Author: Son D. Ngo
 Date:   September 2016
 
 Description: Cpp file for class Grid
  
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
		cout << "Unsuccessful memory allocation. Exiting" << endl;
		exit(1);
	}
	for (int i = 0; i < nRows; i++) {
		data[i] = new (nothrow) float[nCols];
		if (data[i] == NULL) {
			cout << "Unsuccessful memory allocation. Exiting" << endl;
			exit(1);
		}
	}

	numThreads = DEFAULT_NUM_THREADS;
}

Grid::~Grid() {

}

//perform reading the ascii file, return false if unsuccessful at
//any point
bool Grid::readGridFromFile(string gridFileName) {
	string line;
	ifstream myFile;
	cout << "open file " << gridFileName << endl;
	myFile.open(gridFileName.c_str());

	//return false if file path is invalid or file is unopenable
	if (myFile.is_open() == false) {
		cout << "Unable to open " << gridFileName << endl;
		return false;
	}

	header.clear();

	//getting important information about the grid
	getline(myFile,line);
	header.push_back(line);

	nCols = stoi(numberTokenize(line));

	getline(myFile,line);
	header.push_back(line);
	nRows = stoi(numberTokenize(line));

	getline(myFile,line);
	header.push_back(line);
	xllCorner = stof(numberTokenize(line));

	getline(myFile,line);
	header.push_back(line);
	yllCorner = stof(numberTokenize(line));

	getline(myFile,line);
	header.push_back(line);
	cellSize = stof(numberTokenize(line));

	getline(myFile,line);
	header.push_back(line);
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

//compute the viewshed of a source view point, by checking the visibility of each point
//in the grid from the source view point. We will first compute the visibility for points 
//on the same horizontal and vertical lines. This process can be done in one, instead of 
//having to recompute using interpolation. 
void Grid::compute_viewshed(Grid &viewshedGrid, int vprow, int vpcol) {

	viewshedGrid.setHeader(header);

	float vpheight;

	if (inGrid(vprow, vpcol)) {
		if ((vpheight = data[vprow][vpcol]) == NODATA_value) {
			viewshedGrid.setAllData(NODATA_value);
			return;
		}
		else {
			viewshedGrid.setData(vprow, vpcol, 1.0);
		}
	}
	else {
		cout << "SOURCE POINT IS OUT OF BOUND. Exiting..." << endl;
		exit(1);
	}

	//point on the same horizontal line
	for (int i = -1; i <= 1; i += 2) {
		int index = vprow + i * 1;

		//set direct neighbor visible if hasData
		if (inGrid(index, vpcol)) {
			if (data[index][vpcol] == NODATA_value) {
				viewshedGrid.setData(index, vpcol, 0.0);
			}
			else {
				viewshedGrid.setData(index, vpcol, 1.0);
			}
		}

		float maxAngle = (float) (data[index][vpcol] - vpheight) / abs(vprow - index),
		      height;
		index += i;

		while (index >= 0 && index < nRows) {
			height = data[index][vpcol];
			if (height == NODATA_value) {
				viewshedGrid.setData(index, vpcol, 0.0);
				index += i;
				continue;
			}
			float tempAngle = (height - vpheight) / (float)abs(vprow - index);
			if (tempAngle > maxAngle) {
				maxAngle = tempAngle;
				viewshedGrid.setData(index, vpcol, 1.0);
			}
			else {
				viewshedGrid.setData(index, vpcol, 0.0);
			}
			index += i;
		}
	}

	//point on the same vertical line
	for (int i = -1; i <= 1; i += 2) {
		int index = vpcol + i * 1;

		//set direct neighbor visible if hasData
		if (inGrid(vprow, index)) {
			if (data[vprow][index] == NODATA_value) {
				viewshedGrid.setData(vprow, index, 0.0);
			}
			else {
				viewshedGrid.setData(vprow, index, 1.0);
			}
		}

		float maxAngle = (float) (data[vprow][index] - vpheight) / abs(vpcol - index),
			  height;
		index += i;

		while (index >= 0 && index < nCols) {
			height = data[vprow][index];
			if (height == NODATA_value) {
				viewshedGrid.setData(vprow, index, 0.0);
				index += i;
				continue;
			}
			float tempAngle = (height - vpheight) / (float)abs(vpcol - index);
			if (tempAngle > maxAngle) {
				maxAngle = tempAngle;
				viewshedGrid.setData(vprow, index, 1.0);
			}
			else {
				viewshedGrid.setData(vprow, index, 0.0);
			}
			index += i;
		}
	}

	// all other points

	// omp_set_num_threads(numThreads);

	int i, j;

	// #pragma omp parallel private(i,j)
	// {

		// printf("Number of threads: %d\n", omp_get_num_threads());

		#pragma omp parallel for schedule(dynamic, 16) private (i,j)
			for (i = 0; i < nRows; i++) {
				for (j = 0; j < nCols; j++) {
					if (data[i][j] == NODATA_value) {
						viewshedGrid.setData(i, j, 0.0);
					}
					else {
						if (i == vprow || j == vpcol) continue;
						viewshedGrid.setData(i, j, isVisible(vprow, vpcol, i, j));
					}
				}
			}
	// }

}

//set all data to a certain value
void Grid::setAllData(float value) {
	for (int r = 0; r < nRows; r++) {
		for (int c = 0; c < nCols; c++) {
			data[r][c] = value;
		}
	}
}

//set data at a particular point
float Grid::setData(int row, int col, float value) {
	if (row < 0 || row >= nRows || col < 0 || col >= nCols) {
		return NODATA_value;
	}
	else {
		data[row][col] = value;
	}
	return value;
}

//write the grid to file in ascii format
string Grid::writeToFile(string path) {
	ofstream outputFile(path);
	for (unsigned int i = 0; i < header.size(); i++) {
		outputFile << header[i] << endl;
	}

	for (int r = 0; r < nRows; r++) {
		for (int c = 0; c < nCols; c++) {
			outputFile << data[r][c] << " ";
		}
		outputFile << endl;
	}

	return path;
}


/* GRID HELPER FUNCTIONS */

//check if a point is visible from a source. Take on 2 possible
//values: 1.0 for true, 0.0 for false or NODATA
float Grid::isVisible(int vprow, int vpcol, int row, int col) {

	//check to make sure that the incoming point is in grid
	// if (inGrid(row, col) == false) {
	// 	return 0.0;
	// }

	// //direct neighbors are always visible (except NODATA)
	// if (abs(vprow - row) <= 1 && abs(vpcol - col) <= 1) {
	// 	return 1.0;
	// }

	// cout << "row = " << row << " and col = " << col << endl;

	// NODATA_value point is invisible
	// if (data[row][col] == NODATA_value) {
	// 	return 0.0;
	// }

	// cout << "Success" << endl;

	float vpheight = data[vprow][vpcol];

	/* EDGE CASE: points on the two vertical/horizontal lines next to view point */
	if (abs(col - vpcol) == 1) {
		// cout << "hello" << endl;
		return horizontalVisible(vprow, vpcol, row, col, vpheight);
	}
	else if (abs(row - vprow) == 1) {
		// cout << "hello2" << endl;
		return verticalVisible(vprow, vpcol, row, col, vpheight);
	}

	/* END EDGE CASE */

	//regular case, finding all intersecting points, both horizontally and vertically, then 
	//compare the vertical angle of each point with the LOS's vertical angle

	float verticalResult = verticalVisible(vprow, vpcol, row, col, vpheight);

	if (verticalResult == 1.0) {
		return horizontalVisible(vprow, vpcol, row, col, vpheight);
	}

	return 0.0;
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
	for (unsigned int i = 0; i < header.size(); i++) {
		cout << header[i] << endl;
	}
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

//find the height of point x (row) by interpolating its two neighbors in the same column
float Grid::columnInterpolate(int col, float x) {

	int upperIndex = (int)ceil(x);

	float upperHeight = data[upperIndex][col],
		  lowerHeight = data[upperIndex - 1][col];

	if (upperHeight == NODATA_value || lowerHeight == NODATA_value) {
		// return (upperHeight == NODATA_value ? lowerHeight : upperHeight);
		return NODATA_value;
	}

	return (upperHeight - lowerHeight) * (x - upperIndex) + upperHeight;
}

//find the height of point y (col) by interpolating its two neighbors in the same row
float Grid::rowInterpolate(int row, float y) {

	int rightIndex = (int)ceil(y);

	float rightHeight = data[row][rightIndex],
		  leftHeight = data[row][rightIndex - 1];

	if (rightHeight == NODATA_value || leftHeight == NODATA_value) {
		// return (rightHeight == NODATA_value ? leftHeight : rightHeight);
		return NODATA_value;
	}

	return (rightHeight - leftHeight) * (y - rightIndex) + rightHeight;
}

//check if any point intersecting horizontal lines blocks the line of sight
float Grid::horizontalVisible(int vprow, int vpcol, int row, int col, float vpheight) {

	int dc = col - vpcol,
		dr = row - vprow,
		absDr = abs(dr),
		index = absDr - 1,
		coeff = (row > vprow ? 1 : -1); 	//adjust the coefficient to reflect lower/higher row
	float intersectDistance = coeff * (float)dc / dr;  

	while (index) {
		float dy = intersectDistance * index,
			  heightIntersect;

		//if point is on 45-diagonal	  
		if (absDr == abs(dc)) {
			heightIntersect = data[vprow + coeff * index][vpcol + (int)dy];
		}
		else {
			heightIntersect = rowInterpolate(vprow + coeff * index, vpcol + dy);
		}
		if (heightIntersect == NODATA_value) {
			index--;
			continue;
		}

		if ((heightIntersect - vpheight) * absDr > (data[row][col] - vpheight) * index) {
			return 0.0;
		}
		index--;
	}
	return 1.0;
}

//check if any point intersecting vertical lines blocks the line of sight
float Grid::verticalVisible(int vprow, int vpcol, int row, int col, float vpheight) {
	int dc = col - vpcol,
		dr = row - vprow,
		absDc = abs(dc),
		index = 1,
		coeff = (col > vpcol ? 1 : -1); 	//adjust the coefficient to reflect left/right column

	float intersectDistance = coeff * (float)dr / dc;

	while (index < absDc) {
		float dx = intersectDistance * index,
			  heightIntersect;

		//if point is on 45-diagonal
		if (absDc == abs(dr)) {
			heightIntersect = data[vprow + (int)dx][vpcol + coeff * index];
		} 	  
		else {
			heightIntersect = columnInterpolate(vpcol + coeff * index, vprow + dx);
		}

		if (heightIntersect == NODATA_value) {
			index++;
			continue;
		}

		if ((heightIntersect - vpheight) * absDc > (data[row][col] - vpheight) * index) {
			return 0.0;
		}
		index++;
	}
	return 1.0;		
}

//scan for number in a given string
string Grid::numberTokenize(const string &input) {
	return input.substr(input.find_first_of("-0123456789"));
}

//check if the coordinate is within grid
bool Grid::inGrid(int x, int y) const {
	return (x >= 0 && x < nRows && y >= 0 && y < nCols);
}