#include "initial_reduction.hpp"

InitialReduction::InitialReduction(Graph* g) : low(instances[0]), cycle(&instances)
{
	instances.push_back(new Instance(g));
}

std::vector<Instance*>* InitialReduction::Reduce()
{
	int i = 0;
	while (1)
	{
		cycle.setTarget(i);
		low.Set(instances[i]);
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
	
	return &instances;
}
