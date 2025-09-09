#include "GraphAdjMatrix.h"

// constructor
GraphAdjMatrix::GraphAdjMatrix()
{
	//cout << "GraphAdjMatrix constructor" << endl;
	_edges = 0;
}


// constructor that specifies a size and a graph by an integer code (0=empty)
GraphAdjMatrix::GraphAdjMatrix(int size, int binary_code)
{
	//cout << "making graph with size " << size << " and binary code " << binary_code << endl;
	// make an empty graph to size
	vector<bool> row;
	for (int i = 0; i < size; i++)
	{
		row.push_back(false);
	}
	for (int j = 0; j < size; j++)
	{
		_g.push_back(row);
	}
	_edges = 0;

	// fill from the binary code (filling from bottom right of upper triangle)
	int temp = binary_code;
	int row_cursor = size - 2;
	int col_cursor = size - 1;
	while (row_cursor >= 0)
	{
		//cout << "row_cursor " << row_cursor << "  col_cursor " << col_cursor << endl;
		//cout << " temp: " << temp << " which has %2 of " << temp % 2 << endl;
		bool bit = static_cast<bool>(temp % 2);
		if (bit == true)
		{
			_edges++;
		}
		_g[row_cursor][col_cursor] = bit;
		_g[col_cursor][row_cursor] = bit;
		temp /= 2;

		// move cursors
		col_cursor--;
		if (row_cursor == col_cursor)
		{
			row_cursor--;
			col_cursor = size - 1;
		}
	}
}


// constructor that makes a graph with a specific number of edges
GraphAdjMatrix::GraphAdjMatrix(int size, int edges, std::mt19937 gen)
{
	// make an empty graph with the right size
	vector<bool> row;
	for (int i = 0; i < size; i++)
	{
		row.push_back(false);
	}
	for (int j = 0; j < size; j++)
	{
		_g.push_back(row);
	}

	// make the vector of bools to shuffle and randomize
	int max_edges = (size - 1) * (size) / 2; // the number of possible edges (upper triangle of matrix)
	//cout << "max_edges: " << max_edges << endl;
	vector<bool> edge_randomize;
	for (int i = 0; i < edges; i++)
	{
		edge_randomize.push_back(true);
	}
	for (int i = edges; i < max_edges; i++)
	{
		edge_randomize.push_back(false);
	}
	std::shuffle(edge_randomize.begin(), edge_randomize.end(), gen);

	// print edge_randomize to verify it
	/*
	cout << " edge_randomize [";
	for (bool b : edge_randomize)
	{
		cout << " " << b;
	}
	cout << " ]" << endl;
	*/
	// fill from the binary code (filling from bottom right of upper triangle)
	int vec_index = 0;
	int row_cursor = size - 2;
	int col_cursor = size - 1;
	while (row_cursor >= 0)
	{
		//cout << "row_cursor " << row_cursor << "  col_cursor " << col_cursor << endl;
		//cout << " vec_index: " << vec_index << endl;
		_g[row_cursor][col_cursor] = edge_randomize[vec_index];
		_g[col_cursor][row_cursor] = edge_randomize[vec_index];

		// move cursors
		col_cursor--;
		if (row_cursor == col_cursor)
		{
			row_cursor--;
			col_cursor = size - 1;
		}
		vec_index++;
	}

	_edges = edges; // set the number of edges
}


// turns this graph into the inputted graph
GraphAdjMatrix& GraphAdjMatrix::operator =(GraphAdjMatrix graph)
{
	if (vertices() != graph.vertices())
	{
		cout << "ERROR: The graphs are different sizes" << endl;
	}

	vector<vector<bool>> g = graph.get_g();
	for (int i = 0; i < vertices(); i++)
	{
		for (int j = 0; j < vertices(); j++)
		{
			_g[i][j] = g[i][j];
		}
	}
	_edges = graph.edges(); // get the number of edges right too
	return *this;
}


vector<vector<bool>> GraphAdjMatrix::get_g()
{
	return _g;
}


// returns the number of vertices in the graph
int GraphAdjMatrix::vertices()
{
	//cout << "GAM nodes()" << endl;

	return static_cast<int>(_g.size());
}


// returns the number of edges in the graph
const int GraphAdjMatrix::edges()
{
	return _edges;
}


