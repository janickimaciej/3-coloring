#ifndef INST
#define INST

#include "graph.hpp"

class Instance
{

	int n;

public:
	Graph* graph;
	std::vector<int> indexes;
	std::vector<int> innerTree;
	std::vector<int> deleted;
	std::vector<std::vector<int>> superVertices;

	Instance(Graph* graph);
	Instance(const Instance& instance);
	
	static Instance* copy(const Instance* instance);

	void removeVertex(int vertex);
	void addVertex();
};

#endif // !INST

