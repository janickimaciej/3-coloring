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
		for(auto constraint = constraints.begin(); constraint != constraints.end(); constraint++) {
			colorPairs.push_back(ColorPair((*constraint)->parent->getIndex(), (*constraint)->color));
		}
		return colorPairs;
	}

	std::vector<int> Color::getNeighbors() const {
		std::vector<int> neighbors;
		for(auto constraint = constraints.begin(); constraint != constraints.end(); constraint++) {
			neighbors.push_back((*constraint)->parent->getIndex());
		}
		return neighbors;
	}

	void Color::addConstraint(Color* color) {
		if(!isAvailable || !color->isAvailable) {
			return;
		}
		if(color == this) {
			disable();
			return;
		}
		if(color->parent == parent) {
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
		auto constraint = constraints.begin();
		while(constraint != constraints.end()) {
			removeConstraint(*constraint);
			constraint = constraints.begin();
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