// verifies and corrects the number of edges (instead of trusting the in-the-moment adjustments)
bool GraphAdjMatrix::recalc_edges()
{
	int double_count = 0;
	for (vector<bool> v : _g)
	{
		for (bool edge : v)
		{
			if (edge == true)
			{
				double_count++;
			}
		}
	}
	int actual = double_count / 2; // per handshaking theorem, the number of edges is the sum of deg / 2
	//cout << "double_count: " << double_count << endl;
	if (_edges == actual)
	{
		return true;
	}
	else {
		cout << "RECALC_EDGES CAUGHT SOMETHING" << endl;
		_edges = actual;
		return false;
	}
}


// returns a vector with ints representing the degree of each vertex sorted lo to hi
vector<int> GraphAdjMatrix::deg_of_vcs()
{
	vector<int> ans;
	for (int row=0; row<vertices(); row++)
	{
		ans.push_back(0);
		for (bool edge : _g[row])
		{
			if (edge == true)
			{
				ans[row]++;
			}
		}
	}
	std::sort(ans.begin(), ans.end());
	return ans;
}


// true iff a is in the graph
bool GraphAdjMatrix::is_vx(int a)
{
	//cout << "GAM is_node()" << endl;

	return a >= 0 && a < vertices();
}


// true iff a and b are in the graph
bool GraphAdjMatrix::are_vcs(int a, int b)
{
	//cout << "GAM are_nodes()" << endl;

	return a >= 0 && a < vertices() && b >= 0 && b < vertices();
}


// get the element at _g[i][j]
bool GraphAdjMatrix::get(int i, int j)
{
	//cout << "GAM get()" << endl;

	if(are_vcs(i,j))
		return _g[i][j];
	else {
		cout << " err: either " << i << " or " << j << " isn't a node" << endl;
		return false;
	}
}


// set the element at _g[i][j] (with option to recalculate edges after)
void GraphAdjMatrix::set(int i, int j, bool b, bool recalc_e)
{
	//cout << "GAM set()" << endl;

	if (are_vcs(i, j))
		_g[i][j] = b;
	else {
		cout << " err: either " << i << " or " << j << " isn't a node" << endl;
	}

	if (recalc_e)
	{
		recalc_edges();
	}
}


// converts the graph to its integer code (using binary)
int GraphAdjMatrix::to_int()
{
	int ans = 0;
	int value = 1;
	int row = vertices() - 2;
	int col = vertices() - 1;

	// go through the relevant spaces in order
	while (row >= 0)
	{
		if (_g[row][col] == true)
		{
			ans += value;
		}

		// update value
		value *= 2;
		// move cursors
		col--;
		if (row == col)
		{
			row--;
			col = vertices() - 1;
		}
	}

	return ans;
}


// true if there is an edge, false if not
bool GraphAdjMatrix::edge_between(int a, int b)
{
	//cout << "GAM edge_between()" << endl;

	if (are_vcs(a, b))
		return _g[a][b];
	else{
		cout << " err: either " << a << " or " << b << " isn't a node" << endl;
		return false;
	}		
}


// add node
int GraphAdjMatrix::add_vx()
{
	//cout << "GAM add_node()" << endl;

	for (int i = 0; i < vertices(); i++) // for all subvectors
	{
		// add a 0 to the end of them
		_g[i].push_back(false);
	}
	// add a whole empty vector to the end of _g
	vector<bool> v;
	for (int i = 0; i <= vertices(); i++)
	{
		v.push_back(false);
	}
	_g.push_back(v);

	cout << "add_node() returning " << vertices() - 1 << endl;
	return static_cast<int>(vertices() - 1);
}


// add edge (multi-edges not allowed)
void GraphAdjMatrix::add_edge(int a, int b)
{
	//cout << "GAM add_edge()" << endl;

	if (a == b)
	{
		cout << " err: the nodes are the same." << endl;
		return;
	}
	if (!are_vcs(a,b))
	{
		cout << " err: either " << a << " or " << b << " isn't a node" << endl;
		return;
	}
	if (_g[a][b] == 1)
	{
		cout << " err: the edge is already there" << endl;
		return;
	}
	_g[a][b] = 1;
	_g[b][a] = 1;
	_edges++; // if we get here we know there wasn't an edge here already
}


