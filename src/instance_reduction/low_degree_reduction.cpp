#include "low_degree_reduction.hpp"
#include <iostream>

LowReduction::LowReduction(Instance* g)
{
	this->g = g;
	n = g->graph->getVertexCount();
	toDeletion = new bool[n];
	visited = new bool[n];
	degrees = new int[n];
	neighbours = new std::vector<int>[n];
	for (int i = 0; i < n; i++)
	{
		toDeletion[i] = false;
		visited[i] = false;
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
	delete[] visited;
}

bool LowReduction::Reduce()
{
	hasReduced = false;
	for (int i = 0; i < n; i++)
	{
		if (degrees[i] < 3 && !visited[i])
		{
			hasReduced = true;
			ReduceRec(i);
		}
	}
	for (int i = n - 1; i >= 0; i--)
	{
		if (toDeletion[i])
		{
			g->justRemove(i);
		}
	}
	return hasReduced;
}

void LowReduction::Update()
{
	delete[] toDeletion;
	delete[] degrees;
	delete[] neighbours;
	delete[] visited;

	n = g->graph->getVertexCount();
	toDeletion = new bool[n];
	visited = new bool[n];
	degrees = new int[n];
	neighbours = new std::vector<int>[n];
	for (int i = 0; i < n; i++)
	{
		toDeletion[i] = false;
		visited[i] = false;
		neighbours[i] = g->graph->getNeighbors(i);
		degrees[i] = neighbours[i].size();
	}
}

void LowReduction::ReduceRec(int v)
{
	if (visited[v]) return;
	visited[v] = true;
	toDeletion[v] = true;
	g->deleted.push_back(g->indexes[v]);
	
	for (int i = 0; i < neighbours[v].size(); i++)
	{
		degrees[neighbours[v][i]]--;
	}
	for (int i = 0; i < neighbours[v].size(); i++)
	{
		if (degrees[neighbours[v][i]] < 3 && !visited[neighbours[v][i]]) ReduceRec(neighbours[v][i]);
	}
}