#ifndef VARIABLE
#define VARIABLE

#include "color.hpp"
#include <vector>
#include "constraint_info.hpp"
#include "../color_pair.hpp"

namespace InnerRepresentation {
	enum class InitialType {
		Variable,
		Vertex
	};

	class Variable {
		Color colors[4];
		int index;
	public:
		Variable(int index, InitialType nodeType);

		int getIndex() const;
		void setIndex(int index);
	
		bool hasConstraint(int color, const Variable& end, int endColor) const;
		bool hasEdge(const Variable& end) const;
		void addConstraint(int color, Variable& end, int endColor);
		void addEdge(Variable& end);
		void removeConstraint(int color, Variable& end, int endColor);
		void removeEdge(Variable& end);
		std::vector<ConstraintInfo> getConstraints() const;
		std::vector<ColorPair> getConstraints(int color) const;
		std::vector<int> getNeighbors() const;

		bool isColorAvailable(int color) const;
		std::vector<int> getAvailableColors() const;
		void disableColor(int color);
		void setColor(int color);
	};
}

#endif
