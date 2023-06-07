#include "instance.hpp"


Instance::Instance(Graph* graph)
{
	this->graph = graph;
	this->n = graph->getVertexCount();
	for (int i = 0; i < graph->getVertexCount(); i++)
	{
		indexes.push_back(i);
	}
}

Instance::Instance(const Instance& instance)
{
	graph = Graph::copy(instance.graph);
	indexes = std::vector<int>(instance.indexes.begin(), instance.indexes.end());
}

Instance* Instance::copy(const Instance* instance)
{
	return new Instance(*instance);
}

void Instance::removeVertex(int vertex)
{
	deleted.push_back(indexes[vertex]);
	graph->removeVertex(vertex);
	indexes.erase(indexes.begin() + vertex);
}

void Instance::addVertex()
{
	graph->addVertex();
	indexes.push_back(n);
	n++;
}

void Instance::giveColor(Graph* g, int v, int color)
{
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

std::vector<int>* Instance::unMerge(int v)
{
	std::vector<int> unMerged;
	unMerging(v, &unMerged);
	return &unMerged;
}

void Instance::unMerging(int v, std::vector<int>* unmerged)
{
	for (std::vector<int> merge : this->superVertices)
	{
		if (merge[0] == v)
		{
			for (int i = 1; i < merge.size(); i++)
			{
				if (merge[i] < n)
				{
					unmerged->push_back(merge[i]);
				}
				else
				{
					unMerging(merge[i], unmerged);
				}
			}
		}
	}
}