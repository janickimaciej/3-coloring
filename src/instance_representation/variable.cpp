#include "variable.hpp"

Variable::Variable(int index) : index(index) {
	for(int i = 0; i < 4; i++) {
		colors[i].setColor(i);
		colors[i].setParent(this);
	}
}

int Variable::getIndex() {
	return index;
}

void Variable::setIndex(int index) {
	this->index = index;
}

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

void Variable::addConstraint(int color, Variable& end, int endColor) {
	colors[color].addConstraint(&end.colors[endColor]);
}

void Variable::removeEdge(Variable& end) {
	if(!hasEdge(end)) {
		throw "Variable::removeEdge: Edge doesn't exist";
	}
	for(int i = 0; i < 3; i++) {
		colors[i].removeConstraint(&end.colors[i]);
	}
}

std::vector<ConstraintInfo> Variable::getConstraints() const {
	std::vector<ConstraintInfo> constraints;
	for(int i = 0; i < 4; i++) {
		std::vector<ConstraintInfo> colorConstraints = colors[i].getConstraints();
		constraints.insert(constraints.end(), colorConstraints.begin(), colorConstraints.end());
	}
	return constraints;
}

std::vector<int> Variable::getNeighbors() const {
	return colors[0].getNeighbors();
}
