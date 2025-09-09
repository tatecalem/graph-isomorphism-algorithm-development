#include "GraphAdjMatrix.h"
#include "Permutation.h"
#include "IsomorphicTracker.h"
#include "OneOffAnalyses.h"
#include "IsoTesting.h"
#include "GraphGeneration.h"
#include "Tests.h"

#include <iostream>
using std::cout;
using std::endl;

void demonstration();

int main()
{
    demonstration();

    return 0;
}

void demonstration()
{
    cout << "Demonstrating what different parts of this code can do." << endl;
    cout << "  making an empty graph with 5 vertices, then adding edges" << endl;
    GraphAdjMatrix g_d_1(5);
    g_d_1.print();
    g_d_1.add_edge(0, 3);
    g_d_1.add_edge(3, 2);
    g_d_1.add_edge(2, 4);
    g_d_1.print();
    cout << "  Is g_d_1 connected? " << g_d_1.connected() << endl;
    cout << "  What are the degrees of all the vertices of g_d_1?" << endl;
    vector<int> g_d_1_dov = g_d_1.deg_of_vcs();
    cout << "  [";
    for (auto& i : g_d_1_dov)
    {
        cout << " " << i;
    }
    cout << "]" << endl;
    GraphAdjMatrix g_d_1_inv = g_d_1.inverse();
    g_d_1_inv.print();
    GraphAdjMatrix g_d_2(5, 35);
    cout << "  Is g_d_1 isomorphic to this graph? " << g_d_1.is_iso_to(g_d_2);
    g_d_2.print();
    cout << "  deleting all edges that connect to vertices with a degree of 1 from g_d_1." << endl;
    cout << "   (this was the tool I needed for my best isomorphism testing algorithm)" << endl;
    g_d_1.remove_all_edges_conn_to_vcs_with_deg(1);
    g_d_1.print();

    cout << "  generating all graphs of size 5, seeing which ones are isomorphic to each other," << endl;
    cout << "   then making a list of the ones that aren't isomorphic" << endl;
    GraphGeneration::generate_reps_bin_search(5);

    cout << "Finally, take a look at the del_deg_checks() function in IsoTesting.cpp." << endl;
    cout << " It's my fastest and most efficient algorithm that does increasingly complicated" << endl;
    cout << " checks to see if it can easily rule out isomorphism before brute forcing it finally." << endl;
    cout << " It checks if the graphs have the same number of vertices, number of edges," << endl;
    cout << " degree of all the vertices, and then removes all edges connected to vertices" << endl;
    cout << " that have the median degree of all the vertices and sees if those altered graphs" << endl;
    cout << " might still be isomorphic." << endl;
}




