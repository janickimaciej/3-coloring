#ifndef RULE
#define RULE

#include "../instance.hpp"
#include "../../coloring/coloring.hpp"

class Rule
{
public:
	virtual bool apply(Instance* instance, Graph* graph) { return false; };
	virtual Rule* copy() { return new Rule(); }
};

#endif
