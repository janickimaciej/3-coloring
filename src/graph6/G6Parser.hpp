#ifndef PARSER
#define PARSER

#include<fstream>
#include "../instance_representation/graph.hpp"

class G6Parser
{
private:
	char curr;
	std::string path;
	std::fstream file;
	Graph* graph;
	const int mask = 0b10000000;
	void CreateGraph();
	void FillEdges();
public:
	G6Parser() {};
	void OpenFile();
	Graph* Parse();
};

#endif