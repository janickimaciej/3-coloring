#include "instance.hpp"
#include "../instance_representation/rules/rule.hpp"

Instance::Instance(Graph* graph)
{
	this->graph = graph;
	this->n = graph->getVertexCount();
	this->originalN = this->n;
	rules = new Rule*[originalN];
	for (int i = 0; i < originalN; i++)
	{
		rules[i] = new Rule();
		indexes.push_back(i);
	}
}

Instance::Instance(const Instance& instance)
{
	graph = Graph::copy(instance.graph);
	this->n = instance.n;
	this->originalN = instance.originalN;
	indexes = std::vector<int>(instance.indexes.begin(), instance.indexes.end());
	innerTree = std::vector<int>(instance.innerTree.begin(), instance.innerTree.end());
	deleted = std::vector<int>(instance.deleted.begin(), instance.deleted.end());
	superVertices = std::vector<std::vector<int>>(instance.superVertices.begin(), instance.superVertices.end());
	rules = new Rule * [originalN];
	for (int i = 0; i < originalN; i++)
	{
		rules[i] = instance.rules[i]->copy();
	}
}

Instance* Instance::copy(const Instance* instance)
{
	return new Instance(*instance);
}

void Instance::removeVertex(int vertex)
{
	deleted.push_back(indexes[vertex]);
	if (indexes[vertex] == 237)
	{
		int vb = 3;
	}
	graph->removeVertex(vertex);
	indexes.erase(indexes.begin() + vertex);
}

void Instance::addVertex()
{
	if (n == 264 || n == 263)
	{
		int a = 4;
	}
	graph->addVertex();
	indexes.push_back(n);
	n++;
}

void Instance::giveColor(Graph* g, int v, int color)
{
	if (v == 81)
	{
		int vb = 3;
	}
	g->setColor(v, color);
	for (int ver : g->getNeighbors(v))
	{
		if (g->isColorAvailable(ver, color))
		{
			g->disableColor(ver, color);
		}
	}
}

void Instance::giveNaive(Graph* g, int v)
{
	Instance::giveColor(g, v, g->getAvailableColors(v).at(0));
}

int Instance::getMergedColor(Graph* g, std::vector<int> unMerged)
{
	int color = -1;
	int colors[3] = { 0 };
	for (int mer : unMerged)
	{
		std::vector<int> c = g->getAvailableColors(mer);
		for (int col : c)
		{
			colors[col]++;
		}
	}
	for (int i = 0; i < 3; i++)
	{
		if (colors[i] == unMerged.size())
		{
			color = i;
			break;
		}
	}
	return color;
}

std::vector<int>* Instance::unMerge(int v, int max)
{
	std::vector<int>* unMerged = new std::vector<int>;
	unMerging(v, max, unMerged);
	return unMerged;
}

void Instance::unMerging(int v, int max, std::vector<int>* unmerged)
{
	for (std::vector<int> merge : this->superVertices)
	{
		if (merge[0] == v)
		{
			for (int i = 1; i < merge.size(); i++)
			{
				if (merge[i] < max)
				{
					unmerged->push_back(merge[i]);
				}
				else
				{
					unMerging(merge[i], max, unmerged);
				}
			}
		}
	}
}