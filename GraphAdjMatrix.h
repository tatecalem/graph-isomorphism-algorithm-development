#pragma once

#include "Permutation.h"

#include <algorithm> // for sort
#include <functional> // for greater
#include <vector>
using std::vector;
#include <iostream>
using std::cout;
using std::endl;
#include <random>

class GraphAdjMatrix
{
public:
	// default constructor
	GraphAdjMatrix();

	// constructor that specifies a size and a graph by an integer code (0=empty)
	GraphAdjMatrix(int size, int binary_code=0);

	// constructor that makes a graph with a specific number of edges
	GraphAdjMatrix(int size, int edges, std::mt19937 gen);

	// turns this graph into the inputted graph
	GraphAdjMatrix& operator =(GraphAdjMatrix g); // pass by value so insert works

	// returns _g
	vector<vector<bool>> get_g();

	// returns the number of vertices in the graph
	int vertices();

	// returns the number of edges in the graph
	const int edges();

	// verifies and corrects the number of edges (instead of trusting the in-the-moment adjustments)
	bool recalc_edges();

	// returns a vector<int> representing the degree of each vertex sorted lo to hi
	vector<int> deg_of_vcs();

	// true iff a is in the graph
	bool is_vx(int a);

	// true iff a and b are in the graph
	bool are_vcs(int a, int b);

	// get the element at _g[i][j]
	bool get(int i, int j);

	// set the element at _g[i][j] (with option to recalculate edges after)
	void set(int i, int j, bool b, bool recalc_e = true);

	// converts the graph to its integer code (using binary)
	int to_int();

	// true if there is an edge, false if not
	bool edge_between(int a, int b);

	// add node
	int add_vx();

	// add edge (multi-edges not allowed)
	void add_edge(int a, int b);

	// returns next graph (binary counting up on the top right)
	bool next();

	// returns the inverse of the graph (all non-edges become edges, vice versa)
	GraphAdjMatrix inverse();

	// remove all edges connected to vertices with degree n
	void remove_all_edges_conn_to_vcs_with_deg(int n);

	// remove all edges connected to vertices with the median degree in this graph's dov
	void remove_all_edges_conn_to_vcs_with_median_deg();

	// returns true iff connected
	bool connected();
	void connected_rec(int at_node, vector<bool>& visited);

	// returns a vector that assigns each node a label identifying its connected component
	vector<int> conn_comp();
	void conn_comp_rec(int at_node, vector<int>& vxc, int current_comp);
	// returns the size of each connected component (sorted) for comparison
	vector<int> sorted_conn_comp_sizes();

	// true iff self is iso to G
	// this is a rudimentary implementation. More thorough ideas are in IsoTesting.cpp
	bool is_iso_to(GraphAdjMatrix G);

	// print
	void print();

	// print just the top right of the adjacency matrix (above the diagonal)
	void print_tr();

private:
	// implemented as vector<vector<bool>> .  _g[A][B] is true if there is an edge between node A and B, false otherwise.
	vector<vector<bool>> _g;
	int _edges;
};

