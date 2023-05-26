#ifndef INNER_CSP_INSTANCE
#define INNER_CSP_INSTANCE

#include "csp_instance.hpp"
#include "graph.hpp"
#include "variable.hpp"

class InnerCSPInstance : public Graph, public CSPInstance {
	int variableCount;
	Variable* variables;
	Variable* head;
	Variable* tail;
public:
	InnerCSPInstance(int vertices);
	virtual int getVertexCount();
	
	virtual bool hasEdge(int start, int end);
	virtual void addEdge(int start, int end);
	virtual void removeEdge(int start, int end);
};

#endif
