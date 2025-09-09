#pragma once
#include "GraphAdjMatrix.h"
#include <fstream>
using std::ifstream;
using std::ofstream;
#include <chrono>
using namespace std::chrono;
#include <map>
using std::map;
#include <string>
using std::string;
#include <iostream>
using std::cout;
using std::endl;
#include <random>


class IsoTestingTracker;

class IsoTesting
{
public:
	// runs trials of different checks on two random graphs.
	static void rand_trials(int n, int trials);

	// true iff g and h are iso (only using permutation check)
	static bool brute_force(GraphAdjMatrix& g, GraphAdjMatrix& h);

	// true iff g and h have the same number of vertices
	static bool same_size(GraphAdjMatrix& g, GraphAdjMatrix& h);

	// true iff g and h have the same number of edges
	static bool same_num_of_edges(GraphAdjMatrix& g, GraphAdjMatrix& h);

	// true iff g and h have the same sorted degree of vertices
	static bool same_sorted_dov(GraphAdjMatrix& g, GraphAdjMatrix& h);

	// true iff g and h are both connected or both not connected
	static bool same_connectivity(GraphAdjMatrix& g, GraphAdjMatrix& h);

	// true iff g and h have the same sorted connected components
	static bool same_sorted_conn_comps(GraphAdjMatrix& g, GraphAdjMatrix& h);

	// true iff g and h have the same edges and sorted dovs after removing edges connected to vertices with deg d
	static bool same_edg_dov_after_del_deg(GraphAdjMatrix& g, GraphAdjMatrix& h, int del_deg);

	// true iff g and h have the same edges and sorted dovs after removing the median deg of the vcs
	// ASSUMES g and h have the same dov to begin with
	static bool same_edg_dov_after_del_median_deg(GraphAdjMatrix& g, GraphAdjMatrix& h);

	// true iff g and h are iso (vcs() edges() deg_of_vcs(), then brute_force())
	static bool common_sense_checks(GraphAdjMatrix& g, GraphAdjMatrix& h);

	// true iff g and h are iso (common sense, connected() conn_comp_sorted(), those checks on the inverse too, then brute_force())
	static bool conn_checks_tracked(GraphAdjMatrix& g, GraphAdjMatrix& h, IsoTestingTracker& itt);
	static bool conn_checks(GraphAdjMatrix& g, GraphAdjMatrix& h);

	// true iff g and h are iso (common sense, remove all edges connected to vcs with certain degree, then brute_force())
	static bool del_deg_checks_tracked(GraphAdjMatrix& g, GraphAdjMatrix& h, IsoTestingTracker& itt);
	static bool del_deg_checks(GraphAdjMatrix& g, GraphAdjMatrix& h);
};

class IsoTestingTracker
{
public:
	// default constructor
	IsoTestingTracker();

	// track a check (increments the count of the string)
	void check_count(string s, int count = 1);

	// start timer
	void start_timer();

	// end timer and add it to the time stored under label s
	void stop_timer_and_log(string s);

	// print checks and timer
	void print_checks_and_timer();

	// track an exit case
	void exit_case_count(string s, int count = 1);

	// print exit cases
	void print_exit_cases();

private:
	map<string, int> check_counter;
	map<string, int> exit_case_counter;
	map<string, duration<double>> check_timer;
	steady_clock::time_point start;

};



