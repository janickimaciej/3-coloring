#ifndef VARIABLE
#define VARIABLE

#include "color.hpp"

class Variable {
	Variable* next = nullptr;
	Color colors[4];
public:
	bool exists = true;
	void setNext(Variable* next);
};

#endif
