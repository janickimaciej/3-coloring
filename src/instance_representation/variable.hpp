#ifndef VARIABLE
#define VARIABLE

#include "color.hpp"
#include <vector>

class Variable {
	Color colors[4];
	int index;
public:
	Variable(int index);

	int getIndex();
	void setIndex(int index);

	bool hasEdge(const Variable& end) const;
	void addEdge(Variable& end);
	void removeEdge(Variable& end);
	std::vector<int> getNeighbors() const;
};

#endif
