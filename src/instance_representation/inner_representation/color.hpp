#ifndef COLOR
#define COLOR

#include <set>
#include <vector>
#include "../color_pair.hpp"

namespace InnerRepresentation {
	class Variable;

	class Color {
		const int color;
		const Variable* const parent;
		std::set<Color*> constraints;
		bool isAvailable;
	public:
		Color(int color, const Variable* parent, bool isAvailable);

		bool hasConstraint(const Color* color) const;
		std::vector<ColorPair> getConstraints() const;
		std::vector<int> getNeighbors() const;
		void addConstraint(Color* color);
		void removeConstraint(Color* color);
		void clearConstraints();
		bool getIsAvailable() const;
		void disable();

		~Color();
	};
}

#endif
