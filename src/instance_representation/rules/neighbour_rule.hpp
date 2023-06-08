#ifndef NEIGHBOUR_RULE
#define NEIGHBOUR_RULE

#include "rule.hpp"

class NeighbourRule : Rule
{
public:
	virtual bool apply(Instance* instance, Graph* graph) override;
private:
	int neighbour;
	int target;
	int c;
	std::vector<int> cycle;

	void ClearRule(Instance* instance);

public:
	NeighbourRule(int neighbour, int target, std::vector<int> cycle);
	virtual Rule* copy() override { return (Rule*) new NeighbourRule(neighbour, target, cycle); }
};

#endif
