#ifndef CSP_INSTANCE
#define CSP_INSTANCE

#include "constraint_info.hpp"

class CSPInstance {
public:
	virtual int getVariableCount() const = 0;

	virtual void addConstraint(const ConstraintInfo& constraint) = 0;
};

#endif
