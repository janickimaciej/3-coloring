#ifndef EVEN_RULE
#define EVEN_RULE

#include "rule.hpp"

class EvenRule : Rule
{
	std::vector<int> cycle;
	std::vector<int> neigbours;
	int c;

	void ClearRule(Instance* instance);
public:
	EvenRule(std::vector<int> cycle, std::vector<int> neighbours);
	virtual bool apply(Instance* instance) override;
};

#endif // !EVEN_RULE
