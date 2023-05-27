#ifndef COLOR
#define COLOR

#include <set>
#include <vector>
#include "../color_pair.hpp"

class Variable;

class Color {
	int color;
	Variable* parent;
	std::set<Color*> constraints;
public:
	bool isAvailable = true;

	void setColor(int color);
	void setParent(Variable* parent);

	bool hasConstraint(const Color* color) const;
	std::vector<ColorPair> getConstraints() const;
	std::vector<int> getNeighbors() const;
	void addConstraint(Color* color);
	void removeConstraint(Color* color);
	void clearConstraints();

	~Color();
};

#endif
