#pragma once

#include "IsomorphicTracker.h"
#include "IsoTesting.h"
#include <map>
using std::map;
#include <set>
using std::set;

class OneOffAnalyses
{
public:
	// finds graphs that are not isomorphic but pass similar checks
	static void not_iso_similar_checks_from_reps(IsomorphicTracker it, int n);

	// looks at every pair of same_dov non-iso reps, seeing what del_degs distinguish them
	static void find_del_degs_that_distinguish_reps(IsomorphicTracker it, int n);
};

