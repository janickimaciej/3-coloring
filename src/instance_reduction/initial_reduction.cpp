#include "initial_reduction.hpp"

InitialReduction::InitialReduction(Graph* g) : low(g), cycle(&instances)
{
	instances.push_back(g);
}

void InitialReduction::Reduce()
{
	int i = 0;
	while (1)
	{
		cycle.setTarget(i);
		low = LowReduction(instances[i]);
		while (1)
		{
			low.Reduce();
			if (!cycle.Reduce()) break;
			low.Update();
			cycle.Update();
		}
		i++;
		if (i > instances.size()) break;
	}
}
