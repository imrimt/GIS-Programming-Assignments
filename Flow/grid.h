/* include files */
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstddef>

const float NOFLOW_VALUE = -1;
const float INITIAL_ACCUMULATION = 0.0;

using namespace std;

class Grid {

	public:
		Grid();
		Grid(int nRows, int nCols, int NODATA_value);
		~Grid();

		Grid computeFD();
		Grid computeFA(const Grid &FDGrid);

		float computeFAforPoint(int r, int c, const Grid &FDgrid, const Grid &FAgrid);

		bool readGridFromFile(string gridFileName);

		void resetData();
		void printGrid();
		void printHeader();
		void printValues();
		void multiply(float multiplier);

		int valueAt(int r, int c) const {if (inGrid(r,c)) return data[r][c]; else return NODATA_value;}
		int getNRows() {return nRows;}
		int getNCols() {return nCols;}
		int getxllCorner() {return xllCorner;}
		int getyllCorner() {return yllCorner;}
		int getCellSize() {return cellSize;}
		int getNODATA_value() {return NODATA_value;}
		int geth_min() {return h_min;}
		int geth_max() {return h_max;}

		int setData(int r, int c, int value);

		// I-O operations
		string writeToFile(string path);

	private:
		int nCols, nRows, xllCorner, yllCorner, cellSize, NODATA_value;
		float **data;
		float h_min, h_max;
		float multiplier;

		float computeFASinglePoint(int r, int c);

		string numberTokenize(const string &input);
		void printInfo();
		bool isInteger(string str);
		bool inGrid(int x, int y) const;
		void copyInfo(Grid &toGrid);
		int encodingDirection(int r, int c);

};