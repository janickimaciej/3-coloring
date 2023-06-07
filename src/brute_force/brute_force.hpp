#ifndef BRUTE
#define BRUTE

#include "../instance_representation/graph.hpp"

class BruteForce
{
private:
	int n;

	void giveColor(Graph* g, int v, int color);

	int* colors;
	bool checkCombination(Graph* g);
public:
	BruteForce(Graph* graph);
	bool Solve();
	Graph* graph;
};

#endif // !BRUTE
