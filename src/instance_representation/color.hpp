#ifndef COLOR
#define COLOR

#include <set>

class Color {
	std::set<Color*> constraints;
public:
	bool isAvailable = true;
	bool hasConstraint(const Color* color) const;
	void addConstraint(Color* color);
	void removeConstraint(Color* color);
};

#endif
