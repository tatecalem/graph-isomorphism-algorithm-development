#include "IsoTesting.h"



// runs trials of different checks on two random graphs.
void IsoTesting::rand_trials(int n, int trials)
{
	// Find the total number of unique graphs of size n
	int total_graphs = static_cast<int>(std::pow(2, (n * (n - 1)) / 2));
	cout << "total graphs: " << total_graphs << endl;

	// Initialize random number generator
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> range(0, total_graphs-1); // inclusive

	// Initialize IsomorphicTestingTracker
	IsoTestingTracker itt;
	// ...and the count for how many trials the check says "yep, not iso."
	int not_iso_count = 0;

	for (int t = 1; t <= trials; t++)
	{
		//cout << "Trial " << t << endl;
		
		if (t % (trials / 100) == 0)
		{
			cout << "Trial " << t << endl;
		}
		
		// generate two random graphs
		//GraphAdjMatrix g(n, range(gen));
		//GraphAdjMatrix h(n, range(gen)); // if you just wanted any two graphs, uncomment this
		//GraphAdjMatrix h(n, g.edges(), gen); // if you wanted a graph with the same # edges as g, uncomment this
		bool dif_dov = true; // find a graph that also has the same deg_of_vcs
		GraphAdjMatrix g(n,0);
		GraphAdjMatrix h(n,0);
		while (dif_dov)
		{
			GraphAdjMatrix g2(n, range(gen)); // generate a random graph
			vector<int> g2_dov = g2.deg_of_vcs();
			//cout << "  possible g" << endl;
			//g2.print();
			GraphAdjMatrix h2(n, g2.edges(), gen); // generate another graph with the same size
			vector<int> h2_dov = h2.deg_of_vcs();
			//cout << "  possible h" << endl;
			//h2.print();
			dif_dov = false;
			for (int i = 0; i < g2_dov.size(); i++)
			{
				if (g2_dov[i] != h2_dov[i])
				{
					dif_dov = true;
					break;
				}
			}
			if (dif_dov)
			{
				//cout << " reroll. they have different dovs" << endl;
			}
			else {
				//cout << " confirmed: same dov.  Now, copying graphs out to g and h" << endl;
				g = g2;
				h = h2;
			}
		}
		
		// run a check
		itt.start_timer();

		
		//bool could_be_iso = IsoTesting::same_num_of_edges(g,h); // edges check
		//bool could_be_iso = IsoTesting::same_sorted_dov(g, h); // sorted dov check
		//bool could_be_iso = IsoTesting::same_connectivity(g, h); // connectivity check
		//bool could_be_iso = IsoTesting::same_sorted_conn_comps(g, h); // conn comp check
		//bool could_be_iso = IsoTesting::same_edg_dov_after_del_deg(g, h, 3); // del deg, then edges and dov check
		bool could_be_iso = IsoTesting::same_edg_dov_after_del_median_deg(g, h); // del median deg, then edges and dov check

		itt.stop_timer_and_log(string("check1"));
		itt.check_count(string("check1"));
		if (!could_be_iso)
		{
			not_iso_count++;
		}
	}
	itt.print_checks_and_timer();
	cout << "not iso count: " << not_iso_count << endl;
}


// true iff g and h are iso (only using permutation check)
bool IsoTesting::brute_force(GraphAdjMatrix& g, GraphAdjMatrix& h)
{
	//cout << "IsoTesting brute_force()" << endl;

	if (!same_size(g, h))
		return false;

	// check every permutation for a match
	Permutation p(g.vertices());
	do
	{
		bool this_matches = true;
		//p.print();

		// see if the graphs match under this permutation
		for (int i = 0; i < g.vertices(); i++)
		{
			for (int j = 0; j < g.vertices(); j++)
			{
				//cout << "looking at i:" << i << " j:" << j << endl;
				if (i != j && (g.edge_between(i, j) != h.edge_between(p[i], p[j])))
				{
					//cout << " not a match" << endl;
					this_matches = false;
					break;
				}
			}
			if (!this_matches)
			{
				break;
			}
		}
		if (this_matches) // found a match
		{
			//cout << "a match!" << endl;
			return true;
		}
	} while (p.next());

	return false;
}


// true iff g and h have the same number of vertices
bool IsoTesting::same_size(GraphAdjMatrix& g, GraphAdjMatrix& h)
{
	//cout << "IsoTesting same_size()" << endl;
	if (g.vertices() != h.vertices())
	{
		cout << " not iso: different number of nodes" << endl;
		return false;
	}
	else {
		return true;
	}
}


