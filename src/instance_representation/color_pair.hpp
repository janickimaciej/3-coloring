#ifndef COLOR_PAIR
#define COLOR_PAIR

#include <functional>

struct ColorPair {
	int variable;
	int color;

	ColorPair(int variable, int color);
	bool operator==(const ColorPair& colorPair) const;
};

namespace std {
	template<>
	struct hash<ColorPair> {
		size_t operator()(const ColorPair& colorPair) const {
			return ((size_t)colorPair.variable + colorPair.color)*((size_t)colorPair.variable + colorPair.color + 1)/2 +
				colorPair.variable;
		}
	};
}

#endif
