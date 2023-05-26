#ifndef GRAPH
#define GRAPH

class Graph {
public:
	virtual Graph CreateGraph(int vertices) = 0;
	virtual void AddEdge(int start, int end) = 0;
};

#endif
