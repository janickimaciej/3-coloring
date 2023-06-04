#include "constraint_info.hpp"

namespace InnerRepresentation {
	ConstraintInfo::ConstraintInfo(int startVariable, int startColor, int endVariable, int endColor) :
		startVariable(startVariable), startColor(startColor), endVariable(endVariable), endColor(endColor) { }
}
