#include "initial_reduction.hpp"

InitialReduction::InitialReduction(Graph* g) : low(g), cycle(g)
{
}

void InitialReduction::Reduce()
{
	while (1)
	{
		if (low.reduce())
		{
			cycle.reduce();
		}
		else
		{
			if (!cycle.reduce()) break;
		}
		low.Update();
		cycle.Update();
	}
}
