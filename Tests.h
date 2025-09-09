#pragma once

#include "GraphAdjMatrix.h"
#include "GraphGeneration.h"
#include <random>

class Tests
{
public:
	static void remove_all_edges_conn_to_vcs_with_deg_test();
	static void graph_constructor_w_specific_edges_test();
	static void make_random_graph_test();
	static void dov_comparison_test();
};

