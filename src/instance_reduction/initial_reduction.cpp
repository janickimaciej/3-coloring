#include "initial_reduction.hpp"

InitialReduction::InitialReduction(Graph* g)
{
	instances.push_back(new Instance(g));
	low = new LowReduction(instances[0]);
	cycle = new CycleReduction(&instances);
}

InitialReduction::~InitialReduction()
{
	delete low;
	delete cycle;
}

std::vector<Instance*>* InitialReduction::Reduce()
{
	int i = 0;
	while (1)
	{
		cycle->setTarget(i);
		low->Set(instances[i]);
		while (1)
		{
			low->Reduce();
			if (i == 1)
			{
				int f = 5;
			}
			cycle->Update();
			if (!cycle->Reduce()) break;
			low->Update();
			cycle->Update();
		}
		i++;
		if (i >= instances.size()) break;
	}
	
	return &instances;
}
