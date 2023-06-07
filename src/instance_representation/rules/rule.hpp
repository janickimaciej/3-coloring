#ifndef RULE
#define RULE

#include "../instance.hpp"
#include "../../coloring/coloring.hpp"

class Rule
{
public:
	virtual bool apply(Instance* instance) { return false; };
};

#endif // !RULE
