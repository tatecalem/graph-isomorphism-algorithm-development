#include "GraphGeneration.h"

// generates one version of all non-iso graphs with n vertices, then outputs in a file
// reads num_of_edges out of the graph and uses that to skip other graphs
void GraphGeneration::generate_reps_old(int vertices)
{
	long iso_test_count = 0;

	vector<vector<GraphAdjMatrix>> reps; // reps organized in a list by number of edges
	int total_edges = (vertices - 1) * vertices / 2;
	for (int i = 0; i <= total_edges; i++)
	{
		reps.push_back(vector<GraphAdjMatrix>()); // pad it out so it's ready to input
	}
	IsoTestingTracker itt;

	// set up progress tracking
	int total_graphs = static_cast<int>(std::pow(2, ((vertices - 1) * vertices) / 2));
	cout << " total graphs of size " << vertices << ": " << total_graphs << endl;
	int count = 0;
	int divisions = 10;
	int chunk = total_graphs / divisions;
	int next_printout = chunk;
	int chunk_count = 1;
	// set up timer
	steady_clock::time_point start = high_resolution_clock::now();

	GraphAdjMatrix g(vertices);
	do // for each graph, iterated through with .next()
	{
		//cout << "Checking the following graph against all reps so far" << endl;
		//g.print();
		// print status update
		if (true && count == next_printout) // adjust T/F to print/not print
		{
			// calculate time
			steady_clock::time_point end = high_resolution_clock::now();
			milliseconds ms = duration_cast<milliseconds>(duration<double>(end - start));
			start = end; // set start to now to time the next chunk

			// print update
			cout << " " << 100 * chunk_count * 1.0 / divisions << "% " << ms.count() << " ms" << endl;
			chunk_count++;
			next_printout += chunk;
		}
		bool iso_to_existing_set = false;

		//cout << " " << reps[g.edges()].size() << " reps @ this edge count" << endl;
		for (int i = 0; i < reps[g.edges()].size(); i++) // for each rep
		{
			//cout << " checking if G is iso to this rep" << endl;
			//reps[g.edges()][i].print();
			iso_test_count++;
			if (IsoTesting::del_deg_checks_tracked(g, reps[g.edges()][i], itt))
			{
				//cout << " iso to an existing set, so go next" << endl;
				iso_to_existing_set = true;
				break;
			}
		}
		if (!iso_to_existing_set) // add to the reps
		{
			//cout << " not iso to an existing rep, so add to reps" << endl;
			reps[g.edges()].push_back(g);
			//cout << " " << reps[g.edges()].size() << " reps @ this edge count" << endl;
		}
		count++;
	} while (g.next());

	reps_file_output("repstest", reps); // "reps"+to_string(vertices)
	cout << "graphs looked at: " << count << endl;
	itt.print_exit_cases();
	itt.print_checks_and_timer();
	cout << "isomorphic checks done: " << iso_test_count << endl;
}


