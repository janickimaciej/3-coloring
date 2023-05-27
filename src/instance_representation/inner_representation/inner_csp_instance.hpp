#ifndef INNER_CSP_INSTANCE
#define INNER_CSP_INSTANCE

#include "../csp_instance.hpp"
#include "../graph.hpp"
#include "variable.hpp"
#include <vector>

class InnerCSPInstance : public Graph, public CSPInstance {
	std::vector<Variable*> variables;
public:
	InnerCSPInstance(int vertices);
	InnerCSPInstance(const InnerCSPInstance& cspInstance);

	void error(const char* errorMsg) const;

	~InnerCSPInstance();

	// CSP interface
	virtual int getVariableCount() const override;
	virtual std::vector<ColorPair> getConstraints(int variable, int color) const;
	virtual void removeVariable(int variable) override;
	
	virtual bool hasConstraint(int startVariable, int startColor, int endVariable, int endColor) const override;
	virtual void addConstraint(int startVariable, int startColor, int endVariable, int endColor) override;
	virtual void removeConstraint(int startVariable, int startColor, int endVariable, int endColor) override;

	// Graph interface
	virtual int getVertexCount() const override;
	virtual std::vector<int> getNeighbors(int vertex) const override;
	virtual void removeVertex(int vertex) override;
	
	virtual bool hasEdge(int start, int end) override;
	virtual void addEdge(int start, int end) override;
	virtual void removeEdge(int start, int end) override;

	// Common interface
	virtual bool isColorAvailable(int variable, int color) const override;
	virtual std::vector<int> getAvailableColors(int variable) const override;
	virtual void disableColor(int variable, int color) override;
	virtual void setColor(int variable, int color) override;
};

#endif
