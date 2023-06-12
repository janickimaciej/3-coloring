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

bool NeighbourRule::apply(Instance* instance, Graph* graph)
{
	c = cycle.size();
	int n = graph->getVertexCount();
	int color;
	color = graph->getAvailableColors(neighbour).at(0);
	if (cycle[target] < n)
	{
		Instance::giveColor(graph, cycle[target], color);
	}
	else
	{
		std::vector<int> unMerged = *instance->unMerge(cycle[target], n);
		Instance::giveColor(graph, cycle[target], color);
		for (int ver : unMerged)
		{
			Instance::giveColor(graph, ver, color);
		}
	}
	for (int i = (target + 1) % c; i != target; i = (i + 1) % c)
	{
		if (cycle[i] < n)
		{
			Instance::giveNaive(graph, cycle[i]);
		}
		else
		{
			std::vector<int> unMerged = *instance->unMerge(cycle[i], n);
			int color = graph->getAvailableColors(cycle[i]).at(0);
			Instance::giveColor(graph, cycle[i], color);
			for (int ver : unMerged)
			{
				Instance::giveColor(graph, ver, color);
			}
		}
	}
	ClearRule(instance);
	return true;
}
