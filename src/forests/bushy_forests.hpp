#ifndef BUSHY_FOREST
#define BUSHY_FOREST

#include <vector>

#include "../instance_representation/graph.hpp"
#include "../instance_representation/instance.hpp"

class BushyForest
{
private:
	std::vector<Instance*>* instances;
	Instance* instance;
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
	BushyForest(std::vector<Instance*>* instances);
	void PrepareAll();
};

#endif
