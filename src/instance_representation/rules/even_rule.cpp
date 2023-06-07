#include "even_rule.hpp"

void EvenRule::ClearRule(Instance* instance)
{
	for (int ver : cycle) instance->rules[ver] = new Rule();
}

EvenRule::EvenRule(std::vector<int> cycle, std::vector<int> neighbours)
{
	this->cycle = std::vector<int>(cycle.begin(), cycle.end());
	this->neigbours = std::vector<int>(neigbours.begin(), neigbours.end());
	c = cycle.size();
}

bool EvenRule::apply(Instance* instance)
{
	int color = instance->graph->getAvailableColors(neigbours[0]).at(0);
	int color1;
	for (int i = 0; i < c; i++)
	{
		color1 = instance->graph->getAvailableColors(neigbours[(i + 1) % c]).at(0);
		if (color != color1)
		{
			if (cycle[(i + 1) % c] < instance->n)
			{
				Instance::giveColor(instance->graph, cycle[(i + 1) % c], color);
			}
			else
			{
				std::vector<int> unMerged = *instance->unMerge(cycle[(i + 1) % c]);
				for (int ver : unMerged)
				{
					Instance::giveColor(instance->graph, ver, color);
				}
			}
			for (int j = (i + 2) % c; j < i + 1; j = (j + 1) % c)
			{
				if (cycle[j] < instance->n)
				{
					Instance::giveNaive(instance->graph, cycle[j]);
				}
				else
				{
					std::vector<int> unMerged = *instance->unMerge(cycle[j]);
					for (int ver : unMerged)
					{
						Instance::giveNaive(instance->graph, ver);
					}
				}
			}
			return true;
		}
		color = color1;
	}
	int c1, c2;
	if (color == 0)
	{
		c1 = 1;
		c2 = 2;
	}
	else if (color == 1)
	{
		c1 = 0;
		c2 = 2;
	}
	else
	{
		c1 = 0;
		c2 = 1;
	}
	for (int i = 0; i < c; i++)
	{
		Instance::giveColor(instance->graph, cycle[i], i % 2 == 0 ? c1 : c2);
	}
	return true;
}
