#ifndef MYLASTOQDT_H
#define MYLASTOQDT_H

/*****************************************************************************
 File:   mylastoqdt.h
 Author: Son D. Ngo
 Date:   November 2016
 
 Description: Header file for MyLAS class
  
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
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include "quadtree.h"

using namespace std;

class MyLAS {
	public:
		MyLAS();
		~MyLAS();

		int getNumData() { return numData; }
		void buildQuadtree(int maxPoints);
		bool readLiDARData(string file);
		vector<string> getHeader() { return header; }
		quadtree* getQuadtree() { return tree; }
		vector<point3D>* getPoints() { return &points; } 

	private:
		string numberTokenize(const string &input);

		// private variables
		vector<string> header;
		int headerSize, offSetToPointData, numData;

		vector<point3D> points;

		quadtree *tree;
};

#endif	//MYLASTOQDT_H