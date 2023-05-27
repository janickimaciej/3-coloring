#ifndef INNER_CSP_INSTANCE
#define INNER_CSP_INSTANCE

#include "csp_instance.hpp"
#include "graph.hpp"
#include "variable.hpp"
#include <vector>

class InnerCSPInstance : public Graph, public CSPInstance {
	std::vector<Variable> variables;
public:
	InnerCSPInstance(int vertices);
	InnerCSPInstance(const InnerCSPInstance& cspInstance);

	// CSP interface
	virtual int getVariableCount() const override;

	// Graph interface
	virtual int getVertexCount() const override;
	virtual std::vector<int> getNeighbors(int vertex) const override;
	virtual void removeVertex(int vertex) override;
	
	virtual bool hasEdge(int start, int end) override;
	virtual void addEdge(int start, int end) override;
	virtual void removeEdge(int start, int end) override;
};

#endif
