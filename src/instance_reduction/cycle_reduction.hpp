#ifndef CYCLE_RED
#define CYCLE_RED

#include "../instance_representation/graph.hpp"

class CycleReduction
{
private:
	Graph* graph;
	std::vector<Graph*>* instances;
	int n;

	std::vector<int>* neighbours;
	bool* visited;
	int* parents;
	bool* toDeletion;
	bool* threes;
	std::vector<int> cycle;

	bool hasReduced;
	int v;

	bool cycleRec(int curr, int parent);
	void deleteCycle(int start, int end);
	void Clear();
public:

	CycleReduction(std::vector<Graph*>* instances);
	~CycleReduction(); 
	void setTarget(int g);
	void Update();
	bool Reduce();



};

#endif // !CYCLE_RED
