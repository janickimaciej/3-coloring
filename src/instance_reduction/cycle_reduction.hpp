#ifndef CYCLE_REDUCTION
#define CYCLE_REDUCTION

#include "../instance_representation/graph.hpp"
#include "../instance_representation//instance.hpp"

class CycleReduction
{
private:
	Instance* instance;
	std::vector<Instance*>* instances;
	int n;

	std::vector<int>* neighbours;
	bool* visited;
	int* parents;
	bool* toDeletion;
	bool* threes;
	std::vector<int> cycle;

	bool hasReduced;
	int v;
	int target;

	bool cycleRec(int curr, int parent);
	void deleteCycle(int start, int end);
	void Clear();
	void Clear(Instance* g);
	int findNeighbour(int cycleVer);
	int cycleLength(int start, int end);
	void addNeighbourCycle(Instance* inst, int w1, int target);
public:

	CycleReduction(std::vector<Instance*>* instances);
	~CycleReduction(); 
	void setTarget(int g);
	void Update();
	bool Reduce();

	bool ended;


};

#endif
