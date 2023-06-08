#ifndef G6PARSER
#define G6PARSER

#include <fstream>
#include "../instance_representation/graph.hpp"

class G6Parser
{
private:
	char curr;
	std::string path;
	std::fstream file;
	Graph* graph;
	const int mask = 0b10000000;
	void createGraph();
	void fillEdges();
public:
	void openFile();
	void openFile(std::string path);
	Graph* parse();
	static void parseBack(Graph* g);
};

#endif
