#include <iostream>
#include "instance_representation/graph.hpp"
#include "csp/csp_solver.hpp"

using namespace std;

int main() {
	Graph* G1 = Graph::create(4);
	G1->addEdge(0, 1);
	G1->addEdge(0, 2);
	G1->addEdge(0, 3);
	
	cout << (bool)CSPSolver::solve(dynamic_cast<CSPInstance*>(G1)) << endl;
	delete G1;
}
