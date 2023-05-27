#include "variable.hpp"

bool Variable::hasEdge(const Variable& end) const {
	return colors[0].hasConstraint(&end.colors[0]);
}

void Variable::addEdge(Variable& end) {
	if(hasEdge(end)) {
		throw "Variable::addEdge: Edge already exists";
	}
	for(int i = 0; i < 3; i++) {
		colors[i].addConstraint(&end.colors[i]);
	}
}

void Variable::removeEdge(Variable& end) {
	if(!hasEdge(end)) {
		throw "Variable::removeEdge: Edge doesn't exist";
	}
	for(int i = 0; i < 3; i++) {
		colors[i].removeConstraint(&end.colors[i]);
	}
}

std::vector<int> Variable::getNeighbors() const {
	return colors[0].getNeighbors();
}
