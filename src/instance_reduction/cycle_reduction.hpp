#ifndef CYCLE_RED
#define CYCLE_RED

#include "../instance_representation/graph.hpp"

class CycleReduction
{
private:
	Graph* graph;
	bool* visited;
	std::vector<int>* neighbours;
	int v;
	int* parents;
	std::vector<int> cycle;

	bool cycleRec(int curr, int parent);
	void deleteCycle(int start, int end);
public:

	CycleReduction(Graph* graph);
	~CycleReduction();
	bool hasCycle();



};

#endif // !CYCLE_RED
