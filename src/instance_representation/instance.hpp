#ifndef INSTANCE
#define INSTANCE

#include "graph.hpp"

class Rule;

class Instance
{
private:
	void unMerging(int v, int max, std::vector<int>* unmerged);
public:
	int n;
	int originalN;
	Graph* graph;
	Graph* original;
	std::vector<int> indexes;
	std::vector<int> innerTree;
	std::vector<int> deleted;
	std::vector<std::vector<int>> superVertices;

	Rule** rules;

	Instance(Graph* graph);
	Instance(const Instance& instance);
	
	static Instance* copy(const Instance* instance);

	void removeVertex(int vertex);
	void justRemove(int vertex);
	void addVertex();
	void addEdge(int u, int v);

	static void giveColor(Graph* g, int v, int color);
	static void giveNaive(Graph* g, int v);
	static int getMergedColor(Graph* g, std::vector<int> unMerged);

	std::vector<int>* unMerge(int v, int max);
};

#endif
