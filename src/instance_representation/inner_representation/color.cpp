#include "color.hpp"
#include "variable.hpp"
#include <iostream>

namespace InnerRepresentation {
	Color::Color(int color, const Variable* parent, bool isAvailable) : color(color), parent(parent),
		isAvailable(isAvailable) { }

	bool Color::hasConstraint(const Color* color) const {
		return constraints.find((Color*)color) != constraints.end();
	}

	std::vector<ColorPair> Color::getConstraints() const {
		std::vector<ColorPair> colorPairs;
		for(std::set<Color*>::iterator iter = constraints.begin(); iter != constraints.end(); iter++) {
			colorPairs.push_back(ColorPair((*iter)->parent->getIndex(), (*iter)->color));
		}
		return colorPairs;
	}

	std::vector<int> Color::getNeighbors() const {
		std::vector<int> neighbors;
		for(std::set<Color*>::iterator iter = constraints.begin(); iter != constraints.end(); iter++) {
			neighbors.push_back((*iter)->parent->getIndex());
		}
		return neighbors;
	}

	void Color::addConstraint(Color* color) {
		if(!isAvailable || !color->isAvailable) {
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

	bool Color::getIsAvailable() const {
		return isAvailable;
	}

	void Color::disable() {
		clearConstraints();
		isAvailable = false;
	}

	Color::~Color() {
		clearConstraints();
	}
}
