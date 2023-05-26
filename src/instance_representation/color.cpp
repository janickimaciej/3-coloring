#include "color.hpp"

bool Color::hasConstraint(const Color* color) const {
	return constraints.find((Color*)color) != constraints.end();
}

void Color::addConstraint(Color* color) {
	if(color == nullptr) {
		throw "Color::addConstraint: Color can't be null";
	}
	if(hasConstraint(color)) {
		throw "Color::addConstraint: Constraint already exists";
	}
	constraints.insert(color);
}

void Color::removeConstraint(Color* color) {
	if(!hasConstraint(color)) {
		throw "Color::removeConstraint: Constraint doesn't exist";
	}
	constraints.erase(color);
}
