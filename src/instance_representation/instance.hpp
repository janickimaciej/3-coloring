#ifndef INST
#define INST

#include "graph.hpp"
#include "rules/rule.hpp"

class Instance
{
private:
	void unMerging(int v, std::vector<int>* unmerged);
public:
	int n;
	Graph* graph;
	std::vector<int> indexes;
	std::vector<int> innerTree;
	std::vector<int> deleted;
	std::vector<std::vector<int>> superVertices;

	Rule** rules;

	Instance(Graph* graph);
	Instance(const Instance& instance);
	
	static Instance* copy(const Instance* instance);

	void removeVertex(int vertex);
	void addVertex();

	static void giveColor(Graph* g, int v, int color);
	static void giveNaive(Graph* g, int v);

	std::vector<int>* unMerge(int v);
};

#endif // !INST
