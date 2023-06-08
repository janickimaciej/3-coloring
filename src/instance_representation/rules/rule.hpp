#ifndef RULE
#define RULE

#include "../instance.hpp"
#include "../../coloring/coloring.hpp"

class Rule
{
	int n = 5;
public:
	virtual bool apply(Instance* instance, Graph* graph) { return false; };
	virtual Rule* copy() { return new Rule(); }
};

#endif
