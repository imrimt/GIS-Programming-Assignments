#ifndef GRID_H
#define GRID_H

/* include files */
/*****************************************************************************
 File:   grid.h
 Author: Son D. Ngo
 Date:   September 2016
 
 Description: Header file for class Grid
  
 ******************************************************************************/

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstddef>
#include <time.h>
#include <vector>

const float NOFLOW_VALUE = -1;
const float INITIAL_ACCUMULATION = 0.0;

using namespace std;

class Grid {

	public:
		Grid();
		Grid(int nRows, int nCols, int NODATA_value);
		~Grid();

		void computeFD(Grid &FDgrid);
		void computeFA(Grid &FAgrid, const Grid &FDgrid);
		void computeFAslow(Grid &FAgrid, const Grid &FDgrid);

		float computeFAforPoint(int r, int c, const Grid &FDgrid, const Grid &FAgrid);
		float computeFAforPointSlow(int i, int j, const Grid &FDgrid);

		bool readGridFromFile(string gridFileName);

		void resetData();
		void printGrid();
		void printHeader();
		void printValues();
		void multiply(float multiplier);
		void freeGridData() {
			for (int i = 0; i < nRows; i++) {
				delete[] data[i];
			}
			delete[] data;
		}

		int valueAt(int r, int c) const {if (inGrid(r,c)) return data[r][c]; else return NODATA_value;}
		int getNRows() {return nRows;}
		int getNCols() {return nCols;}
		int getxllCorner() {return xllCorner;}
		int getyllCorner() {return yllCorner;}
		int getCellSize() {return cellSize;}
		int getNODATA_value() {return NODATA_value;}
		int geth_min() {return h_min;}
		int geth_max() {return h_max;}
		vector<string> getHeader() { return header; }

		int setData(int r, int c, int value);
		void setHeader(const vector<string> newHeader) { header = newHeader; }

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

		vector<string> header;

};

#endif	//GRID_H