// converts to next graph (returns false on overflow)
bool GraphAdjMatrix::next()
{
	//cout << "GAM next()" << endl;

	int row = static_cast<int>(vertices() - 2);
	int col = static_cast<int>(vertices() - 1);
	bool carry = true;
	while (row >= 0)
	{
		// do the binary counting
		if (carry && _g[row][col] == true) // 1+carry means set to 0 and continue carrying
		{
			_g[row][col] = false;
			_g[col][row] = false; // get the other side too
			_edges--; // the current edge is turned to 0
		}
		else if (carry && _g[row][col] == false) // 0+carry means set to 1 and stop carrying
		{
			_g[row][col] = true;
			_g[col][row] = true; // get the other side too
			carry = false;
			_edges++; // the current edge is turned to 1
		}
		// otherwise do nothing.  the existing data remains

		// find the next thing to look at
		col--;
		if (row == col)
		{
			row--;
			col = static_cast<int>(vertices() - 1);
		}
	}

	if (carry == true) // if it overflowed, it should just return empty but let's give it a message
	{
		//cout << " overflow occurred" << endl;
		return false;
	}

	return true;
}


// returns the inverse of the graph (all non-edges become edges, vice versa)
GraphAdjMatrix GraphAdjMatrix::inverse()
{
	GraphAdjMatrix inv(vertices());
	for (int i=0; i<vertices(); i++)
	{
		for (int j=0; j<vertices(); j++)
		{
			inv.set(i, j, !get(i, j), false);
		}
	}
	return inv;
}


// remove all edges connected to vertices with degree n
void GraphAdjMatrix::remove_all_edges_conn_to_vcs_with_deg(int n)
{
	//cout << "GraphAdjMatrix removing all edges connected to vertices with degree " << n << endl;

	// find all vcs with deg n.  Store them in a vector<bool> where true means it has the right deg
	vector<bool> vx_has_deg_n;
	for (vector<bool> vx_vec: _g)
	{
		int deg_count = 0;
		for (bool edge: vx_vec)
		{
			if (edge)
			{
				deg_count++;
			}
		}
		if (deg_count == n) // if the vx has the right deg, vec[vx]=true.  False otherwise.
		{
			vx_has_deg_n.push_back(true);
		}
		else {
			vx_has_deg_n.push_back(false);
		}
	}

	// print the vector
	/*
	cout << "printing vx_has_deg_n" << endl;
	cout << " [";
	for (bool val : vx_has_deg_n)
	{
		cout << " " << val;
	}
	cout << " ]" << endl;
	*/

	// remove all edges that have the right degree
	for (int i = 0; i < vertices(); i++)
	{
		if (vx_has_deg_n[i]) // if vx i has the right degree
		{
			for (int j=0; j<vertices(); j++)
			{
				if (_g[i][j] == true) // if there's an edge here
				{
					// erase it and it's counterpart
					_g[i][j] = false;
					_g[j][i] = false;
					_edges--; // adjust _edges
				}
			}
		}
	}
	recalc_edges();
}


// remove all edges connected to vertices with the median degree in this graph's dov
void GraphAdjMatrix::remove_all_edges_conn_to_vcs_with_median_deg()
{
	// find the median degree
	vector<int> deg_vec = deg_of_vcs();
	int del_deg = deg_vec[deg_vec.size() / 2];
	//cout << "cutting edges connected to vertices with degree " << del_deg << endl;
	
	// cut the edges connected to those vertices
	remove_all_edges_conn_to_vcs_with_deg(del_deg);
}


// returns true iff connected
bool GraphAdjMatrix::connected()
{
	// generate the visited tracker
	vector<bool> visited;
	for (int i = 0; i < vertices(); i++)
	{
		visited.push_back(false);
	}
	int at_node = 0; // start at node 0 WLOG (starting at 0 ensures no out-of-vec errors)

	connected_rec(at_node, visited); // begin recursive call

	for (auto b : visited)
		if (!b) // if there is a node that wasn't visited
			return false;
	return true; // else
}
void GraphAdjMatrix::connected_rec(int at_node, vector<bool>& visited)
{
	//cout << "looking at node " << at_node << endl;
	visited[at_node] = true;

	for (int n = 0; n < vertices(); n++)
	{
		//cout << " will next step be an edge to node " << n << " which is " << _g[at_node][n] << "?" << endl;
		if (_g[at_node][n] && !visited[n]) // if there's an edge to a node that hasn't been visited
		{
			connected_rec(n, visited);
		}
	}
	//cout << "end of rec step" << endl;
}


