#include "G6Parser.hpp"
#include <iostream>
#include "G6Parser.hpp"

using namespace std;


void G6Parser::OpenFile()
{
	cout << "Input path to graph6 fie:\n";
	cin >> path;
	file.open(path, ios::in);
	while (!file)
	{
		cout << "Incorrect path, input another path:\n";
		cin >> path;
		file.open(path, ios::in);
	}
}

Graph* G6Parser::Parse()
{
	CreateGraph();
	FillEdges();
	return graph;
}

void G6Parser::CreateGraph()
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
					n << 8;
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
				if (i != 5) n << 8;
			}
		}
	}

	graph = Graph::CreateGraph(n);
}

void G6Parser::FillEdges()
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
			if (bits & mask) graph->AddEdge(i, j);
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