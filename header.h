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
	int rows, cols;

	float **data;
} Grid;

string numberTokenize(const string &input);

int nCols, nRows, xllCorner, yllCorner, cellSize, NODATA_value;
int multiplier;