#ifndef CONSTRAINT_INFO
#define CONSTRAINT_INFO

struct ConstraintInfo {
	int startVariable;
	int startColor;
	int endVariable;
	int endColor;

	ConstraintInfo(int startVariable, int startColor, int endVariable, int endColor);
};

#endif
