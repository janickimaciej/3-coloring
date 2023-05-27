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
	
	bool hasConstraint(int color, const Variable& end, int endColor) const;
	bool hasEdge(const Variable& end) const;
	void addConstraint(int color, Variable& end, int endColor);
	void addEdge(Variable& end);
	void removeConstraint(int color, Variable& end, int endColor);
	void removeEdge(Variable& end);
	std::vector<ConstraintInfo> getConstraints() const;
	std::vector<int> getNeighbors() const;
};

#endif
