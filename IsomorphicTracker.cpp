#include "IsomorphicTracker.h"

// constructor that pads out the vector to a certain node count
IsomorphicTracker::IsomorphicTracker(int max_nodes)
{
	for (int i = 0; i <= max_nodes; i++)
	{
		_it.push_back(vector<set<int>>());
	}
}


// returns the largest node count that we can call without vector out of range
int IsomorphicTracker::largest_node_count()
{
	return static_cast<int>(_it.size() - 1);
}


// reads into this from the stored files
void IsomorphicTracker::retrieve(int lo, int hi)
{
	if (hi == -1)
		hi = lo;
	for (int n = lo; n <= hi; n++)
	{
		cout << "pulling the stored iso groups for " << n << " nodes" << endl;

		ifstream input;
		input.open("it" + to_string(n));
		if (!input.is_open())
		{
			cout << "the file was unable to be opened" << endl;
		}

		string s;
		set<int> temp_set;
		while (input >> s)
		{
			if (s == "-")  // if you've reached the breakpoint
			{
				_it[n].push_back(temp_set); // add the iso set
				temp_set.clear(); // reset the temp set
			}
			else {
				temp_set.emplace(std::stoi(s));
			}
		}

		input.close();
	}
}


// generates all sets of isomorphic graphs for a given node count
void IsomorphicTracker::generate(int nodes)
{
	if (nodes > largest_node_count())
	{
		cout << " err: node_count not in current tracker range" << endl;
		return;
	}
	else {
		cout << "generating for node_count " << nodes << endl;
	}

	// set up progress tracking
	int total_graphs = static_cast<int>(std::pow(2, (nodes * (nodes - 1)) / 2));
	cout << "total graphs of size " << nodes << ": " << total_graphs << endl;
	int count = 0;
	int divisions = 100;
	int chunk = total_graphs / divisions;
	//cout << "tenth: " << tenth << endl;
	int next_printout = chunk;
	int chunk_count = 1;

	// set up timer
	steady_clock::time_point start = high_resolution_clock::now();

	// set up storage for the first graph in each iso_set
	vector<GraphAdjMatrix> representatives;

	GraphAdjMatrix G(nodes);
	do
	{
		/*
		if (G.recalc_edges() == false)
		{
			cout << " ERR: edges desynced" << endl;
			exit(1);
		}
		*/

		// print status update
		if (count == next_printout)
		{
			// calculate time
			steady_clock::time_point end = high_resolution_clock::now();
			milliseconds ms = duration_cast<milliseconds>(duration<double>(end-start));
			start = end; // set start to now to time the next chunk
			
			// print update
			cout << " " << 100*chunk_count/divisions << "% " << ms.count() << " ms" << endl;
			chunk_count++;
			next_printout += chunk;
		}

		// don't bother with this if it has a node with no edges connected to it
		//   we've thought about that graph before in a less-nodes round
		// also don't bother with this if it has a node with max edges connected
		//   its inverse has an isolated node
		vector<int> dov = G.deg_of_vcs();
		if (nodes>0 && (dov[0] != 0 && dov[dov.size() - 1] != nodes - 1))
		{
			//cout << "let's see if this is iso to anything" << endl;
			//G.print();
			bool iso_to_existing_set = false;
			for (int i = 0; i < representatives.size(); i++) // for each representative
			{
				//cout << " checking if G is iso to this" << endl;
				//representatives[i].print();
				if (G.is_iso_to(representatives[i]))
				{
					//cout << " iso to an existing set" << endl;
					iso_to_existing_set = true;
					_it[nodes][i].emplace(G.to_int());
					break;
				}
			}
			if (!iso_to_existing_set) // put it in its own set of isomorphic graphs
			{
				//cout << " starting its own set" << endl;
				set<int> temp;
				temp.emplace(G.to_int());
				_it[nodes].push_back(temp);

				// add it to the representatives vector
				representatives.push_back(G);
			}
		}
		count++;
	} while (G.next());
}


// generates up to n
void IsomorphicTracker::generate_up_to(int n)
{
	if (n > largest_node_count())
	{
		cout << " err: node_count not in current tracker range" << endl;
		return;
	}
	else {
		cout << "generating UP TO node_count " << n << endl;
	}
	for (int i = 0; i <= n; i++)
	{
		generate(i);
	}
}


// returns a vector of the first graph in each isomorphic group for a given number of nodes
vector<GraphAdjMatrix> IsomorphicTracker::representatives(int n)
{
	vector<GraphAdjMatrix> ans;
	for (auto& iso_set : _it[n])
	{
		GraphAdjMatrix g(n, *iso_set.begin());
		//cout << "here's a rep" << endl;
		//g.print();
		ans.push_back(g);
	}
	return ans;
}


// file output
void IsomorphicTracker::file_output(int nodes)
{
	cout << "outputting to files (or at least trying to)" << endl;

	ofstream foutput;
	
	cout << "file output to it" << nodes << endl;
	foutput.open("it" + to_string(nodes));
	if (!foutput.is_open())
	{
		cout << "the file was unable to be opened" << endl;
	}

	for (set<int> s : _it[nodes])
	{
		for (int code : s)
		{
			foutput << " " << code;
		}
		foutput << endl << "-" << endl;
	}

	foutput.close();
}


// print
void IsomorphicTracker::print()
{
	for (int n=0; n<=largest_node_count(); n++)
	{
		cout << "sets of isomorphic graphs with " << n << " nodes (with no isolated or fully-connected vcs)" << endl;
		long count = 0;
		for (set<int> s : _it[n])
		{
			for (int code : s)
			{
				cout << " " << code;
				count++;
			}
			cout << endl;
		}
		cout << "that was " << count << " graphs tracked out of " << static_cast<int>(std::pow(2, (n * (n - 1)) / 2)) << " total" << endl;
	}
}
