#include "coloring.hpp"
#include <iostream>


Coloring::Coloring(Graph* graph, bool show)
{
	this->show = show;
	this->graph = graph;
	this->instances = nullptr;
	this->instance = nullptr;
	n = 0;
	solved = false;
}

bool Coloring::Solve()
{
	InitialReduction initial(Graph::copy(graph));
	instances = initial.Reduce();
	if (instances->size() == 0) return false;
	BushyForest forest(instances);
	forest.PrepareAll();
	Result result;
	for (int i = 0; i < instances->size(); i++)
	{
		if (tryToColor(i))
		{
			return true;
		}
	}
	return solved;
}


bool Coloring::tryToColor(int inst)
{
	n = graph->getVertexCount();
	instance = instances->at(inst);
	return colorForest(0, instance);
}

bool Coloring::colorForest(int v, Instance* copy)
{
	if (v == copy->innerTree.size())
	{
		Instance* inst = Instance::copy(copy);
		Result result = CSPSolver::solve(dynamic_cast<CSPInstance*>(inst->graph));
		if (result == Result::Success)
		{
			instance = inst;
			Graph* g = inst->original;
			copyColoring(g);
			checkRest(g, copy);
			return true;
		}
		else return false;
	}
	else
	{
		Instance* inst;
		int ver = copy->innerTree[v];
		for (int color : copy->graph->getAvailableColors(ver))
		{
			inst = Instance::copy(copy);
			try
			{
				Instance::giveColor(inst->graph, ver, color);
			}
			catch (std::invalid_argument ex)
			{
				return false;
			}
			if (colorForest(v + 1, inst)) return true;
			delete inst;
		}
		return false;

	}
}

void Coloring::copyColoring(Graph* g)
{
	for (int i = 0; i < instance->graph->getVertexCount(); i++)
	{
		int color = instance->graph->getAvailableColors(i).at(0);
		if (instance->indexes[i] < n)
		{
			Instance::giveColor(g, instance->indexes[i], color);
		}
		else
		{
			std::vector<int> unmerged = *instance->unMerge(instance->indexes[i], n);
			for (int ver : unmerged)
			{
				Instance::giveColor(g, ver, color);
			}
		}

	}
}


bool Coloring::checkRest(Graph* copy, Instance* copyInst)
{
	int ver;
	int b = 3;
	for (int i = copyInst->deleted.size() - 1; i >= 0; i--)
	{
		ver = copyInst->deleted[i];
		int l = 5;
		if (ver < n)
		{
			if (!copyInst->rules[ver]->apply(copyInst, copy))
			{
				std::vector<int> av = copy->getAvailableColors(ver);
				Instance::giveNaive(copy, ver);
				std::vector<int> av2 = copy->getAvailableColors(ver);
				int b = 3;
			}
		}
		else
		{
			std::vector<int> unMerged = *copyInst->unMerge(ver, n);
			int color = copyInst->getMergedColor(copy, unMerged);
			int color2 = copy->getAvailableColors(ver).at(0);
			if (color != color2)
			{
				int o = 9;
			}
			Instance::giveColor(copy, ver, color);
			for (int mer : unMerged)
			{
				if (!copyInst->rules[mer]->apply(copyInst, copy))
				{
					Instance::giveColor(copy, mer, color);
				}
			}
		}
	}
	if (show) showColoring(copy);
	graph = copy;
	return true;
}

void Coloring::showColoring(Graph* g)
{
	for (int i = 0; i < instance->originalN; i++)
	{
		std::cout << i << ": " << g->getAvailableColors(i).at(0) << "\n";
	}
	std::cout << "\n";
}
