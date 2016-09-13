/* include files */
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstddef>

using namespace std;

typedef struct Grid {
	int rows, cols, xllCorner, yllCorner, cellSize;
	float NODATA_value;
	float h_min, h_max;
	float **data;
} Grid;

string numberTokenize(const string &input);
bool readGridFromFile(string gridFileName, Grid &grid);
void printGrid(const Grid &grid);
void printInfo(const Grid &grid);
void printHeader(const Grid &grid);
void printValues(const Grid &grid);
void multiply(Grid &grid, float multiplier);
bool isInteger(string str);

int nCols, nRows, xllCorner, yllCorner, cellSize, NODATA_value;
float multiplier;