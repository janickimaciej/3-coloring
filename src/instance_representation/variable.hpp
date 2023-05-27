#ifndef VARIABLE
#define VARIABLE

#include "color.hpp"
#include <vector>

class Variable {
	std::vector<Color> colors = std::vector<Color>(4, Color(*this));
public:
	int index;

	bool hasEdge(const Variable& end) const;
	void addEdge(Variable& end);
	void removeEdge(Variable& end);
	std::vector<int> getNeighbors() const;
};

#endif
