#ifndef GRAPH
#define GRAPH

#include <vector>

class Graph {
public:
	static Graph* createGraph(int vertices);

	virtual int getVertexCount() const = 0;
	virtual std::vector<int> getNeighbors(int vertex) const = 0;
	virtual void removeVertex(int vertex) = 0;
	
	virtual bool hasEdge(int start, int end) = 0;
	virtual void addEdge(int start, int end) = 0;
	virtual void removeEdge(int start, int end) = 0;
};

#endif
