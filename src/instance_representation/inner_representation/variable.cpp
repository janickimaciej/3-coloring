#include "variable.hpp"

namespace InnerRepresentation {
	Variable::Variable(int index, InitialType nodeType) :
		colors {
			Color(0, this, true),
			Color(1, this, true),
			Color(2, this, true),
			Color(3, this, nodeType == InitialType::Variable)
		}, index(index) { }

	int Variable::getIndex() const {
		return index;
	}

	void Variable::setIndex(int index) {
		this->index = index;
	}

	bool Variable::hasConstraint(int color, const Variable& end, int endColor) const {
		return colors[color].hasConstraint(&end.colors[endColor]);
	}

	bool Variable::hasEdge(const Variable& end) const {
		for(int color = 0; color < 3; color++) {
			if(colors[color].hasConstraint(&end.colors[color])) {
				return true;
			}
		}
		return false;
	}

	void Variable::addConstraint(int color, Variable& end, int endColor) {
		colors[color].addConstraint(&end.colors[endColor]);
	}

	void Variable::addEdge(Variable& end) {
		for(int color = 0; color < 3; color++) {
			colors[color].addConstraint(&end.colors[color]);
		}
	}

	void Variable::removeConstraint(int color, Variable& end, int endColor) {
		colors[color].removeConstraint(&end.colors[endColor]);
	}

	void Variable::removeEdge(Variable& end) {
		for(int color = 0; color < 3; color++) {
			colors[color].removeConstraint(&end.colors[color]);
		}
	}

	std::vector<ConstraintInfo> Variable::getConstraints() const {
		std::vector<ConstraintInfo> constraints;
		for(int color = 0; color < 4; color++) {
			std::vector<ColorPair> colorPairs = colors[color].getConstraints();
			for(int colorPair = 0; colorPair < colorPairs.size(); colorPair++) {
				constraints.push_back(ConstraintInfo(index, color, colorPairs[colorPair].variable, colorPairs[colorPair].color));
			}
		}
		return constraints;
	}

	std::vector<ColorPair> Variable::getConstraints(int color) const {
		return colors[color].getConstraints();
	}

	std::vector<int> Variable::getNeighbors() const {
		std::set<int> neighbors;
		for(int color = 0; color < 3; color++) {
			std::vector<int> colorNeighbors = colors[color].getNeighbors();
			neighbors.insert(colorNeighbors.begin(), colorNeighbors.end());
		}
		return std::vector<int>(neighbors.begin(), neighbors.end());
	}

	bool Variable::isColorAvailable(int color) const {
		return colors[color].getIsAvailable();
	}

	std::vector<int> Variable::getAvailableColors() const {
		std::vector<int> availableColors;
		for(int color = 0; color < 4; color++) {
			if(colors[color].getIsAvailable()) {
				availableColors.push_back(color);
			}
		}
		return availableColors;
	}

	void Variable::disableColor(int color) {
		colors[color].disable();
	}

	void Variable::setColor(int color) {
		for(int disabledColor = 0; disabledColor < 4; disabledColor++) {
			if(disabledColor == color) {
				continue;
			}
			disableColor(disabledColor);
		}
	}
}
