#ifndef COLOR
#define COLOR

#include <set>
#include <vector>
#include "constraint_info.hpp"

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
	std::vector<ConstraintInfo> getConstraints() const;
	std::vector<int> getNeighbors() const;
	void addConstraint(Color* color);
	void removeConstraint(Color* color);
	void clearConstraints();

	~Color();
};

#endif
