#include "color.hpp"
#include "variable.hpp"
#include <iostream>

void Color::setColor(int color) {
	this->color = color;
}

void Color::setParent(Variable* parent) {
	this->parent = parent;
}

bool Color::hasConstraint(const Color* color) const {
	return constraints.find((Color*)color) != constraints.end();
}

std::vector<ConstraintInfo> Color::getConstraints() const {
	std::vector<ConstraintInfo> constraintInfos;
	for(std::set<Color*>::iterator iter = constraints.begin(); iter != constraints.end(); iter++) {
		constraintInfos.push_back(ConstraintInfo(parent->getIndex(), color, (*iter)->parent->getIndex(), (*iter)->color));
	}
	return constraintInfos;
}

std::vector<int> Color::getNeighbors() const {
	std::vector<int> neighbors;
	for(std::set<Color*>::iterator iter = constraints.begin(); iter != constraints.end(); iter++) {
		neighbors.push_back((*iter)->parent->getIndex());
	}
	return neighbors;
}

void Color::addConstraint(Color* color) {
	if(hasConstraint(color)) {
		return;
	}
	constraints.insert(color);
	color->constraints.insert(this);
}

void Color::removeConstraint(Color* color) {
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
