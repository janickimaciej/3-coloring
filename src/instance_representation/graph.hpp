#ifndef GRAPH
#define GRAPH

#include <vector>

class Graph {
public:
	static Graph* create(int vertices);
	static Graph* copy(const Graph* graph);
	virtual void copyColoring(const Graph* reducedGraph, std::vector<int> removedVertices) = 0;

	virtual int getVertexCount() const = 0;
	virtual std::vector<int> getNeighbors(int vertex) const = 0;
	virtual void addVertex() = 0;
	virtual void removeVertex(int vertex) = 0;
	
	virtual bool hasEdge(int start, int end) const = 0;
	virtual void addEdge(int start, int end) = 0;
	virtual void removeEdge(int start, int end) = 0;

	virtual bool isColorAvailable(int vertex, int color) const = 0;
	virtual std::vector<int> getAvailableColors(int vertex) const = 0;
	virtual void disableColor(int vertex, int color) = 0;
	virtual void setColor(int vertex, int color) = 0;
};

#endif
