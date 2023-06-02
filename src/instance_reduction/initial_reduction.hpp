#ifndef INIT_RED
#define INIT_RED
#include "cycle_reduction.hpp"
#include "low_degree_reduction.hpp"

class InitialReduction
{
private:
	LowReduction low;
	CycleReduction cycle;

public:
	InitialReduction(Graph* g);
	void Reduce();
};

#endif // !INIT_RED
