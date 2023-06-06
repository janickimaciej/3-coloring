#ifndef BRUTE
#define BRUTE

#include "../instance_representation/graph.hpp"

class bruteForce
{
private:
	int n;

	bool tryRec(int v, Graph* copy);

public:
	bruteForce(Graph* graph);
	bool Solve();
	Graph* graph;
};

#endif // !BRUTE
