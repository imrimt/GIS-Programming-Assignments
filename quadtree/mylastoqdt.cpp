/*****************************************************************************
 File:   mylastoqdt.cpp
 Author: Son D. Ngo
 Date:   November 2016
 
 Description: Cpp file for MyLAS class
  
 ******************************************************************************/

#include "mylastoqdt.h"

// Constructor
MyLAS::MyLAS() {
	tree = quadtree_init();
}

// Destructor
MyLAS::~MyLAS() {
	quadtree_free(tree);
}


// build quadtree given the number of points in leaf
void MyLAS::buildQuadtree(int maxPoints) {
	max_points_per_leaf = maxPoints;
	printf("max_points_per_leaf = %d\n", max_points_per_leaf);
	tree = quadtree_build(points, numData, max_points_per_leaf);

	printf("Size of tree: %d\n", tree->root->k);
}

// read in LiDAR data and store them into a vector of points
bool MyLAS::readLiDARData(string file) {

	string line;
	ifstream myFile;
	cout << "open file " << file << endl;
	myFile.open(file.c_str());

	// return false if file path is invalid or file is unopenable
	if (myFile.is_open() == false) {
		return false;
	}

	header.clear();

	// saving extra information, no need to parse
	for (int i = 0; i < 8; i++) {
		getline(myFile, line);
		header.push_back(line);
	}

	// obtain header size
	getline(myFile, line);
	header.push_back(line);
	headerSize = stoi(numberTokenize(line));

	// obtain offset to point data
	getline(myFile, line);
	header.push_back(line);
	offSetToPointData = stoi(numberTokenize(line));

	// saving extra information, no need to parse
	for (int i = 0; i < 3; i++) {
		getline(myFile, line);
		header.push_back(line);
	}

	// obtain number of points records
	getline(myFile,line);
	header.push_back(line);
	numData = stoi(numberTokenize(line));

	// saving the rest of the header, no need to parse
	for (int i = 0; i < 5; i++) {
		getline(myFile, line);
		header.push_back(line);
	}

	// parse data into array of point3D
	while (getline(myFile, line)) {
		point3D point;
		stringstream ss(line);

		string value = "";

		int count = 3;

		for (int i = 0; i < line.length(); i++) {
			if (line[i] == ',') {
				switch (count) {
					case 3: {
						point.x = atof(value.c_str());
						break;
					}
					case 2: {
						point.y = atof(value.c_str());
						break;
					}
					case 1: {
						point.z = atof(value.c_str());
						break;
					}
				}
				value = "";
				count--;
			}
			else {
				value += line[i];
			}
		}
		point.c = stoi(value);

		points.push_back(point);
	}

	return true;

}

//scan for number in a given string
string MyLAS::numberTokenize(const string &input) {
	return input.substr(input.find_first_of("-0123456789"));
}