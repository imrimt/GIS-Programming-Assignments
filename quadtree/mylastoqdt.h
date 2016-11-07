#ifndef MYLASTOQDT_H
#define MYLASTOQDT_H

/*****************************************************************************
 File:   mylastoqdt.h
 Author: Son D. Ngo
 Date:   November 2016
 
 Description: Header file for main
  
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

using namespace std;

class MyLAS {
	public:
		MyLAS();
		~MyLAS();

		bool readLiDARData(string file);
		vector<string> getHeader() { return header; }

	private:
		string numberTokenize(const string &input);

		// private variables
		vector<string> header;
		int headerSize, offSetToPointData, numPointsInLeaf;
};

#endif	//MYLASTOQDT_H