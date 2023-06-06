#include <iostream>
#include "instance_representation/graph.hpp"
#include "csp/csp_solver.hpp"
#include "graph6/G6Parser.hpp"
#include "instance_reduction/cycle_reduction.hpp"
#include "instance_reduction/low_degree_reduction.hpp"
#include <vector>
#include "brute_force/brute_force.hpp"
#include "coloring/coloring.hpp"

using namespace std;

void K() {
	G6Parser parser;
	//parser.openFile("C:\\Users\\User\\Desktop\\Graphs\\cycleTest.g6");
	//parser.openFile("C:\\Users\\User\\Desktop\\Graphs\\lowTest.g6");
	parser.openFile("C:\\Users\\User\\Desktop\\Graphs\\testDeleteAll.g6");
	
	Graph* g = parser.parse();
	cout << g->getVertexCount() << "\n";
	
	cout << "\nBrute\n";

	BruteForce brute(g);
	if (brute.Solve())
	{
		cout << "solved\n";
		for (int i = 0; i < brute.graph->getVertexCount(); i++)
		{
			cout << i << ": " << brute.graph->getAvailableColors(i).at(0) << "\n";
		}
	}
	else
	{
		cout << "not solved\n";
	}
	
	cout << "\nOur\n";

	Coloring coloring(g);
	if (coloring.Solve())
	{
		cout << "solved\n";
		for (int i = 0; i < coloring.graph->getVertexCount(); i++)
		{
			cout << i << ": " << coloring.graph->getAvailableColors(i).at(0) << "\n";
		}
	}
	else
	{
		cout << "not solved\n";
	}
}

void M() {
	Graph* G1 = Graph::create(4);
	G1->addEdge(0, 1);
	G1->addEdge(0, 2);
	G1->addEdge(0, 3);
	
	cout << (bool)CSPSolver::solve(dynamic_cast<CSPInstance*>(G1)) << endl;
	delete G1;
}

int main() {
	//K();
	M();
}
