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
//in the grid from the source view point
void Grid::compute_viewshed(Grid &viewshedGrid, int vprow, int vpcol) {
	viewshedGrid.setHeader(header);

	if (inGrid(vprow, vpcol)) {
		if (data[vprow][vpcol] == NODATA_value) {
			viewshedGrid.setAllData(NODATA_value);
			return;
		}
		else {
			viewshedGrid.setAllData(INITIAL_VIEW);
		}
	}
	else {
		cout << "SOURCE POINT IS OUT OF BOUND. Exiting..." << endl;
		exit(1);
	}

	//pointer to vertical angle grid, storing vertical angle of all points from view point
	Grid* VAGridP = new Grid(nRows, nCols, NODATA_value);
	VAGridP->setAllData(INITIAL_ANGLE);

	for (int i = 0; i < viewshedGrid.getNRows(); i++) {
		for (int j = 0; j < viewshedGrid.getNCols(); j++) {
			viewshedGrid.setData(i, j, isVisible(vprow, vpcol, i, j, VAGridP));
		}
	}

	VAGridP->freeGridData();
	delete VAGridP;
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
	for (int i = 0; i < header.size(); i++) {
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

//check if a point is visible from a source. Take on 3 possible
//values: NODATA_value, 1.0 for true, 0.0 for false
float Grid::isVisible(int vprow, int vpcol, int row, int col, Grid *VAGridP) {

	//check to make sure that the incoming point is in grid
	if (inGrid(row, col) == false) {
		return 0.0;
	}

	//direct neighbors are always visible (regardless of data value)
	if (abs(vprow - row) == 1 && abs(vpcol - col) == 1) {
		return 1.0;
	}

	//NODATA_value point is invisible
	if (data[row][col] == NODATA_value) {
		return 0.0;
	}

	//viewpoint itself is always visible
	if (vprow == row && vpcol == col) {
		return 1.0;
	}

	float vpheight = data[vprow][vpcol],
		  angle = VAGridP->valueAt(row, col) == INITIAL_ANGLE ? 
					VAGridP->setData(row, col, atan((float)(data[row][col] - vpheight) / distance(vprow, vpcol, row, col))) : 
					VAGridP->valueAt(row, col);

	//if vertical angle is invalid then assume the point is invisible
	// if (angle == INVALID_VALUE) {
	// 	return 0.0;
	// }

	/* EDGE CASE 1: points on the verticle/horizontal lines of coordinates */

	//points on same horizontal line
	if (row == vprow) {

		//left horizontal point
		if (col < vpcol) {
			int temp = col + 1;
			float tempAngle;
			while (temp < vpcol) {
				tempAngle = VAGridP->valueAt(row, temp) == INITIAL_ANGLE ? 
					VAGridP->setData(row, temp, atan((float)(data[row][temp] - vpheight) / distance(vprow, vpcol, row, temp))) : 
					VAGridP->valueAt(row, temp);
				if (tempAngle > angle) return 0.0;
				temp++;
			}
			return 1.0;
		}

		//right horizontal point
		else {
			int temp = vpcol + 1;
			float tempAngle;
			while (temp < col) {
				tempAngle = VAGridP->valueAt(row, temp) == INITIAL_ANGLE ? 
					VAGridP->setData(row, temp, atan((float)(data[row][temp] - vpheight) / distance(vprow, vpcol, row, temp))) : 
					VAGridP->valueAt(row, temp);
				if (tempAngle > angle) return 0.0;
				temp++;
			}
			return 1.0;
		}
	}

	//points on same vertical line
	else if (col == vpcol) {

		//point at lower row 
		if (row < vprow) {
			int temp = row + 1;
			float tempAngle;
			while (temp < vprow) {
				tempAngle = VAGridP->valueAt(temp, col) == INITIAL_ANGLE ? 
					VAGridP->setData(temp, col, atan((float)(data[temp][col] - vpheight) / distance(vprow, vpcol, temp, col))) : 
					VAGridP->valueAt(temp, col);
				if (tempAngle > angle) return 0.0;
				temp++;
			}
			return 1.0;
		}

		//point at higher row
		else {
			int temp = vprow + 1;
			float tempAngle;
			while (temp < row) {
				tempAngle = VAGridP->valueAt(temp, col) == INITIAL_ANGLE ? 
					VAGridP->setData(temp, col, atan((float)(data[temp][col] - vpheight) / distance(vprow, vpcol, temp, col))) : 
					VAGridP->valueAt(temp, col);
				if (tempAngle > angle) return 0.0;
				temp++;
			}
			return 1.0;
		}
	}

	/* END EDGE CASE 1 */

	/* EDGE CASE 2: points on the two vertical lines next to view point */
	else if (abs(col - vpcol) == 1) {
		return horizontalVisible(vprow, vpcol, row, col, vpheight, angle);
	}
	else if (abs(row - vprow) == 1) {
		return verticalVisible(vprow, vpcol, row, col, vpheight, angle);
	}

	/* END EDGE CASE 2 */

	//regular case, finding all intersecting points, both horizontally and vertically, then 
	//compare the vertical angle of each point with the LOS's vertical angle

	float verticalResult = verticalVisible(vprow, vpcol, row, col, vpheight, angle);
	if (verticalResult == 1.0) return horizontalVisible(vprow, vpcol, row, col, vpheight, angle);
	else return 0.0;
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
	for (int i = 0; i < header.size(); i++) {
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

	float upperHeight = data[(int)ceil(x)][col],
		  lowerHeight = data[(int)floor(x)][col];

	if (upperHeight == NODATA_value || lowerHeight == NODATA_value) {
		return (upperHeight == NODATA_value ? lowerHeight : upperHeight);
	}

	float slope = upperHeight - lowerHeight,
		  intercept = upperHeight - slope * ceil(x);

	return slope * x + intercept;
}

//find the height of point y (col) by interpolating its two neighbors in the same row
float Grid::rowInterpolate(int row, float y) {
	float rightHeight = data[row][(int)ceil(y)],
		  leftHeight = data[row][(int)floor(y)];

	if (rightHeight == NODATA_value || leftHeight == NODATA_value) {
		return (rightHeight == NODATA_value ? leftHeight : rightHeight);
	}

	float slope = rightHeight - leftHeight,
		  intercept = rightHeight - slope * ceil(y);

	return slope * y + intercept;
}

//check if any point intersecting horizontal lines blocks the line of sight
float Grid::horizontalVisible(int vprow, int vpcol, int row, int col, float vpheight, float angle) {
	int index = 1;

	//points at higher row
	if (row > vprow) {
		while (index < abs(vprow - row)) {
			float tempRow = vprow + index,
				  tempCol = vpcol + (float(col - vpcol) * index / (float)(row - vprow)),
			      heightIntersect = rowInterpolate(tempRow, tempCol);
			if (heightIntersect == NODATA_value) {
				index++;
				continue;
			}
			float tempAngle = atan((float)(heightIntersect - vpheight) / distance(vprow, vpcol, tempRow, tempCol));
			if (tempAngle > angle) {
				return 0.0;
			}
			index++;
		}
		return 1.0;
	}

	//points at lower row
	else {
		while (index < abs(vprow - row)) {
			float tempRow = vprow - index,
				  tempCol = vpcol - (float)(col - vpcol) * index / (float)(row - vprow),
				  heightIntersect = rowInterpolate(tempRow, tempCol);
			if (heightIntersect == NODATA_value) {
				index++;
				continue;
			}
			float tempAngle = atan((float)(heightIntersect - vpheight) / distance(vprow, vpcol, tempRow, tempCol));
			if (tempAngle > angle) {
				return 0.0;
			}
			index++;
		}
		return 1.0;
	}
}

//check if any point intersecting vertical lines blocks the line of sight
float Grid::verticalVisible(int vprow, int vpcol, int row, int col, float vpheight, float angle) {
	int index = 1;

	//right points to view point
	if (col > vpcol) {
		while (index < abs(vpcol - col)) {
			float tempCol = vpcol + index,
				  tempRow = vprow + (float)(row - vprow) * index /(float)(col - vpcol),
			      heightIntersect = columnInterpolate(tempCol, tempRow);
			if (heightIntersect == NODATA_value) {
				index++;
				continue;
			}
			float tempAngle = atan((float)(heightIntersect - vpheight) / distance(vprow, vpcol, tempRow, tempCol));
			if (tempAngle > angle) {
				return 0.0;
			}
			index++;
		}
		return 1.0;
	}

	//left points to view point
	else {
		while (index < abs(vpcol - col)) {
			float tempCol = vpcol - index,
				  tempRow = vprow - (float)(row - vprow) * index /(float)(col - vpcol),
			      heightIntersect = columnInterpolate(tempCol, tempRow);
			if (heightIntersect == NODATA_value) {
				index++;
				continue;
			}
			float tempAngle = atan((float)(heightIntersect - vpheight) / distance(vprow, vpcol, tempRow, tempCol));
			if (tempAngle > angle) {
				return 0.0;
			}
			index++;
		}
		return 1.0;
	}		
}

//find the distance between two points (vprow, vpcol) and (row, col)
float Grid::distance(float vprow, float vpcol, float row, float col) {
	return sqrt(pow(vprow - row, 2) + pow(vpcol - col, 2));
}

// //find the vertical angle between two points given height and distance
// float Grid::verticalAngle(float heightDistance, float distance) {
// 	// if (distance == 0) return INVALID_VALUE;
// 	return atan((float)(heightDistance) / (float)distance);
// }

//scan for number in a given string
string Grid::numberTokenize(const string &input) {
	return input.substr(input.find_first_of("-0123456789"));
}

//check if the coordinate is within grid
bool Grid::inGrid(int x, int y) const {
	return (x >= 0 && x < nRows && y >= 0 && y < nCols);
}