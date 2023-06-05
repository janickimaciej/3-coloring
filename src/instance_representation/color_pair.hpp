#ifndef COLOR_PAIR
#define COLOR_PAIR

struct ColorPair {
	int variable;
	int color;

	ColorPair(int variable, int color);
	bool operator==(const ColorPair& colorPair);
};

#endif
