#ifndef LOW_DEGREE_REDUCTION
#define LOW_DEGREE_REDUCTION

#include "../instance_representation/graph.hpp"
#include "../instance_representation/instance.hpp"

class LowReduction
{
private: 

	Instance* g;
	bool* toDeletion;
	bool* visited;
	int* degrees;
	std::vector<int>* neighbours;
	int n;

	bool hasReduced;

	void ReduceRec(int v);

public:
	LowReduction(Instance* g);
	~LowReduction();
	bool Reduce();
	void Update();
	void Set(Instance* g);
};
#endif