// true iff g and h have the same number of edges
bool IsoTesting::same_num_of_edges(GraphAdjMatrix& g, GraphAdjMatrix& h)
{
	return g.edges() == h.edges();
}


// true iff g and h have the same sorted degree of vertices
bool IsoTesting::same_sorted_dov(GraphAdjMatrix& g, GraphAdjMatrix& h)
{
	vector<int> g_dov = g.deg_of_vcs();
	vector<int> h_dov = h.deg_of_vcs();
	for (int i = 0; i < g_dov.size(); i++)
	{
		if (g_dov[i] != h_dov[i])
			return false;
	}
	return true;
}


// true iff g and h are both connected or both not connected
bool IsoTesting::same_connectivity(GraphAdjMatrix& g, GraphAdjMatrix& h)
{
	return g.connected() == h.connected();
}


// true iff g and h have the same sorted connected components
bool IsoTesting::same_sorted_conn_comps(GraphAdjMatrix& g, GraphAdjMatrix& h)
{
	vector<int> g_conn_comp_sizes = g.sorted_conn_comp_sizes();
	vector<int> h_conn_comp_sizes = h.sorted_conn_comp_sizes();
	for (int i = 0; i < g_conn_comp_sizes.size(); i++)
	{
		if (g_conn_comp_sizes[i] != h_conn_comp_sizes[i])
			return false;
	}
	return true;
}


// true iff g and h have the same edges and sorted dovs after removing edges connected to vcs with deg del_deg
bool IsoTesting::same_edg_dov_after_del_deg(GraphAdjMatrix& g, GraphAdjMatrix& h, int del_deg)
{
	// removing the edges connected to vcs with deg del_deg
	GraphAdjMatrix g_del(g);
	g_del.remove_all_edges_conn_to_vcs_with_deg(del_deg);
	//cout << "Before removing edges" << endl;
	//g.print();
	//cout << "After removing edges" << endl;
	//g_del.print();
	GraphAdjMatrix h_del(h);
	h_del.remove_all_edges_conn_to_vcs_with_deg(del_deg);
	//cout << "Before removing edges" << endl;
	//h.print();
	//cout << "After removing edges" << endl;
	//h_del.print();

	bool not_iso = IsoTesting::same_num_of_edges(g_del,h_del); // edges check inside del check
	if (not_iso == false) // if we still don't know they're not iso, dov check
	{
		//cout << "they still have the same number of edges" << endl;
		not_iso = IsoTesting::same_sorted_dov(g_del, h_del); // dov check inside del check
		if (not_iso == false)
		{
			//cout << "they still have the same dov too" << endl;
		}
	}
	return not_iso;
}


// true iff g and h have the same edges and sorted dovs after removing the median deg of the vcs
// ASSUMES g and h have the same dov to begin with
bool IsoTesting::same_edg_dov_after_del_median_deg(GraphAdjMatrix& g, GraphAdjMatrix& h)
{
	// find the median degree
	vector<int> deg_vec = g.deg_of_vcs();
	int del_deg = deg_vec[deg_vec.size() / 2];
	//cout << "cutting edges connected to vertices with degree " << del_deg << endl;

	// removing the edges connected to vcs with deg del_deg
	GraphAdjMatrix g_del(g);
	g_del.remove_all_edges_conn_to_vcs_with_deg(del_deg);
	//cout << "Before removing edges" << endl;
	//g.print();
	//cout << "After removing edges" << endl;
	//g_del.print();
	GraphAdjMatrix h_del(h);
	h_del.remove_all_edges_conn_to_vcs_with_deg(del_deg);
	//cout << "Before removing edges" << endl;
	//h.print();
	//cout << "After removing edges" << endl;
	//h_del.print();

	bool could_be_iso = IsoTesting::same_num_of_edges(g_del, h_del); // edges check inside del check
	if (could_be_iso) // if we still don't know they're not iso, dov check
	{
		//cout << "they still have the same number of edges" << endl;
		could_be_iso = IsoTesting::same_sorted_dov(g_del, h_del); // dov check inside del check
		if (could_be_iso)
		{
			//cout << "they still have the same dov too" << endl;
		}
	}
	return could_be_iso;
}


// true iff g and h are iso (vcs() edges() deg_of_vcs(), then brute_force())
bool IsoTesting::common_sense_checks(GraphAdjMatrix& g, GraphAdjMatrix& h)
{
	//cout << "IsoTesting common_sense_checks()" << endl;

	if (!same_size(g, h))
		return false;

	if (!same_num_of_edges(g,h))
		return false;

	if (!same_sorted_dov(g, h))
		return false;

	return brute_force(g, h);
}


