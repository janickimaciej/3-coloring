#include "color_pair.hpp"

ColorPair::ColorPair(int variable, int color) : variable(variable), color(color) { }

bool ColorPair::operator==(const ColorPair& colorPair) {
	return variable == colorPair.variable && color == colorPair.color;
}
