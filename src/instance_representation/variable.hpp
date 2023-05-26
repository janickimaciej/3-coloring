#ifndef VARIABLE
#define VARIABLE

#include "color.hpp"

class Variable {
	Variable* next = nullptr;
	Color colors[4];
public:
	bool exists = true;
	void setNext(Variable* next);

	bool hasEdge(const Variable& end) const;
	void addEdge(Variable& end);
	void removeEdge(Variable& end);
};

#endif
