#include <iostream>
#include "instance_representation/graph.hpp"
#include "graph6/G6Parser.hpp"

using namespace std;

int main() {
	Graph* G = Graph::createGraph(10);
	cout << G->hasEdge(3, 5) << endl;
	G->addEdge(3, 5);
	cout << G->hasEdge(3, 5) << endl;
	G->removeEdge(3, 5);
	cout << G->hasEdge(3, 5) << endl;

	G6Parser parser;
	parser.openFile();
}
