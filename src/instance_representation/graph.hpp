#ifndef GRAPH
#define GRAPH

class Graph {
public:
	static Graph* CreateGraph(int vertices);
	virtual void AddEdge(int start, int end) = 0;
};

#endif
