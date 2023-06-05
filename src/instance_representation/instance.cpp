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
