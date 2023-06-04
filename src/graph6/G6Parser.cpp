#include "G6Parser.hpp"
#include <iostream>

using namespace std;

void G6Parser::openFile()
{
	cout << "Input path to graph6 file:\n";
	cin >> path;
	file.open(path, ios::in);
	while (!file)
	{
		cout << "Incorrect path, input another path:\n";
		cin >> path;
		file.open(path, ios::in);
	}
}

void G6Parser::openFile(string path) {
	file.open(path, ios::in);
	if(!file) {
		throw "G6Parser::openFile: Incorrect file path";
	}
}

Graph* G6Parser::parse()
{
	createGraph();
	fillEdges();
	return graph;
}

void G6Parser::createGraph()
{
	int n;

	file >> curr;

	if (curr != 126) // 0 <= n <= 62
	{
		n = curr - 63;
	}
	else
	{
		n = 0;
		file >> curr;
		if (curr != 126) // 63 <= n <= 258047
		{
			for (int i = 0; i < 3; i++)
			{
				n += curr - 63;
				if (i != 2)
				{
					n <<= 8;
					file >> curr;
				}
			}
		}
		else // 258048 <= n <= 68719476735
		{
			for (int i = 0; i < 6; i++)
			{
				file >> curr;
				n += curr - 63;
				if (i != 5) n <<= 8;
			}
		}
	}

	graph = Graph::create(n);
}

void G6Parser::fillEdges()
{
	int bits;
	int i = 0;
	int j = 1;
	while (1)
	{
		file >> curr;
		if (file.eof()) break;

		bits = (curr - 63) << 2;
		for (int k = 0; k < 6; k++)
		{
			if (bits & mask) graph->addEdge(i, j);
			bits <<= 1;
			i++;
			if (i == j)
			{
				j++;
				i = 0;
			}
		}
	}
}
