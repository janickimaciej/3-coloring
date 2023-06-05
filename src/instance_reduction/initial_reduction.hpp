#ifndef INIT_RED
#define INIT_RED

#include <vector>

#include "cycle_reduction.hpp"
#include "low_degree_reduction.hpp"

class InitialReduction
{
private:
	LowReduction low;
	CycleReduction cycle;
	// TreeReduction if time
	std::vector<Instance*> instances;

public:
	InitialReduction(Graph* g);
	std::vector<Instance*>* Reduce();
};

#endif // !INIT_RED
