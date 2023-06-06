#ifndef BRUTE
#define BRUTE

#include "../instance_representation/graph.hpp"

class BruteForce
{
private:
	int n;

	bool tryRec(int v, Graph* copy);
	void giveColor(Graph* g, int v, int color);

public:
	BruteForce(Graph* graph);
	bool Solve();
	Graph* graph;
};

#endif // !BRUTE
