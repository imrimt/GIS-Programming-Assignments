/*****************************************************************************
 File:   mylastoqdt.cpp
 Author: Son D. Ngo
 Date:   November 2016
 
 Description: Cpp file for main
  
 ******************************************************************************/

#include "mylastoqdt.h"

bool isInteger(string str);

int main(int argc, char* argv[]) {

	if (argc != 3) {
		cout << "Invalid number of arguments. Please enter in the following format: " << endl
			 << "[path to executable] [LiDAR data] [number of points per leaf]" << endl;
		exit(1);
	}

	if (!isInteger(argv[2])) {
		cout << "Invalid argument for coordinates. Please make sure it's a valid integer" << endl;
		exit(1);
	}

	string input = argv[1];
	string numPoints = argv[2];
	string inputPath;
	string fileName;
	max_points_per_leaf = stoi(numPoints);

	if (input.find_last_of("/") != string::npos) {
		int pos = input.find_last_of("/");
		inputPath = input.substr(0, pos + 1);
		fileName = input.substr(input.find_last_of("/") + 1);
	} 
	else {
		fileName = input;
	}

	MyLAS mylas;

	if (!mylas.readLiDARData(input)) {
		cout << "Unable to read " << input << endl;
		exit(1);
	}

	mylas.buildQuadtree();

	return 0;
}

MyLAS::MyLAS() {
	// points = new (nothrow) point3D;
	tree = quadtree_init();
}

MyLAS::~MyLAS() {

}

void MyLAS::buildQuadtree() {
	tree = quadtree_build(points, numData, max_points_per_leaf);
}

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

	// cout << "headerSize = " << headerSize << endl;

	// obtain offset to point data
	getline(myFile, line);
	header.push_back(line);
	offSetToPointData = stoi(numberTokenize(line));

	// cout << "offSetToPointData = " << offSetToPointData << endl;

	// saving extra information, no need to parse
	for (int i = 0; i < 3; i++) {
		getline(myFile, line);
		header.push_back(line);
	}

	// obtain number of points records
	getline(myFile,line);
	header.push_back(line);
	numData = stoi(numberTokenize(line));

	// cout << "numData = " << numData << endl;

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
			if (!count) {
				break;
			}
		}

		points.push_back(point);
	}

	// cout << "size of points = " << points.size() << endl;

	return true;

}

//scan for number in a given string
string MyLAS::numberTokenize(const string &input) {
	return input.substr(input.find_first_of("-0123456789"));
}

//return true if a given string represents an integer, otherwise
//return false
bool isInteger(string str) {
	char start = str[0];
	if (isdigit(start) == false && start != '-') {
		return false;
	} 
	if (start == '0' && str.length() > 1) {
		return false;
	}
	for (unsigned int i = 1; i < str.length(); i++) {
		if (isdigit(str[i]) == false) return false;
	}

	return true;
}