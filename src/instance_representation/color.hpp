#ifndef COLOR
#define COLOR

#include <set>
#include <vector>

class Variable;

class Color {
	Variable& parent;
	std::set<Color*> constraints;
public:
	bool isAvailable = true;

	Color(Variable& parent);
	bool hasConstraint(const Color* color) const;
	std::vector<int> getNeighbors() const;
	void addConstraint(Color* color);
	void removeConstraint(Color* color);
	void clearConstraints();

	~Color();
};

#endif
