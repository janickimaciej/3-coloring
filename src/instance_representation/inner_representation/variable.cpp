#include "variable.hpp"

Variable::Variable(int index) : index(index) {
	for(int i = 0; i < 4; i++) {
		colors[i].setColor(i);
		colors[i].setParent(this);
	}
	colors[3].isAvailable = false;
}

int Variable::getIndex() {
	return index;
}

void Variable::setIndex(int index) {
	this->index = index;
}

bool Variable::hasConstraint(int color, const Variable& end, int endColor) const {
	return colors[color].hasConstraint(&end.colors[endColor]);
}

bool Variable::hasEdge(const Variable& end) const {
	return colors[0].hasConstraint(&end.colors[0]) || colors[1].hasConstraint(&end.colors[1]) ||
		colors[2].hasConstraint(&end.colors[2]);
}

void Variable::addConstraint(int color, Variable& end, int endColor) {
	colors[color].addConstraint(&end.colors[endColor]);
}

void Variable::addEdge(Variable& end) {
	for(int i = 0; i < 3; i++) {
		if(colors[i].isAvailable) {
			colors[i].addConstraint(&end.colors[i]);
		}
	}
}

void Variable::removeConstraint(int color, Variable& end, int endColor) {
	colors[color].removeConstraint(&end.colors[endColor]);
}

void Variable::removeEdge(Variable& end) {
	for(int i = 0; i < 3; i++) {
		if(colors[i].isAvailable) {
			colors[i].removeConstraint(&end.colors[i]);
		}
	}
}

std::vector<ConstraintInfo> Variable::getConstraints() const {
	std::vector<ConstraintInfo> constraints;
	for(int color = 0; color < 4; color++) {
		std::vector<ColorPair> colorPairs = colors[color].getConstraints();
		for(int colorPair = 0; colorPair < colorPairs.size(); colorPair++) {
			constraints.push_back(ConstraintInfo(index, color, colorPairs[colorPair].variable, colorPairs[colorPair].color));
		}
	}
	return constraints;
}

std::vector<ColorPair> Variable::getConstraints(int color) const {
	return colors[color].getConstraints();
}

std::vector<int> Variable::getNeighbors() const {
	for(int i = 0; i < 3; i++) {
		std::vector<int> colorNeighbors = colors[i].getNeighbors();
		if(colorNeighbors.size() != 0) {
			return colorNeighbors;
		}
	}
	return std::vector<int>();
}

bool Variable::isColorAvailable(int color) const {
	return colors[color].isAvailable;
}

std::vector<int> Variable::getAvailableColors() const {
	std::vector<int> availableColors;
	for(int i = 0; i < 4; i++) {
		if(colors[i].isAvailable) {
			availableColors.push_back(i);
		}
	}
	return availableColors;
}

void Variable::disableColor(int color) {
	colors[color].clearConstraints();
	colors[color].isAvailable = false;
}
