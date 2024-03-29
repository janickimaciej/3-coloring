#ifndef COLORING
#define COLORING

#include "../instance_reduction/initial_reduction.hpp"
#include "../instance_representation/graph.hpp"
#include "../instance_representation/instance.hpp"
#include "../forests/bushy_forests.hpp"
#include "../csp/csp_solver.hpp"
#include "../instance_representation/rules/rule.hpp"

class Coloring
{
	bool show = true;

	std::vector<Instance*>* instances;
	Instance* instance;
	int n;
	bool solved;

	bool tryToColor(int inst);
	bool colorForest(int v, Instance* copy);
	void copyColoring(Graph* g);
	bool checkRest(Graph* copy, Instance* copyInst);
	void showColoring(Graph* g);

public:
	Graph* graph;
	Coloring(Graph* graph, bool show = false);
	bool Solve();

};

#endif
