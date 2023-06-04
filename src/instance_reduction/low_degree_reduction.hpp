#ifndef LOW_RED
#define LOW_RED

#include "../instance_representation/graph.hpp"

class LowReduction
{
private: 

	Graph* g;
	bool* toDeletion;
	int* degrees;
	std::vector<int>* neighbours;
	int n;

	bool hasReduced;

	void ReduceRec(int v);

public:
	LowReduction(Graph* g);
	~LowReduction();
	bool Reduce();
	void Update();
	void Set(Graph* g);
};
#endif // !LOW_RED
