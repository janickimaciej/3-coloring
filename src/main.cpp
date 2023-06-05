#include <iostream>
#include "instance_representation/graph.hpp"
#include "csp/csp_solver.hpp"
#include "graph6/G6Parser.hpp"
#include "instance_reduction/cycle_reduction.hpp"
#include "instance_reduction/low_degree_reduction.hpp"
#include <vector>

using namespace std;

int main() {
	G6Parser parser;
	//parser.openFile("C:\\Users\\User\\Desktop\\Graphs\\cycleTest.g6");
	//parser.openFile("C:\\Users\\User\\Desktop\\Graphs\\lowTest.g6");
	parser.openFile("C:\\Users\\User\\Desktop\\Graphs\\threesTest.g6");
	

	Graph* g = parser.parse();
	cout << g->getVertexCount() << "\n";
	
	/*LowReduction low_red(g);
	low_red.Reduce();

	cout << "\n";
	for (int i = 0; i < g->getVertexCount(); i++)
	{
		cout << i << ": ";
		for (int v : g->getNeighbors(i))
		{
			cout << v << " ";
		}
		cout << "\n";
	}*/
	
	std::vector<Graph*> instances;
	instances.push_back(g);
	cout << instances.size() << "\n";
	CycleReduction cycle(&instances);
	cout << instances.size() << "\n";
	
	Graph* G1 = Graph::create(4);
	G1->addEdge(0, 1);
	G1->addEdge(0, 2);
	G1->addEdge(0, 3);
	
	cout << (bool)CSPSolver::solve(dynamic_cast<CSPInstance*>(G1)) << endl;
	delete G1;
}
