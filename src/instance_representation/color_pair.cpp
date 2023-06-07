#include "color_pair.hpp"

ColorPair::ColorPair(int variable, int color) : variable(variable), color(color) { }

bool ColorPair::operator==(const ColorPair& colorPair) const {
	return variable == colorPair.variable && color == colorPair.color;
}

bool ColorPair::operator!=(const ColorPair& colorPair) const {
	return !(*this == colorPair);
}