// true iff g and h are iso (common sense, connected() conn_comp_sorted(), those checks on the inverse too, then brute_force())
bool IsoTesting::conn_checks_tracked(GraphAdjMatrix& g, GraphAdjMatrix& h, IsoTestingTracker& itt)
{
	//cout << "IsoTesting conn_checks_tracked()" << endl;
	bool exit;

	// vertices check
	//cout << " vertices check" << endl;
	itt.start_timer();
	exit = !same_size(g, h);
	itt.stop_timer_and_log(string("vertex count"));
	itt.check_count(string("vertex count"));
	if (exit)
	{
		itt.exit_case_count(string("vertex count"));
		return false;
	}

	// edges check
	//cout << " edges check" << endl;
	itt.start_timer();
	exit = (g.edges() != h.edges());
	itt.stop_timer_and_log(string("edge count"));
	itt.check_count(string("edge count"));
	if (exit)
	{
		itt.exit_case_count(string("edge count"));
		return false;
	}

	// deg_of_vcs check
	//cout << " sorted deg_of_vcs check" << endl;
	itt.start_timer();
	exit = false;
	vector<int> g_dov = g.deg_of_vcs();
	vector<int> h_dov = h.deg_of_vcs();
	for (int i = 0; i < g_dov.size(); i++)
	{
		if (g_dov[i] != h_dov[i])
		{
			exit = true;
			break;
		}
	}
	itt.stop_timer_and_log(string("sorted deg_of_vcs"));
	itt.check_count(string("sorted deg_of_vcs"));
	if (exit)
	{		
		itt.exit_case_count(string("sorted deg_of_vcs"));
		return false;
	}

	// connected check
	//cout << " connectivity check" << endl;
	itt.start_timer();
	exit = (g.connected() != h.connected());
	itt.stop_timer_and_log(string("connected"));
	itt.check_count(string("connected"));
	if (exit)
	{
		itt.exit_case_count(string("connected"));
		return false;
	}

	// conn_comp sizes
	//cout << " connected components sizes check" << endl;
	itt.start_timer();
	exit = false;
	vector<int> g_conn_comp_sizes = g.sorted_conn_comp_sizes();
	vector<int> h_conn_comp_sizes = h.sorted_conn_comp_sizes();
	for (int i = 0; i < g_conn_comp_sizes.size(); i++)
	{
		if (g_conn_comp_sizes[i] != h_conn_comp_sizes[i])
		{
			exit = true;
			break;
		}
	}
	itt.stop_timer_and_log(string("sorted connected component sizes"));
	itt.check_count(string("sorted connected component sizes"));
	if (exit)
	{
		itt.exit_case_count(string("sorted connected component sizes"));
		return false;
	}

	//cout << "making inverses" << endl;
	GraphAdjMatrix g_inv = g.inverse();
	GraphAdjMatrix h_inv = h.inverse();
	//cout << "done making inverses" << endl;

	// inv connected
	//cout << " inverse connectivity check" << endl;
	itt.start_timer();
	exit = (g_inv.connected() != h_inv.connected());
	itt.stop_timer_and_log(string("inverse connected"));
	itt.check_count(string("inverse connected"));
	if (exit)
	{
		itt.exit_case_count(string("inverse connected"));
		return false;
	}

	// inv conn_comp sizes
	//cout << " inverse connected components sizes check" << endl;
	itt.start_timer();
	exit = false;
	vector<int> g_inv_conn_comp_sizes = g_inv.sorted_conn_comp_sizes();
	vector<int> h_inv_conn_comp_sizes = h_inv.sorted_conn_comp_sizes();
	for (int i = 0; i < g_inv_conn_comp_sizes.size(); i++)
	{
		if (g_inv_conn_comp_sizes[i] != h_inv_conn_comp_sizes[i])
		{
			exit = true;
			break;
		}
	}
	itt.stop_timer_and_log(string("inverse sorted connected component sizes"));
	itt.check_count(string("inverse sorted connected component sizes"));
	if (exit)
	{
		itt.exit_case_count(string("inverse sorted connected component sizes"));
		return false;
	}

	// brute force
	itt.exit_case_count(string("brute force"));
	itt.check_count(string("brute force"));
	itt.start_timer();
	bool bf_iso = brute_force(g, h);
	itt.stop_timer_and_log(string("brute force"));
	return bf_iso;
}
bool IsoTesting::conn_checks(GraphAdjMatrix& g, GraphAdjMatrix& h)
{
	IsoTestingTracker itt;
	return conn_checks_tracked(g, h, itt);
}


