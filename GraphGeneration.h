#pragma once

#include "GraphAdjMatrix.h"
#include "IsoTesting.h"

#include <fstream>
using std::ifstream;
using std::ofstream;
#include <vector>
using std::vector;
#include <chrono>
using namespace std::chrono;
#include <string>
using std::to_string;
using std::string;
#include <iostream>
using std::cout;
using std::endl;
#include <utility>
using std::pair;
#include <algorithm>

class GraphGeneration
{
public:
	// generates one version of all non-iso graphs with n vertices, then outputs in a file
	static void generate_reps_old(int vertices);
	static void generate_reps_bin_search(int vertices);

	// outputs a vector of graphs to file_name
	static void reps_file_output(string file_name, vector<vector<GraphAdjMatrix>> reps);

	// comparison functions for deg_of_vcs vector<int>s
	static bool dov_less_than(vector<int> dov1, vector<int> dov2);
	static bool dov_greater_than(vector<int> dov1, vector<int> dov2);
	static bool dov_equal_to(vector<int> dov1, vector<int> dov2);
	// print the deg_of_vcs of a given graph
	static void print_dov_of(GraphAdjMatrix& g);

};

