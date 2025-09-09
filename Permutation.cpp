#include "Permutation.h"

// constructor (generates [0,...,n-1] to start)
Permutation::Permutation(int n)
{
	for (int i = 0; i < n; i++)
	{
		_p.push_back(i);
	}
}


// convert to next permutation
bool Permutation::next()
{
	return std::next_permutation(_p.begin(), _p.end());
}


// get from index
int Permutation::operator[](int i)
{
	return _p[i];
}


void Permutation::print()
{
	cout << " [";
	for (int i = 0; i < _p.size(); i++)
	{
		cout << " " << _p[i];
	}
	cout << " ]" << endl;
}
