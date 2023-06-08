#include "even_rule.hpp"

void EvenRule::ClearRule(Instance* instance)
{
	for (int ver : cycle) instance->rules[ver] = new Rule();
}

EvenRule::EvenRule(std::vector<int> cycle, std::vector<int> neighbours)
{
	this->cycle = std::vector<int>(cycle.begin(), cycle.end());
	this->neighbours = std::vector<int>(neighbours.begin(), neighbours.end());
	c = cycle.size();
}

bool EvenRule::apply(Instance* instance, Graph* graph)
{
	int neigh = neighbours[0];
	
	int n = graph->getVertexCount();
	int color;
	if (neigh < n)
	{
		color = graph->getAvailableColors(neighbours[0]).at(0);
	} 
	else
	{
		color = graph->getAvailableColors(instance->unMerge(neigh, n)->at(0)).at(0);
	}
	int color1;
	for (int i = 0; i < c; i++)
	{
		neigh = neighbours[(i + 1) % c];
		if (neigh < n)
		{
			color1 = graph->getAvailableColors(neighbours[(i + 1) % c]).at(0);
		}
		else
		{
			if (neigh == 252)
			{
				int x = 5;
			}
			int k = instance->unMerge(neigh, n)->at(0);
			color1 = graph->getAvailableColors(instance->unMerge(neigh, n)->at(0)).at(0);
		}
		if (color != color1)
		{
			if (cycle[(i + 1) % c] < graph->getVertexCount())
			{
				Instance::giveColor(graph, cycle[(i + 1) % c], color);
			}
			else
			{
				std::vector<int> unMerged = *instance->unMerge(cycle[(i + 1) % c], graph->getVertexCount());
				for (int ver : unMerged)
				{
					Instance::giveColor(graph, ver, color);
				}
			}
			for (int j = (i + 2) % c; j < i + 1; j = (j + 1) % c)
			{
				if (cycle[j] < n)
				{
					Instance::giveNaive(graph, cycle[j]);
				}
				else
				{
					std::vector<int> unMerged = *instance->unMerge(cycle[j], graph->getVertexCount());
					for (int ver : unMerged)
					{
						Instance::giveNaive(graph, ver);
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
		Instance::giveColor(graph, cycle[i], i % 2 == 0 ? c1 : c2);
	}
	return true;
}
