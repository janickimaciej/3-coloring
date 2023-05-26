#ifndef INNER_CSP_INSTANCE
#define INNER_CSP_INSTANCE

#include "csp_instance.hpp"
#include "graph.hpp"

class InnerCSPInstance : public Graph, public CSPInstance {
	int vertices;
public:
	InnerCSPInstance(int vertices);
	virtual void AddEdge(int start, int end) override;
};

#endif
