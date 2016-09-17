/* include files */
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstddef>

using namespace std;

class Grid {

	public:
		Grid();
		~Grid();

		Grid computeFD();
		Grid computeFA();

		bool readGridFromFile(string gridFileName);

		void printGrid();
		void printHeader();
		void printValues();
		void multiply(float multiplier);

		int getNRows() {return nRows;}
		int getNCols() {return nCols;}
		int getxllCorner() {return xllCorner;}
		int getyllCorner() {return yllCorner;}
		int getCellSize() {return cellSize;}
		int getNODATA_value() {return NODATA_value;}
		int geth_min() {return h_min;}
		int geth_max() {return h_max;}

		int setData(int r, int c, int value);

	private:
		int nCols, nRows, xllCorner, yllCorner, cellSize, NODATA_value;
		float **data;
		float h_min, h_max;
		float multiplier;

		string numberTokenize(const string &input);
		void printInfo();
		bool isInteger(string str);
		void copyInfo(Grid &toGrid);

};