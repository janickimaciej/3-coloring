#ifndef COLOR
#define COLOR

#include "../instance_reduction/initial_reduction.hpp"
#include "../instance_representation/graph.hpp"
#include "../instance_representation/instance.hpp"
#include "../forests/bushy_forests.hpp"
#include "../csp/csp_solver.hpp"

class Coloring
{
	std::vector<Instance*>* instances;
	Graph* graph;
	Instance* instance;
	int n;

	bool tryToColor(int inst);
	bool colorForest(int v, Graph* copy);
	void copyColoring();
	void unMerge(int v, std::vector<int>* unmerged);
	bool checkRest(Graph* copy);

public:
	Coloring(Graph* graph);
	bool Solve();

};

#endif // !COLOR