// generates one version of all non-iso graphs with n vertices, then outputs in a file
// reads num_of_edges out of the graph and uses that to skip other graphs
// also binary searches with the deg_of_vcs
void GraphGeneration::generate_reps_bin_search(int vertices)
{
	cout << "generating all non-iso representatives with " << vertices << " vertices." << endl;
	long iso_test_count = 0;

	vector<vector<GraphAdjMatrix>> reps; // reps organized in a list by number of edges
	int total_edges = (vertices - 1) * vertices / 2;
	for (int i = 0; i <= total_edges; i++)
	{
		reps.push_back(vector<GraphAdjMatrix>()); // pad it out so it's ready to input
	}
	IsoTestingTracker itt;

	// set up progress tracking
	int total_graphs = static_cast<int>(std::pow(2, ((vertices - 1) * vertices) / 2));
	cout << " total graphs of size " << vertices << ": " << total_graphs << endl;
	int count = 0;
	int divisions = 10;
	int chunk = total_graphs / divisions;
	int next_printout = chunk;
	int chunk_count = 1;
	// set up timer
	steady_clock::time_point start = high_resolution_clock::now();

	GraphAdjMatrix g(vertices);
	do // for each graph, iterated through with .next()
	{
		//cout << "Checking " << g.to_int() << " against all in reps[" << g.edges() << "]" << endl;
		//g.print();
		// print status update
		if (true && count == next_printout) // adjust T/F to print/not print
		{
			// calculate time
			steady_clock::time_point end = high_resolution_clock::now();
			milliseconds ms = duration_cast<milliseconds>(duration<double>(end - start));
			start = end; // set start to now to time the next chunk

			// print update
			cout << " " << 100 * chunk_count*1.0 / divisions << "% " << ms.count() << " ms" << endl;
			chunk_count++;
			next_printout += chunk;
		}
		// START NEW BLOCK

		int edges = g.edges();
		vector<int> g_dov = g.deg_of_vcs();
		//cout << " g dov: "; GraphGeneration::print_dov_of(g);
		int lo = 0; // can't be unsigned because if the search is below the min_val this will hold -1
		int hi = static_cast<int>(reps[edges].size() - 1);
		int mid = (lo + hi) / 2; // assigned out here so it wouldn't be unassigned if gen_and_fine has 0 size, avoiding the while loop
		//cout << "  lo:" << lo << " mid:" << mid << " hi:" << hi << " start binary search" << endl;
		while (lo <= hi)
		{
			mid = lo + (hi - lo) / 2; // avoids integer overflow
			//cout << "   lo:" << lo << " mid:" << mid << " hi:" << hi << endl;
			GraphAdjMatrix mid_graph = reps[edges][mid];
			//cout << "  mid dov: "; GraphGeneration::print_dov_of(mid_graph);
			if (GraphGeneration::dov_less_than(g_dov,mid_graph.deg_of_vcs()))
			{
				//cout << "   search dov < current mid dov" << endl;
				hi = mid - 1;
				//cout << "    changed hi to " << hi << endl;
			}
			else if (GraphGeneration::dov_greater_than(g_dov, mid_graph.deg_of_vcs()))
			{
				//cout << "   search dov > current mid dov" << endl;
				lo = mid + 1;
				//cout << "    changed lo to " << lo << endl;
			}
			else { // GraphGeneration::dov_equal_to(g.deg_of_vcs(),mid_graph.deg_of_vcs())
				//cout << "   search dov == current mid dov" << endl;
				break; // if we ever find the right number, break.  We use mid as our next startpoint.
			}
		}
		//cout << "  done with binary search" << endl;

		bool dov_match = (lo <= hi); // true iff the dovs match
		bool guaranteed_iso = false; // true iff the search is truly iso to a current rep

		int min_i_of_same = mid;
		int max_i_of_same = mid;
		if (dov_match) // scan outwards to find the boundaries of the same dov section
		{
			//   I think that the lo and hi and mid can help narrow that search somewhat,
			//      but for now I'll just go out in both directions.
			//   Technically you could binary search this too but it's not worth it I think,
			//   especially because it'd just be with deg_of_vcs comparisons not actual iso tests
			vector<int> mid_dov = reps[edges][mid].deg_of_vcs();
			while (min_i_of_same > 0 &&
				GraphGeneration::dov_equal_to(mid_dov, reps[edges][min_i_of_same - 1].deg_of_vcs()))
			{
				min_i_of_same--;
			}
			while (max_i_of_same < reps[edges].size() - 1 &&
				GraphGeneration::dov_equal_to(mid_dov, reps[edges][max_i_of_same + 1].deg_of_vcs()))
			{
				max_i_of_same++;
			}
			//cout << "  min_i_of_same:" << min_i_of_same << " max_i_of_same:" << max_i_of_same << endl;

			// Then you iterate through those bounds to iso test the graphs
			for (int i = min_i_of_same; i <= max_i_of_same; i++)
			{
				//cout << "  iso comparing g to reps[" << edges << "][" << i << "]" << endl;
				iso_test_count++;
				if (IsoTesting::del_deg_checks_tracked(g,reps[edges][i],itt))
				{
					//cout << "   It's iso to reps[" << edges << "][" << i << "]" << endl;
					guaranteed_iso = true;
					break;
				}
			}
			if (!guaranteed_iso)
			{
				//cout << "   g is non-iso to every rep with same dov" << endl;
			}
		}
		if (!dov_match) {
			//cout << " NEW REP via dov mismatch" << endl;
			// Insert at lo because I worked examples through on paper and that's correct
			reps[edges].insert(reps[edges].begin() + lo, g);
			//cout << "  inserted at reps[" << edges << "][lo=" << lo << "]" << endl;
		}
		else if (!guaranteed_iso)
		{
			//cout << " NEW REP via testing iso against all dov matches" << endl;
			// Insert the new rep at the right edge of the same_dov chunk (max_i_of_same), and shift the rest.
			// because vector.insert() specifies the position to be inserted, I use max_i_of_same+1
			reps[edges].insert(reps[edges].begin() + max_i_of_same + 1, g);
			//cout << "  inserted at reps[" << edges << "][max_i_of_same+1=" << max_i_of_same << "]" << endl;
		}
		else {
			//cout << " NO NEW REP" << endl;
		}

		// print reps
		/*
		cout << "Print Reps" << endl;
		for (int e = 0; e < reps.size(); e++)
		{
			cout << e << " edges:";
			for (GraphAdjMatrix& g_print : reps[e])
			{
				cout << " " << g_print.to_int();
			}
			cout << endl;
		}
		*/

		// END NEW BLOCK
		count++;
	} while (g.next());

	reps_file_output("repstest", reps); // "reps"+to_string(vertices)
	cout << "graphs looked at: " << count << endl;
	//itt.print_exit_cases();
	//itt.print_checks_and_timer();
	//cout << "isomorphic checks done: " << iso_test_count << endl;
}


