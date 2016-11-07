/*****************************************************************************
 File:   parviewshed.cpp
 Author: Son D. Ngo
 Date:   October 2016
 
 Description: For a given view point (x,y), compute its viewshed using OPENMP
 				and write to an output file
 
 Running instructions:
 Run this program with 4 arguments on the command line: 
	input file 
	output file (path) 
	x-coord of source  
	y-coord of source
 
 g++ -Wall -o viewshed viewshed.cpp grid.cpp
 ./parviewshed [grid-file-to-read] [file-to-write] x-coord y-coord num-of-threads
 
 Running on dover: 
 g++ -std=c++11 -Wall -o viewshed viewshed.cpp grid.cpp
 ./viewshed [grid-file-to-read] [file-to-write] x-coord y-coord num-of-threads
  
 ******************************************************************************/

#include "grid.h"

bool isInteger(string str);
void testFunction();

int main(int argc, char* argv[]) {

	//for testing purpose only
	if (argc == 3 && strcmp(argv[1], "test") == 0) {
		testFunction();
		return 0;
	}

	if (argc != 6) {
		cout << "Invalid number of arguments. Please make sure the command is the following format: " << endl;
		cout << "./viewshed [grid-file-to-read] [file-to-write] x-coord y-coord" << endl;
		exit(1);
	}

	string input = argv[1];
	string output = argv[2];
	string inputPath;
	string fileName;

	if (isInteger(argv[3]) == false || isInteger(argv[4]) == false || isInteger(argv[5]) == false) {
		cout << "Invalid argument for coordinates. Please make sure it's a valid integer" << endl;
		exit(1);
	}

	int vprow = stoi(argv[3]), vpcol = stoi(argv[4]), global_num_threads = stoi(argv[5]);

	if (input.find_last_of("/") != string::npos) {
		int pos = input.find_last_of("/");
		inputPath = input.substr(0, pos + 1);
		fileName = input.substr(input.find_last_of("/") + 1);
	} 
	else {
		fileName = input;
	}

	Grid grid;

	float start, end, sum = 0.0;

	start = omp_get_wtime();

	if (grid.readGridFromFile(input) == false) {
		cout << "Unable to read file. Exiting" << endl;
		exit(1);
	}

	end = omp_get_wtime();

	cout << "reading " << fileName << ": " << end - start << " seconds" << endl;

	Grid viewshedGrid(grid.getNRows(), grid.getNCols(), grid.getNODATA_value());

	grid.setNumThreads(global_num_threads);

	start = omp_get_wtime();

	grid.compute_viewshed(viewshedGrid, vprow, vpcol);

	end = omp_get_wtime();

	// cout << "computing viewshed with " << global_num_threads << " threads: " << (end - start) << endl;

	printf("computing viewshed with %d threads: %.8f seconds\n ", global_num_threads, end-start);

	// sum += (float)(end - start);

	// start = omp_get_wtime();

	// cout << "write to " << viewshedGrid.writeToFile(output);

	// end = omp_get_wtime();

	// cout << " in " << end - start << " seconds" << endl;

	// sum += end - start;

	// cout << "Total Running Time : " << sum << " seconds" << endl;

	//cleanup memory
	grid.freeGridData();
	viewshedGrid.freeGridData();

	cout << "========================================" << endl;

	return 0;

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

//function is written for testing purpose
void testFunction() {
	string path = "../testGrids/";
	string tests[] = {"brunsdem", "kaweah", "sierra", "portland_me", "usadem2", "eelriver", "washington"};
	// string tests[] = {"brunsdem", "kaweah"};

	Grid grid;
	grid.setNumThreads(DEFAULT_NUM_THREADS);

	for (unsigned int i = 0; i < sizeof(tests)/sizeof(string); i++) {
		if (!grid.readGridFromFile(path + tests[i] + ".asc")) {
			cout << "Unable to read " << path + tests[i] + ".asc" << endl;
			continue;
		}
		Grid viewshedGrid(grid.getNRows(), grid.getNCols(), grid.getNODATA_value());
		cout << "middle point: (" << grid.getNRows()/2 << "," << grid.getNCols()/2 << ")" << endl;
		float start, end;
		start = clock();
		grid.compute_viewshed(viewshedGrid, grid.getNRows()/2, grid.getNCols()/2);
		end = clock(); 
		cout << "time to compute viewshed " << tests[i] << ": " << (float)(end - start) / CLOCKS_PER_SEC << endl;
		cout << "write to " << viewshedGrid.writeToFile(path + tests[i] + "vis.asc") << endl;
		grid.freeGridData();
	}

	// grid.freeGridData();
}