#ifndef B_FOREST
#define B_FOREST

#include <vector>

#include "../instance_representation/graph.hpp"

class BushyForest
{
private:
	std::vector<Graph*>* instances;
	Graph* graph;
	bool* inForest;
	bool* insideNode;
	std::vector<int>* neighbours;
	std::vector<int> forest;
	void setTarget(int target);
	int n;
	int outNeigh;
	void CheckSoil(int place);
	void PlantTree(int root);
	void CheckLeaf(int leaf);
	void PlantForest();
	void generateInstances();

public:
	BushyForest(std::vector<Graph*>* instances);
	void PrepareAll();
};

#endif // !B_FOREST

