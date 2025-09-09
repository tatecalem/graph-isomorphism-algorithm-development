#include "OneOffAnalyses.h"
#include "GraphAdjMatrix.h"

// finds graphs that are not isomorphic but pass similar checks (look for examples)
void OneOffAnalyses::not_iso_similar_checks_from_reps(IsomorphicTracker it, int n)
{
	cout << "finding graphs that pass the same checks but aren't iso for " << n << " nodes" << endl;

	// get representatives (all the non-isomorphic graphs with n nodes)
	vector<GraphAdjMatrix> representatives = it.representatives(n);

	// stores the example graphs in a vector of sets of graphs that all pass the same tests but aren't iso
	vector<set<int>> examples;

	// go through every pair and...
	for (int i = 0; i < representatives.size(); i++)
	{
		for (int j = i + 1; j < representatives.size(); j++)
		{
			int g_int = representatives[i].to_int();
			int h_int = representatives[j].to_int();

			// CURRENT CHECKS: Are dovs equal?  Also, are dovs equal after deleting the median degree?  (this includes edge checks automatically [part of dov])
			bool pass_same_checks = IsoTesting::same_sorted_dov(representatives[i], representatives[j]);
			if (pass_same_checks) // if you passed the first test, can you pass both?
			{
				pass_same_checks = IsoTesting::same_edg_dov_after_del_median_deg(representatives[i], representatives[j]);
			}
			// pass_same_checks true iff the graphs passed both tests.

			if (pass_same_checks)
			{
				//cout << "found a pair of non-iso graphs that pass the same checks" << endl;
				//cout << " " << representatives[i].to_int() << " and " << representatives[j].to_int() << endl;
				//representatives[i].print(); cout << " and" << endl; representatives[j].print();

				// go through the existing examples and see if either graph is already in a group
				bool already_in_examples = false;
				for (set<int>& s : examples)
				{
					if (s.find(g_int) != s.end() || s.find(h_int) != s.end()) // if either graph is in a group of examples already
					{
						//cout << " one of these graphs is already in an example set" << endl;
						// add the graph ints to the example set if they're not there already.
						s.emplace(g_int);
						s.emplace(h_int);
						already_in_examples = true;
						break;
					}
				}

				if (!already_in_examples) // if these graphs aren't in examples
				{
					set<int> temp;
					temp.emplace(g_int);
					temp.emplace(h_int);
					examples.push_back(temp); // add a new example set into the vector
				}
			}
		}
	}

	cout << "printing those examples.  There are " << examples.size() << " example groups." << endl;
	for (set<int> s: examples)
	{
		cout << "Example group of size " << s.size() << endl;
		for (const auto& graph_as_int : s)
		{
			cout << " printing " << graph_as_int << endl;
			GraphAdjMatrix temp(n, graph_as_int);
			temp.print();
			cout << "  and here's what it looks like when you cut the median deg" << endl;
			GraphAdjMatrix g_del(n, graph_as_int);
			g_del.remove_all_edges_conn_to_vcs_with_median_deg();
			g_del.print();
		}
	}
	cout << "done printing examples" << endl;
	
}


// looks at every pair of same_dov non-iso reps, seeing what del_degs distinguish them
void OneOffAnalyses::find_del_degs_that_distinguish_reps(IsomorphicTracker it, int n)
{
	cout << "finding out what del_degs work to distinguish between the same_dov non-iso reps." << endl;

	// get representatives (all the non-isomorphic graphs with n nodes)
	vector<GraphAdjMatrix> representatives = it.representatives(n);

	// store the results of this analysis in some data structure

	// go through every non-iso pair and...
	vector<int> distinguishes_count; // will count every time a particular del_deg distinguishes same_dov non-iso graphs
	for (int i = 0; i < n; i++)
		distinguishes_count.push_back(0);
	for (int i = 0; i < representatives.size(); i++)
	{
		for (int j = i + 1; j < representatives.size(); j++)
		{
			int g_int = representatives[i].to_int();
			int h_int = representatives[j].to_int();

			bool same_dov = IsoTesting::same_sorted_dov(representatives[i], representatives[j]);

			if (same_dov)
			{
				// this section runs on every pair of graphs that have the same degree of vertices but aren't iso
				cout << "looking at " << representatives[i].to_int() <<
					" and " << representatives[j].to_int() << " with dov [";
				vector<int> dov = representatives[i].deg_of_vcs();
				for (int d : dov)
				{
					cout << " " << d;
				}
				cout << " ]" << endl;

				// run through every del_deg to see if it distinguishes the two graphs
				cout << " distinguished by";
				for (int del_deg = 0; del_deg < n - 1; del_deg++)
				{
					bool same_check_results = IsoTesting::same_edg_dov_after_del_deg(
						representatives[i], representatives[j], del_deg);

					
					if (!same_check_results)
					{
						cout << " " << del_deg;
						distinguishes_count[del_deg]++;
					}
				}
				cout << endl;
				
				// print the pairs of graph ints, and the del_degs that distinguish them (or don't)
			}
		}
	}
	cout << "Here's how many times each del_deg distinguished between graphs" << endl;
	for (int i=0; i<distinguishes_count.size(); i++)
	{
		cout << " del_deg of " << i << " distinguished " << distinguishes_count[i] << " times" << endl;
	}
	// TODO: go back into this code and store, then print, the results.
	// TODO: also, look into how to analyze what del_deg will best work for what dov
}





/*
Saving old code in case I break the new stuff

	cout << "finding graphs with the same deg_of_vcs but aren't iso for " << n << " nodes" << endl;

	// get representatives (all the non-isomorphic graphs with n nodes)
	vector<GraphAdjMatrix> representatives = it.representatives(n);

	// stores the example graphs in a map from the dovs to the graphs that aren't iso with that dov
	map<vector<int>, set<int>> examples;

	// go through every pair and...
	for (int i = 0; i < representatives.size(); i++)
	{
		for (int j = i + 1; j < representatives.size(); j++)
		{
			vector<int> G_deg_of_vcs = representatives[i].deg_of_vcs(); // grab a dov to use as a key later
			bool pass_same_checks = IsoTesting::same_sorted_dov(representatives[i], representatives[j]); // are the dovs equal?

			if (pass_same_checks)
			{
				//cout << "found a pair of non-iso graphs that pass the same checks" << endl;
				//representatives[i].print(); cout << " and" << endl; representatives[j].print();

				if (examples.count(G_deg_of_vcs) == 0) // if this deg_of_vcs isn't in examples
				{
					set<int> temp;
					temp.emplace(representatives[i].to_int()); // add G (as an int)
					temp.emplace(representatives[j].to_int()); // add H (as an int)
					examples.emplace(G_deg_of_vcs, temp); // add key:deg_of_vcs value:vec of the graphs
				}
				else {
					examples[G_deg_of_vcs].emplace(representatives[i].to_int());
					examples[G_deg_of_vcs].emplace(representatives[j].to_int());
				}
			}
		}
	}

	cout << "printing those examples" << endl;
	for (const auto& pair : examples)
	{
		cout << "deg_of_vcs: [";
		for (const auto& i : pair.first)
		{
			cout << i << ",";
		}
		cout << "]" << endl;

		for (const auto& graph_as_int : pair.second)
		{
			cout << " printing " << graph_as_int << endl;
			GraphAdjMatrix temp(n, graph_as_int);
			temp.print();
		}
	}
	cout << "done printing examples" << endl;
*/