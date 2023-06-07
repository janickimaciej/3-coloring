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

void K();
void M();

int main() {
	//K();
	M();
}

void K() {
	G6Parser parser;

	/*parser.openFile("cycleEvil.g6");
	Graph* h = parser.parse();
	std::vector<Instance*> instances;
	instances.push_back(new Instance(h));
	CycleReduction cyc(&instances);
	cyc.Reduce();*/


	//parser.openFile("C:\\Users\\User\\Desktop\\Graphs\\bruteCheck.g6");
	vector<string> files;
	vector<int> edges;
	/*files.push_back("C:\\Users\\User\\Desktop\\Graphs\\n5e7.g6");
	edges.push_back(7);
	files.push_back("C:\\Users\\User\\Desktop\\Graphs\\n7e16_not3.g6");
	edges.push_back(16);
	files.push_back("C:\\Users\\User\\Desktop\\Graphs\\n11e32.g6");
	edges.push_back(32);
	files.push_back("C:\\Users\\User\\Desktop\\Graphs\\n21e59.g6");
	edges.push_back(56);
	files.push_back("C:\\Users\\User\\Desktop\\Graphs\\n31e62.g6");
	edges.push_back(62);*/
	files.push_back("n31e62.g6");
	edges.push_back(62);
	files.push_back("n103e1133.g6");
	edges.push_back(1133);
	files.push_back("n64e672c4.g6");
	edges.push_back(672);
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
			cout << "Our in " << durationMs.count() / 1000000.0 << " seconds\n";
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
	G6Parser parser;

	vector<string> files;
	vector<int> edges;
	
	files.push_back("D:\\Downloads\\n5e7.g6");
	edges.push_back(7);
	files.push_back("D:\\Downloads\\n11e32.g6");
	edges.push_back(32);
	files.push_back("D:\\Downloads\\n21e59.g6");
	edges.push_back(59);
	files.push_back("D:\\Downloads\\n31e62.g6");
	edges.push_back(62);
	files.push_back("D:\\Downloads\\n12e24F.g6");
	edges.push_back(24);
	files.push_back("D:\\Downloads\\n4e6F.g6");
	edges.push_back(6);

	Graph* g;
	Graph* copy;
	for(int i = 0; i < files.size(); i++) {
		parser.openFile(files[i]);
		Graph* g = parser.parse();

		cout << "n" << g->getVertexCount() << " e" << edges[i] << "\n";

		switch(CSPSolver::solve(dynamic_cast<CSPInstance*>(g))) {
		case Result::Success:
			cout << "Success" << endl;
			break;
		case Result::Failure:
			cout << "Failure" << endl;
			break;
		}

		cout << endl;
	}
}
