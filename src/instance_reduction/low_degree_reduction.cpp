#include "low_degree_reduction.hpp"
#include <iostream>

LowReduction::LowReduction(Instance* g)
{
	this->g = g;
	n = g->graph->getVertexCount();
	toDeletion = new bool[n];
	degrees = new int[n];
	neighbours = new std::vector<int>[n];
	for (int i = 0; i < n; i++)
	{
		toDeletion[i] = false;
		neighbours[i] = g->graph->getNeighbors(i);
		degrees[i] = neighbours[i].size();
	}
	hasReduced = false;
}

void LowReduction::Set(Instance* g)
{
	this->g = g;
	Update();
}

LowReduction::~LowReduction()
{
	delete[] toDeletion;
	delete[] degrees;
	delete[] neighbours;
}

bool LowReduction::Reduce()
{
	hasReduced = false;
	for (int i = 0; i < n; i++)
	{
		if (degrees[i] < 3 && degrees[i] > 0)
		{
			hasReduced = true;
			ReduceRec(i);
		}
	}
	for (int i = n - 1; i >= 0; i--)
	{
		if (toDeletion[i])
		{
			g->removeVertex(i);
		}
	}
	return hasReduced;
}

void LowReduction::Update()
{
	delete[] toDeletion;
	delete[] degrees;
	delete[] neighbours;

	n = g->graph->getVertexCount();
	toDeletion = new bool[n];
	degrees = new int[n];
	neighbours = new std::vector<int>[n];
	for (int i = 0; i < n; i++)
	{
		toDeletion[i] = false;
		neighbours[i] = g->graph->getNeighbors(i);
		degrees[i] = neighbours[i].size();
	}
}

void LowReduction::ReduceRec(int v)
{
	toDeletion[v] = true;
	std::cout << v << " ";
	
	for (int i = 0; i < degrees[v]; i++)
	{
		degrees[neighbours[v][i]]--;
	}
	for (int i = 0; i < degrees[v]; i++)
	{
		if (degrees[neighbours[v][i]] < 3 && degrees[neighbours[v][i] > 0]) ReduceRec(neighbours[v][i]);
	}
}