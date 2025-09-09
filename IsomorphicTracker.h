#pragma once

#include "GraphAdjMatrix.h"

#include <fstream>
using std::ifstream;
using std::ofstream;
#include <string>
using std::to_string;
using std::string;
#include <chrono>
using namespace std::chrono;
#include <cmath>
#include <vector>
using std::vector;
#include <set>
using std::set;
#include <iostream>
using std::cout;
using std::endl;


// An old chunk of code where I had a data type that stored all the graphs of a given size
//   and kept the ones that were isomorphic in the same basket.  This quickly became the limiting
//   factor and required too much memory, so I switched to only storing one representative graph
//   of each set of isomorphic graphs.
class IsomorphicTracker
{
public:
	// constructor that pads out the vector to a certain node count
	IsomorphicTracker(int max_nodes);

	// returns the largest node count that we can call without vector out of range
	int largest_node_count();

	// reads into this from the stored files
	void retrieve(int lo, int hi = -1);

	// generates all sets of isomorphic graphs for a given node count
	//  doesn't bother worrying about graphs with an isolated node or full node (inv has iso node)
	void generate(int node_count);

	// generates up to n
	void generate_up_to(int n);

	// returns a vector of the first graph in each isomorphic group for a given number of nodes
	vector<GraphAdjMatrix> representatives(int n);

	// file output
	void file_output(int nodes);

	// print
	void print();
private:
	vector<vector<set<int>>> _it;
	// it[n] gives a vector of sets of isomorphic graphs with n nodes.
};

