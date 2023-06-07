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
	Instance* instance;
	int n;
	bool solved;

	bool tryToColor(int inst);
	bool colorForest(int v, Graph* copy);
	void copyColoring();
	bool checkRest(Graph* copy);

public:
	Graph* graph;
	Coloring(Graph* graph);
	bool Solve();

};

#endif // !COLOR

