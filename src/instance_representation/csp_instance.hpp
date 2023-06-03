#ifndef CSP_INSTANCE
#define CSP_INSTANCE

#include <vector>
#include "color_pair.hpp"

class CSPInstance {
public:
	static CSPInstance* create(int variables);
	static CSPInstance* copy(const CSPInstance* cspInstance);
	virtual void copyColoring(const CSPInstance* cspInstance, std::vector<int> removedVariables) = 0;

	virtual int getVariableCount() const = 0;
	virtual std::vector<ColorPair> getConstraints(int variable, int color) const = 0;
	virtual void addVariable() = 0;
	virtual void removeVariable(int variable) = 0;
	
	virtual bool hasConstraint(int startVariable, int startColor, int endVariable, int endColor) const = 0;
	virtual void addConstraint(int startVariable, int startColor, int endVariable, int endColor) = 0;
	virtual void removeConstraint(int startVariable, int startColor, int endVariable, int endColor) = 0;

	virtual bool isColorAvailable(int variable, int color) const = 0;
	virtual std::vector<int> getAvailableColors(int variable) const = 0;
	virtual void disableColor(int variable, int color) = 0;
	virtual void setColor(int variable, int color) = 0;
};

#endif
