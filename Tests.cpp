#include "Tests.h"


// house graph is size 5 id 748
// football graph is size 5 id 742
void Tests::remove_all_edges_conn_to_vcs_with_deg_test()
{
    // make the house graph (id: 748)
    GraphAdjMatrix g1(5);
    g1.add_edge(0, 1);
    g1.add_edge(1, 2);
    g1.add_edge(2, 3);
    g1.add_edge(3, 0);
    g1.add_edge(0, 4);
    g1.add_edge(1, 4);
    g1.print();
    cout << "this graph as an int is " << g1.to_int() << endl;

    // make the football graph (id: 742)
    GraphAdjMatrix g2(5);
    g2.add_edge(0, 1);
    g2.add_edge(1, 2);
    g2.add_edge(3, 0);
    g2.add_edge(3, 2);
    g2.add_edge(4, 0);
    g2.add_edge(4, 2);
    g2.print();
    cout << "this graph as an int is " << g2.to_int() << endl;

    int n = 2;
    g1.remove_all_edges_conn_to_vcs_with_deg(n);
    cout << "g1 after removing all vcs with deg " << n << endl;
    g1.print();
    g2.remove_all_edges_conn_to_vcs_with_deg(n);
    cout << "g2 after removing all vcs with deg " << n << endl;
    g2.print();
}


void Tests::graph_constructor_w_specific_edges_test()
{
    // Initialize random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

    // make a graph with a specific number of edges
    int size = 7;
    int edges = 12;
    GraphAdjMatrix g(size, edges, gen);
    cout << "graph with size " << size << " and " << edges << "edges" << endl;
    g.print();
}


void Tests::make_random_graph_test()
{
    int n = 6;
    // Find the total number of unique graphs of size n
    int total_graphs = static_cast<int>(std::pow(2, (n * (n - 1)) / 2));
    // Initialize random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> range(0, total_graphs - 1); // inclusive

    GraphAdjMatrix g(n, range(gen));
    cout << "I made a random graph" << endl;
    g.print();
}


void Tests::dov_comparison_test()
{
    vector<int> dov1;
    dov1.push_back(2);
    dov1.push_back(2);
    dov1.push_back(3);
    vector<int> dov2;
    dov2.push_back(1);
    dov2.push_back(2);
    dov2.push_back(3);
    cout << "comparing [";
    for (int n : dov1)
        cout << " " << n;
    cout << " ] and [";
    for (int n : dov2)
        cout << " " << n;
    cout << " ]" << endl;
    cout << " < result " << GraphGeneration::dov_less_than(dov1, dov2) << endl;
    cout << " > result " << GraphGeneration::dov_greater_than(dov1, dov2) << endl;
    cout << " = result " << GraphGeneration::dov_equal_to(dov1, dov2) << endl;
}
