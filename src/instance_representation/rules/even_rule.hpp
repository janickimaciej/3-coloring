#ifndef EVEN_RULE
#define EVEN_RULE

#include "rule.hpp"

class EvenRule : Rule
{
public:
	virtual bool apply(Instance* instance, Graph* graph) override;
private:
	std::vector<int> cycle;
	std::vector<int> neighbours;
	int c;

	void ClearRule(Instance* instance);
public:
	EvenRule(std::vector<int> cycle, std::vector<int> neighbours);
	virtual Rule* copy() override { return (Rule*) new EvenRule(cycle, neighbours); }
};

#endif // !EVEN_RULE
