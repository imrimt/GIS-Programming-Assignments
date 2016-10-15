#ifndef GRID_H
#define GRID_H

/*****************************************************************************
 File:   grid.h
 Author: Son D. Ngo
 Date:   September 2016
 
 Description: Header file for class Grid
  
 ******************************************************************************/

/* include files */
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <string.h>
#include <cstddef>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <cmath>

const float INITIAL_VIEW = -1.0;
const float INITIAL_ANGLE = -10.0;
const float INVALID_VALUE = -5.0;

using namespace std;

class Grid {

	public:
		Grid();
		Grid(int nRows, int nCols, int NODATA_value);
		~Grid();

		void compute_viewshed(Grid &viewshedGrid, int vprow, int vpcol);

		bool readGridFromFile(string gridFileName);

		void printGrid();
		void printHeader();
		void printValues();

		void freeGridData() {
			for (int i = 0; i < nRows; i++) {
				delete[] data[i];
			}
			delete[] data;
		}

		float valueAt(int r, int c) const {if (inGrid(r,c)) return data[r][c]; else return NODATA_value;}
		int getNRows() {return nRows;}
		int getNCols() {return nCols;}
		int getxllCorner() {return xllCorner;}
		int getyllCorner() {return yllCorner;}
		int getCellSize() {return cellSize;}
		int getNODATA_value() {return NODATA_value;}
		int geth_min() {return h_min;}
		int geth_max() {return h_max;}
		vector<string> getHeader() { return header; }

		void setAllData(float value);
		float setData(int r, int c, float value);
		void setHeader(const vector<string> newHeader) { header = newHeader; }

		// I-O operations
		string writeToFile(string path);

	private:
		int nCols, nRows, xllCorner, yllCorner, cellSize, NODATA_value;
		float **data;
		float h_min, h_max;
		float multiplier;

		float rowInterpolate(int row, float y);
		float columnInterpolate(int col, float x);
		// float distance(float vprow, float vpcol, float row, float col);
		// float verticalAngle(float heightDistance, float distance);
		float horizontalVisible(int vprow, int vpcol, int row, int col, float vpheight, float angle);
		float verticalVisible(int vprow, int vpcol, int row, int col, float vpheight, float angle);

		string numberTokenize(const string &input);
		void printInfo();
		float isVisible(int vprow, int vpcol, int row, int col);
		bool isInteger(string str);
		bool inGrid(int x, int y) const;
		int encodingDirection(int r, int c);

		vector<string> header;

};

#endif	//GRID_H