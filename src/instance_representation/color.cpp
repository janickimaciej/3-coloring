#include "color.hpp"
#include "variable.hpp"
#include <iostream>

Color::Color(Variable& parent) : parent(parent) { }

bool Color::hasConstraint(const Color* color) const {
	return constraints.find((Color*)color) != constraints.end();
}

std::vector<int> Color::getNeighbors() const {
	std::vector<int> neighbors;
	std::set<Color*>::iterator iter = constraints.begin();
	while(iter != constraints.end()) {
		neighbors.push_back((*iter)->parent.index);
		iter++;
	}
	return neighbors;
}

void Color::addConstraint(Color* color) {
	if(color == nullptr) {
		throw "Color::addConstraint: Color can't be null";
	}
	if(hasConstraint(color)) {
		throw "Color::addConstraint: Constraint already exists";
	}
	constraints.insert(color);
	color->constraints.insert(this);
}

void Color::removeConstraint(Color* color) {
	if(!hasConstraint(color)) {
		throw "Color::removeConstraint: Constraint doesn't exist";
	}
	color->constraints.erase(this);
	constraints.erase(color);
}

void Color::clearConstraints() {
	std::set<Color*>::iterator iter;
	while((iter = constraints.begin()) != constraints.end()) {
		removeConstraint(*iter);
	}
}

Color::~Color() {
	clearConstraints();
}
