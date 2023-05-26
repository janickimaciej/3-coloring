#ifndef GRAPH
#define GRAPH

class Graph {
public:
	static Graph* CreateGraph(int vertices);
	virtual int GetVertexCount() = 0;
	
	virtual bool HasEdge(int start, int end) = 0;
	virtual void AddEdge(int start, int end) = 0;
	virtual void RemoveEdge(int start, int end) = 0;
};

#endif
