#include "neighbour_rule.hpp"

void NeighbourRule::ClearRule(Instance* instance)
{
	for (int ver : cycle) instance->rules[ver] = new Rule;
}

NeighbourRule::NeighbourRule(int neighbour, int target, std::vector<int> cycle)
{
	this->neighbour = neighbour;
	this->target = target;
	this->cycle = std::vector<int>(cycle.begin(), cycle.end());
	int c = cycle.size();
}

bool NeighbourRule::apply(Instance* instance)
{
	if (cycle[target] < instance->n)
	{
		Instance::giveColor(instance->graph, cycle[target], instance->graph->getAvailableColors(neighbour).at(0));
	}
	else
	{
		std::vector<int> unMerged = *instance->unMerge(cycle[target]);
		for (int ver : unMerged)
		{
			Instance::giveColor(instance->graph, ver, instance->graph->getAvailableColors(neighbour).at(0));
		}
	}
	for (int i = (target + 1) % c; i < target; i = (i + 1) % c)
	{
		if (cycle[i] < instance->n)
		{
			Instance::giveNaive(instance->graph, cycle[i]);
		}
		else
		{
			std::vector<int> unMerged = *instance->unMerge(cycle[i]);
			for (int ver : unMerged)
			{
				Instance::giveNaive(instance->graph, ver);
			}
		}
	}
	ClearRule(instance);
	return true;
}
