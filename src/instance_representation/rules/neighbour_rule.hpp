#ifndef  NEIGH_RULE
#define NEIGH_RULE

#include "rule.hpp"

class NeighbourRule : Rule
{
private:
	int neighbour;
	int target;
	int c;
	std::vector<int> cycle;

	void ClearRule(Instance* instance);

public:
	NeighbourRule(int neighbour, int target, std::vector<int> cycle);
	virtual bool apply(Instance* instance) override;
};

#endif // ! NEIGH_RULE