// returns a vector that assigns each node a label identifying its connected component
vector<int> GraphAdjMatrix::conn_comp()
{
	//cout << " GAM conn_comp()" << endl;
	// generate the visited tracker
	vector<int> vx_component; // labels each vertex with a component
	for (int i = 0; i < vertices(); i++)
	{
		vx_component.push_back(0);
	}
	int at_node = 0; // start at node 0 WLOG (starting at 0 ensures no out-of-vec errors)
	int current_comp = 1; // start by identifying connected component 1

	while (true) // keep finding another connected component until every vertex has been reached
	{
		//cout << "finding a connected component" << endl;

		// find a connected component
		conn_comp_rec(at_node, vx_component, current_comp); // begin recursive call

		// break if everything has been visited
		bool all_vx_visited = true;
		for (int i=0; i<vx_component.size(); i++)
			if (vx_component[i] == 0)
			{
				all_vx_visited = false;
				at_node = i; // this will be the start of the next connected component
				current_comp++;
				break; // break from just this inner loop
			}
		if (all_vx_visited) // exit condition
			break;
	}
	return vx_component;
}
void GraphAdjMatrix::conn_comp_rec(int at_node, vector<int>& vxc, int current_comp)
{
	//cout << " GAM conn_comp_rec()" << endl;
	vxc[at_node] = current_comp;

	for (int n = 0; n < vertices(); n++)
	{
		if (_g[at_node][n] && vxc[n]==0) // if there's an edge to a node that hasn't been visited
		{
			conn_comp_rec(n, vxc, current_comp);
		}
	}
}
// returns the size of each connected component (sorted) for comparison
vector<int> GraphAdjMatrix::sorted_conn_comp_sizes()
{
	vector<int> cc = conn_comp();
	vector<int> ans;
	ans.push_back(-1); // ans[0] shouldn't be anything useful because ans[1] is the size of the smallest connected component

	for (auto v_label : cc)
	{
		// extend ans if needed
		if (v_label > ans.size() - 1) {
			for (int i = static_cast<int>(ans.size()); i <= v_label; i++)
				ans.push_back(0);
		}
		ans[v_label]++; // one more vx in this connected component
	}
	std::sort(ans.begin(), ans.end());
	return ans;
}


// true iff self is iso to G
// this is a rudimentary implementation. More thorough ideas are in IsoTesting.cpp
bool GraphAdjMatrix::is_iso_to(GraphAdjMatrix G)
{
	//cout << "GAM is_iso_to()" << endl;

	if (vertices() != G.vertices())
	{
		//cout << " not iso: different number of nodes" << endl;
		return false;
	}

	/*
	// false if different number of edges
	if (edges() != G.edges())
	{
		cout << " not iso: different number of edges" << endl;
		return false;
	}
	*/

	// false if deg_of_vcs doesn't match
	vector<int> dov = deg_of_vcs();
	vector<int> G_dov = G.deg_of_vcs();
	// we can assume dovs have same lengths bc we already checked num of nodes
	for (int i = 0; i < dov.size(); i++)
	{
		if (dov[i] != G_dov[i])
		{
			//cout << " not iso: deg_of_vcs doesn't match (or edges)" << endl;
			return false;
		}
	}

	// main section.  check every permutation
	Permutation p(vertices());
	do
	{
		bool this_matches = true;
		//p.print();

		// see if the graphs match under this permutation
		for (int i = 0; i < vertices(); i++)
		{
			for (int j = 0; j < vertices(); j++)
			{
				//cout << "looking at i:" << i << " j:" << j << endl;
				if (i != j && (edge_between(i, j) != G.edge_between(p[i], p[j])))
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

	return false; // if no match found, return false
}


// print
void GraphAdjMatrix::print()
{
	cout << endl;
	for (vector<bool> v : _g)
	{
		for (bool edge : v)
		{
			if (edge == true)
			{
				cout << " " << static_cast<char>(254);
			}
			else{
				cout << " " << static_cast<char>(250);
			}
		}
		cout << endl;
	}
	cout << endl;
}


// print just the top right of the adjacency matrix (above the diagonal)
void GraphAdjMatrix::print_tr()
{
	cout << endl;
	for (int i=0; i<vertices(); i++)
	{
		for (int j=0; j<vertices(); j++)
		{
			if (i >= j)
			{
				cout << "  ";
			}
			else if (_g[i][j] == true)
			{
				cout << " " << static_cast<char>(254);
			}
			else { // the edge is false
				cout << " " << static_cast<char>(250);
			}
		}
		cout << endl;
	}
	cout << endl;
}
