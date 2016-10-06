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
	// NODATA_value = 0;

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

	for (int i = 0; i < viewshedGrid.getNRows(); i++) {
		for (int j = 0; j < viewshedGrid.getNCols(); j++) {
			if (data[i][j] == NODATA_value) {
				// cout << "result of point (" << i << "," << j << ") is " << NODATA_value << endl;
				viewshedGrid.setData(i, j, NODATA_value);
			}
			else {
				float result = isVisible(vprow, vpcol, i , j);
				// cout << "result of point (" << i << "," << j << ") is " << result << endl;
				viewshedGrid.setData(i, j, result);
			}
		}
	}
}

//perform multiplying the data of the grid by a given multiplier
void Grid::multiply(float multiplier) {
	for (int r = 0; r < nRows; r++) {
		for (int c = 0; c < nCols; c++) {
			data[r][c] *= (data[r][c] == NODATA_value ? 1 : multiplier);
		}
	}
}

//compute flow direction for all points
void Grid::computeFD(Grid &FDgrid) {
	FDgrid.setHeader(header);

	for (int i = 0; i < nRows; i++) {
		for (int j = 0; j < nCols; j++) {
			if (data[i][j] == NODATA_value) {
				FDgrid.setData(i,j, NODATA_value);
				continue;
			}
			float max = INT_MIN;
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
									max = (float)data[i][j] - temp;
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
}

//compute flow accumulation for all points using dynamic programming
void Grid::computeFA(Grid &FAgrid, const Grid &FDgrid) {
	// Grid FAgrid(nRows, nCols, NODATA_value);
	FAgrid.resetFAData();
	FAgrid.setHeader(header);

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
}

//compute flow accumulation at one point using dynamic programming
float Grid::computeFAforPoint(int i, int j, const Grid &FDgrid, const Grid &FAgrid) {
	if (FAgrid.valueAt(i,j) != INITIAL_ACCUMULATION) {
		return FAgrid.valueAt(i,j);
	}

	float flow = 1.0;
	for (int dx = -1; dx <= 1; dx++) {
		for (int dy = -1; dy <= 1; dy++) {
			if (dx || dy) {
				if (inGrid(i + dx, j + dy)) {
					// float temp = FDgrid.valueAt(i + dx, j + dy);
					// if (temp )
					if (FDgrid.valueAt(i + dx, j + dy) == encodingDirection(-1 * dx, -1 * dy)) {
						flow += computeFAforPoint(i + dx, j + dy, FDgrid, FAgrid);
					}
				}
			}
		}
	}

	return flow;
}

//compute flow accumation for all points using quadratic recursion
void Grid::computeFAslow(Grid &FAgrid, const Grid &FDgrid) {
	FAgrid.resetFAData();
	FAgrid.setHeader(header);

	for (int i = 0; i < nRows; i++) {
		for (int j = 0; j < nCols; j++) {
			if (FDgrid.valueAt(i,j) == NODATA_value) {
				FAgrid.setData(i, j, NODATA_value);
			}
			else {
				FAgrid.setData(i,j, computeFAforPointSlow(i, j, FDgrid));
			}
		}
	}
}

//compute flow accumulation at one point using quadratic recursion
float Grid::computeFAforPointSlow(int i, int j, const Grid &FDgrid) {
	float flow = 1;
	for (int dx = -1; dx <= 1; dx++) {
		for (int dy = -1; dy <= 1; dy++) {
			if (dx || dy) {
				if (inGrid(i + dx, j + dy)) {
					if (FDgrid.valueAt(i + dx, j + dy) == encodingDirection(-1 * dx, -1 * dy)) {
						flow += computeFAforPointSlow(i + dx, j + dy, FDgrid);
					}
				}
			}
		}
	}

	return flow;
}

//set all data to a certain value
void Grid::setAllData(float value) {
	for (int r = 0; r < nRows; r++) {
		for (int c = 0; c < nCols; c++) {
			data[r][c] = value;
		}
	}
}

//resets all data of the grid to 0.0
void Grid::resetFAData() {
	for (int r = 0; r < nRows; r++) {
		for (int c = 0; c < nCols; c++) {
			data[r][c] = INITIAL_ACCUMULATION;
		}
	}
}

//set data at a particular point
int Grid::setData(int row, int col, int value) {
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
float Grid::isVisible(int vprow, int vpcol, int row, int col) {

	//check to make sure that the incoming point is in grid
	if (inGrid(row, col) == false) return NODATA_value;

	float vpheight = data[vprow][vpcol];

	// float height = data[row][col];
	float angle = verticalAngle(vprow, vpcol, row, col); 

	// cout << "looking at point (" << row << "," << col << ")" << endl;

	//edge case 1: points on the verticle/horizontal lines of coordinates

	//points on same horizontal line
	if (row == vprow) {
		// cout << "horizontal" << endl;
		if (col == vpcol) {
			return 1.0;
		}
		//left horizontal point
		else if (col < vpcol) {
			int temp = col + 1;
			while (temp < vpcol) {
				if (verticalAngle(vprow, vpcol, row, temp) > angle) return 0.0;
				temp++;
			}
			return 1.0;
		}
		//right horizontal point
		else {
			int temp = vpcol + 1;
			while (temp < col) {
				if (verticalAngle(vprow, vpcol, row, temp) > angle) return 0.0;
				temp++;
			}
			return 1.0;
		}
	}

	//points on same vertical line
	else if (col == vpcol) {
		// cout << "vertical" << endl;
		//upper point
		if (row < vprow) {
			int temp = row + 1;
			while (temp < vprow) {
				if (verticalAngle(vprow, vpcol, temp, col) > angle) return 0.0;
				temp++;
			}
			return 1.0;
		}
		//lower point
		else {
			int temp = vprow + 1;
			// cout << "looking at point (" << row << "," << col << ")" << endl;
			while (temp < row) {
				float result = verticalAngle(vprow, vpcol, temp, col);
				// cout << "vertical angle between (" << vprow << "," << vpcol << ") and (" << temp << "," << col << ") is " << result << endl;
				if (result > angle) return 0.0;
				temp++;
			}
			return 1.0;
		}
	}

	//edge case 2: points on the two vertical lines next to view point
	else if (abs(col - vpcol) == 1) {
		vector<float> rIntersects;
		for (int i = 1; i <= abs(vprow - row); i++) {
			rIntersects.push_back((float)(col - vpcol) * i / (float)(row - vprow));
		} 
		int index = 0;
		// cout << "edge case 2" << endl;
		if (abs(row - vprow) == 1) {
			return 1.0;
		}

		//points at higher row
		else if (row > vprow) {
			// cout << "size of rIntersects " << rIntersects.size() << endl;
			//if lower points have no data then assume that the point is visible
			// float avg = 0.0;
			// if (data[row-1][col] == NODATA_value || data[row-1][vpcol] == NODATA_value) {
			// 	avg = data[row-1][col] == NODATA_value ? data[row-1][vpcol] : data[row-1][col];
			// }
			// else {
			// 	avg = (float)(data[row-1][col] + data[row-1][vpcol])/2.0;
			// }
			// if (avg == NODATA_value) {
			// 	return 1.0;
			// }
			// float temp = verticalAngle(vpheight, avg, distance(vprow, vpcol, row - 1, (float)(col + vpcol)/2.0));
			// return (angle >= temp ? 1.0 : 0.0);

			while (index < rIntersects.size()) {
				float heightIntersect = rowInterpolate(vprow + index + 1, vpcol + rIntersects[index]);
				if (heightIntersect == NODATA_value) {
					index++;
					continue;
				}
				float temp = verticalAngle(vpheight, heightIntersect, distance(vprow, vpcol, vprow + index + 1, vpcol + rIntersects[index]));
				if (temp > angle) {
					return 0.0;
				}
				index++;
			}
			return 1.0;

		}

		//points at lower row
		else {
			// float avg = 0.0;
			// if (data[row+1][col] == NODATA_value || data[row+1][vpcol] == NODATA_value) {
			// 	avg = data[row+1][col] == NODATA_value ? data[row+1][vpcol] : data[row+1][col];
			// }
			// else {
			// 	avg = (float)(data[row+1][col] + data[row+1][vpcol])/2.0;
			// }
			// if (avg == NODATA_value) {
			// 	return 1.0;
			// }
			// float temp = verticalAngle(vpheight, avg, distance(vprow, vpcol, row + 1, (float)(col + vpcol) / 2.0));
			// return (angle >= temp ? 1.0 : 0.0);

			while (index < rIntersects.size()) {
				float heightIntersect = rowInterpolate(vprow - index - 1, vpcol - rIntersects[index]);
				if (heightIntersect == NODATA_value) {
					index++;
					continue;
				}
				float temp = verticalAngle(vpheight, heightIntersect, distance(vprow, vpcol, vprow - index - 1, vpcol - rIntersects[index]));
				if (temp > angle) {
					return 0.0;
				}
				index++;
			}
			return 1.0;
		}
	}

	vector<float> rIntersects;

	//initializing the y-coord for intersecting points relative to the viewpoint
	for (int i = 1; i <= abs(vpcol - col) - 1; i++) {
		rIntersects.push_back((float)(row - vprow) * i/(float)(col - vpcol));
	}

	int index = 0;

	//right points to view point
	if (col > vpcol) {
			// if (row == 204 && col == 127) {
				// cout << "rIntersects[index] = " << rIntersects[index] << " at index " << index << endl;
				// cout << "checking " << vprow + rIntersects[index] << " at index " << index << endl;
			// }
		while (index < rIntersects.size()) {
			float heightIntersect = columnInterpolate(vpcol + index + 1, vprow + rIntersects[index]);
			if (heightIntersect == NODATA_value) {
				index++;
				continue;
			}
			float temp = verticalAngle(vpheight, heightIntersect, distance(vprow, vpcol, vprow + rIntersects[index], vpcol + index + 1));
			// if (col == 2) {
				// cout << "angle = " << angle << " with temp = " << temp << endl;
			// }
			if (temp > angle) {
				return 0.0;
			}
			index++;
		}
		return 1.0;
	}
	//left points to view points
	else {
		while (index < rIntersects.size()) {
			float heightIntersect = columnInterpolate(vpcol - index - 1, vprow - rIntersects[index]);
			if (heightIntersect == NODATA_value) {
				index++;
				continue;
			}
			float temp = verticalAngle(vpheight, heightIntersect, distance(vprow, vpcol, vprow - rIntersects[index], vpcol - index - 1));
			if (temp > angle) {
				return 0.0;
			}
			index++;
		}
		return 1.0;
	}		
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

//find the distance between two points (vprow, vpcol) and (row, col)
float Grid::distance(float vprow, float vpcol, float row, float col) {
	if (vprow == row && vpcol == col) return INVALID_VALUE;
	return sqrt(pow(vprow - row, 2) + pow(vpcol - col, 2));
}

//find the vertical angle between two points (vprow, vpcol) and (row, col)
float Grid::verticalAngle(int vprow, int vpcol, int row, int col) {

	if (!(inGrid(vprow, vpcol) && inGrid(row, col))) {
		return INVALID_VALUE;
	}

	float height = data[row][col],
	      vpheight = data[vprow][vpcol];

	if (height == NODATA_value || vpheight == NODATA_value) {
		return NODATA_value;
	}

	float d = distance(vprow, vpcol, row, col);
	if (d == INVALID_VALUE) return INVALID_VALUE;
		
	return atan((float)(height - vpheight)/(float)d);
}

//find the vertical angle between two points given height and distance
float Grid::verticalAngle(float vpheight, float height, float distance) {
	if (distance == 0) return INVALID_VALUE;
	return atan((float)(height - vpheight) / (float)distance);
}

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