// comparison functions for deg_of_vcs vector<int>s
// works like comparing integers, so [0,1,1,2] is less than [1,1,1,1]
bool GraphGeneration::dov_less_than(vector<int> dov1, vector<int> dov2)
{
	if (dov1.size() != dov2.size())
		cout << "ERROR: dov_less_than() dovs have different sizes" << endl;

	for (int i = 0; i<static_cast<int>(dov1.size()); i++)
	{
		if (dov1[i] < dov2[i]) // if dov1 has less of the next highest place value, true
			return true;
		else if (dov1[i] > dov2[i]) // if dov1 has more of the next highest place value, false
			return false;
	}
	return false; // both dovs are equal, return false
}
bool GraphGeneration::dov_greater_than(vector<int> dov1, vector<int> dov2)
{
	return dov_less_than(dov2, dov1);
}
bool GraphGeneration::dov_equal_to(vector<int> dov1, vector<int> dov2)
{
	if (dov1.size() != dov2.size())
	{
		cout << "ERROR: dov_equal_to() dovs have different sizes" << endl;
		return false; // anyway, they're not the same
	}

	for (int i = static_cast<int>(dov1.size() - 1); i >= 0; i--)
	{
		if (dov1[i] != dov2[i])
			return false;
	}
	return true;
}
// print the deg_of_vcs of a given graph
void GraphGeneration::print_dov_of(GraphAdjMatrix& g)
{
	vector<int> dov = g.deg_of_vcs();
	cout << "[";
	for (int i : dov)
	{
		cout << " " << i;
	}
	cout << " ]" << endl;
}


// file outputs the iso-set representatives in the vector reps
void GraphGeneration::reps_file_output(string file_name, vector<vector<GraphAdjMatrix>> reps)
{
	cout << "trying to output representatives to file " << file_name << endl;

	ofstream foutput;

	foutput.open(file_name);
	if (!foutput.is_open())
	{
		cout << "the file was unable to be opened" << endl;
	}

	// put them in a vector to be sorted properly.
	vector<int> sorted_rep_ints;
	for (vector<GraphAdjMatrix> reps_w_certain_num_of_edges : reps)
	{
		for (GraphAdjMatrix g : reps_w_certain_num_of_edges)
		{
			sorted_rep_ints.push_back(g.to_int());
		}
	}
	std::sort(sorted_rep_ints.begin(), sorted_rep_ints.end());

	// now output from sorted_reps
	for (int rep_int : sorted_rep_ints)
	{
		foutput << rep_int << endl;
	}

	foutput.close();
}


