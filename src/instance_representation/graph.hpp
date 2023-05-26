#ifndef GRAPH
#define GRAPH

class Graph {
public:
	static Graph* createGraph(int vertices);
	virtual int getVertexCount() = 0;
	
	virtual bool hasEdge(int start, int end) = 0;
	virtual void addEdge(int start, int end) = 0;
	virtual void removeEdge(int start, int end) = 0;
};

#endif
