#pragma once

#include <vector>
using std::vector;
#include <algorithm>
#include <iostream>
using std::cout;
using std::endl;

class Permutation
{
public:
	// constructor (generates [0,...,n-1] to start)
	Permutation(int n);

	// convert to next permutation
	bool next();

	// get from index
	int operator[](int i);

	// print
	void print();

private:
	vector<int> _p;
};

