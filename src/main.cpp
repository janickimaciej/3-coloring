#include <iostream>
#include "instance_representation/graph.hpp"
#include "csp/csp_solver.hpp"
#include "graph6/G6Parser.hpp"
#include "instance_reduction/cycle_reduction.hpp"
#include "instance_reduction/low_degree_reduction.hpp"
#include <vector>
#include "brute_force/brute_force.hpp"
#include "coloring/coloring.hpp"
#include <chrono>

using namespace std;
using namespace std::chrono;

void K() {
	G6Parser parser;
	//parser.openFile("C:\\Users\\User\\Desktop\\Graphs\\bruteCheck.g6");
	vector<string> files;
	vector<int> edges;
	files.push_back("C:\\Users\\User\\Desktop\\Graphs\\n5e7.g6");
	edges.push_back(7);
	files.push_back("C:\\Users\\User\\Desktop\\Graphs\\n7e16_not3.g6");
	edges.push_back(16);
	files.push_back("C:\\Users\\User\\Desktop\\Graphs\\n11e32.g6");
	edges.push_back(32);
	files.push_back("C:\\Users\\User\\Desktop\\Graphs\\n21e59.g6");
	edges.push_back(56);
	files.push_back("C:\\Users\\User\\Desktop\\Graphs\\n31e62.g6");
	edges.push_back(62);
	/*files.push_back("C:\\Users\\User\\Desktop\\Graphs\\n32e80.g6");
	edges.push_back(80);*/
	//parser.openFile("C:\\Users\\User\\Desktop\\Graphs\\n21e59.g6");

	/*Graph* g = parser.parse();
	cout << g->getVertexCount() << "\n";
	G6Parser::parseBack(g);*/

	Graph* g;
	Graph* copy;
	for (int i = 0; i < files.size(); i++)
	{
		parser.openFile(files[i]);
		Graph* g = parser.parse();

		cout << "n" << g->getVertexCount() << " e" << edges[i] << "\n";

		Coloring coloring(Graph::copy(g));
		auto start = high_resolution_clock::now();
		cout << coloring.Solve() << "\n";
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<seconds>(stop - start);
		if (duration.count() < 5)
		{
			auto durationMs = duration_cast<microseconds>(stop - start);
			cout << "Our in " << durationMs.count() << " microseconds\n";
		}
		else
		{
			cout << "Our in " << duration.count() << " s\n";
		}


		/*BruteForce brute(Graph::copy(g));
		auto startB = high_resolution_clock::now();
		cout << brute.Solve() << "\n";
		auto stopB = high_resolution_clock::now();
		auto durationB = duration_cast<seconds>(stopB - startB);
		if (durationB.count() < 5)
		{
			auto durationMsB = duration_cast<microseconds>(stopB - startB);
			cout << "Brute in " << durationMsB.count() << " microseconds\n";
		}
		else
		{
			cout << "Brute in " << durationB.count() << " s\n";
		}

		cout << "\n\n";*/
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
	K();
	//M();
}