// true iff g and h are iso (common sense, remove all edges connected to vcs with certain degree, then brute_force())
bool IsoTesting::del_deg_checks_tracked(GraphAdjMatrix& g, GraphAdjMatrix& h, IsoTestingTracker& itt)
{
	//cout << "IsoTesting del_deg_checks_tracked with " << g.to_int() << endl;
	//g.print();
	//cout << " and " << h.to_int() << endl;
	//h.print();
	bool could_be_iso;
	
	// vertices check
	//cout << " vertices check" << endl;
	itt.start_timer();
	could_be_iso = IsoTesting::same_size(g, h);
	itt.stop_timer_and_log(string("vertex count"));
	itt.check_count(string("vertex count"));
	if (!could_be_iso)
	{
		itt.exit_case_count(string("vertex count"));
		//cout << "false bc of vertex count" << endl;
		return false;
	}

	// edges check
	//cout << " edges check" << endl;
	itt.start_timer();
	could_be_iso = IsoTesting::same_num_of_edges(g, h);
	itt.stop_timer_and_log(string("edge count"));
	itt.check_count(string("edge count"));
	if (!could_be_iso)
	{
		itt.exit_case_count(string("edge count"));
		//cout << "false bc of edge count" << endl;
		return false;
	}

	// deg_of_vcs check
	//cout << " sorted deg_of_vcs check" << endl;
	itt.start_timer();
	could_be_iso = IsoTesting::same_sorted_dov(g, h);
	itt.stop_timer_and_log(string("sorted deg_of_vcs"));
	itt.check_count(string("sorted deg_of_vcs"));
	if (!could_be_iso)
	{
		itt.exit_case_count(string("sorted deg_of_vcs"));
		//cout << "false bc of sorted dov count" << endl;
		return false;
	}
	
	// remove edges connected to vertices with certain degrees of vertices
	//cout << " remove median deg edges check" << endl;
	itt.start_timer();
	// remove edges with endpoints with median degree
	GraphAdjMatrix g_del(g);
	GraphAdjMatrix h_del(h);
	g_del.remove_all_edges_conn_to_vcs_with_median_deg();
	h_del.remove_all_edges_conn_to_vcs_with_median_deg();
	//cout << " edge check w/in del deg check" << endl;
	could_be_iso = IsoTesting::same_num_of_edges(g, h); // then an edge check
	if (could_be_iso)
	{
		//cout << " sorted dov check w/in del deg check" << endl;
		could_be_iso = IsoTesting::same_sorted_dov(g, h); // and a sorted dov check
	}
	itt.stop_timer_and_log(string("del deg"));
	itt.check_count(string("del deg"));
	if (!could_be_iso)
	{
		itt.exit_case_count(string("del deg"));
		//cout << "false bc of del deg count" << endl;
		return false;
	}

	// brute force
	//cout << " no checks distinguished, so brute force" << endl;
	itt.exit_case_count(string("brute force"));
	itt.check_count(string("brute force"));
	itt.start_timer();
	bool bf_iso = IsoTesting::brute_force(g, h);
	//cout << "graph " << g.to_int() << " did brute force" << endl;
	itt.stop_timer_and_log(string("brute force"));
	return bf_iso;
}
bool IsoTesting::del_deg_checks(GraphAdjMatrix& g, GraphAdjMatrix& h)
{
	IsoTestingTracker itt;
	return del_deg_checks_tracked(g, h, itt);
}


// default constructor
IsoTestingTracker::IsoTestingTracker()
{

}


// track a check (increments the count of the string)
void IsoTestingTracker::check_count(string s, int count)
{
	check_counter[s] += count;
}


// start timer
void IsoTestingTracker::start_timer()
{
	start = high_resolution_clock::now();
}


// end timer and add it to the time stored under label s
void IsoTestingTracker::stop_timer_and_log(string s)
{
	steady_clock::time_point end = high_resolution_clock::now();
	microseconds mic_s = duration_cast<microseconds>(duration<double>(end - start));
	check_timer[s] += mic_s;
}


// print checks and timer
void IsoTestingTracker::print_checks_and_timer()
{
	cout << "printing checks" << endl;
	for (auto& p : check_counter)
	{
		cout << " " << p.second << " checks for " << p.first << endl;
	}
	cout << "printing timers" << endl;
	for (auto& p : check_timer)
	{
		cout << " " << p.second.count() << "sec for " << p.first << endl;
	}
}


// track an exit case
void IsoTestingTracker::exit_case_count(string s, int count)
{
	//cout << "adding exit case " << s << "+" << count << endl;
	exit_case_counter[s] += count;
}


// print exit cases
void IsoTestingTracker::print_exit_cases()
{
	cout << "exit case counter" << endl;
	for (auto& p : exit_case_counter)
	{
		cout << " " << p.first << " " << p.second << endl;
	}
}


