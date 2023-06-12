#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <windows.h>
#include <cstdlib>
#include "instance_representation/graph.hpp"
#include "csp/csp_solver.hpp"
#include "graph6/G6Parser.hpp"
#include "instance_reduction/cycle_reduction.hpp"
#include "instance_reduction/low_degree_reduction.hpp"
#include "brute_force/brute_force.hpp"
#include "coloring/coloring.hpp"
#include "test/test.hpp"

using namespace std;
using namespace std::chrono;

void K();
void M();
std::wstring strToWstr(std::string str);
std::string wstrToStr(std::wstring wstr);

int main() {
	
	//K();
	M();
}

void K() {
	G6Parser parser;

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
	//files.push_back("n31e62.g6");
	//files.push_back("n103e1133.g6");
	//files.push_back("n64e672c4.g6");

	//files.push_back("n250e375c3.g6");

	//files.push_back("n12e22.g6");

	//files.push_back("n11e19.g6");

	//files.push_back("sierpinski.g6");

	//files.push_back("graph_746.g6");
	//files.push_back("graph_958.g6");

	// 34 VERTICES, DOABLE!!
	files.push_back("chr_e3/graph_1266.g6");
	files.push_back("chr_e3/graph_49392.g6");
	files.push_back("chr_e3/graph_49416.g6");
	files.push_back("chr_e3/graph_49415.g6");
	files.push_back("chr_e3/graph_49417.g6");
	 
	/*parser.openFile("testNewCycleLinked.g6");
	Graph* g = parser.parse();
	Instance* inst = new Instance(g);
	std::vector<Instance*> instances;
	instances.push_back(inst);
	CycleReduction cycle(&instances);
	cycle.Reduce();
	for (int i = 0; i < instances.size(); i++)
	{
		G6Parser::parseBack(instances[i]->graph);
	}*/
	 
	
	//files.push_back("graph_30567.g6");
	
	
	// 124 ver
	//files.push_back("graph_1136.g6");


	//files.push_back("n250e375c3.g6");

	for (int i = 0; i < files.size(); i++)
	{
		parser.openFile(files[i]);
		Graph* g = parser.parse();

		cout << "n" << g->getVertexCount() << " e" << g->getEdgeCount() << "\n";

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
		cout << "\n";
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
	Test::run("chr_e3/", Result::Success, "results.txt");
	//Test::run("chr_ge4/", Result::Failure);
}
