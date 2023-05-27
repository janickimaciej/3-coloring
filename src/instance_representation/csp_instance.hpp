#ifndef CSP_INSTANCE
#define CSP_INSTANCE

class CSPInstance {
public:
	static CSPInstance* create(int variables);
	static CSPInstance* copy(CSPInstance* cspInstance);

	virtual int getVariableCount() const = 0;
	virtual void removeVariable(int vertex) = 0;
	
	virtual bool hasConstraint(int startVariable, int startColor, int endVariable, int endColor) const = 0;
	virtual void addConstraint(int startVariable, int startColor, int endVariable, int endColor) = 0;
	virtual void removeConstraint(int startVariable, int startColor, int endVariable, int endColor) = 0;
};

#endif
