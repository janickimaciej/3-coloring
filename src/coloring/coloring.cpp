#include "coloring.hpp"
#include <iostream>


Coloring::Coloring(Graph* graph)
{
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
	BushyForest forest(instances);
	forest.PrepareAll();
	Rule* rule = instances->at(0)->rules[2];
	Result result;
	for (int i = 0; i < instances->size(); i++)
	{
		if (tryToColor(i)) return true;
	}
	//return false;
	return solved;
}

bool Coloring::tryToColor(int inst)
{
	n = graph->getVertexCount();
	instance = instances->at(inst);
	return colorForest(0,instance);
}

bool Coloring::colorForest(int v, Instance* copy)
{
	if (v == copy->innerTree.size())
	{
		Instance* inst = Instance::copy(copy);
		/*for (int bark : inst->innerTree)
		{
			inst->removeVertex(bark);
		}*/
		Result result = CSPSolver::solve(dynamic_cast<CSPInstance*>(inst->graph));
		if (result == Result::Success)
		{
			instance = inst;
			for (int i = 0; i < n; i++)
			{
				std::cout << i << ": " << instance->graph->getAvailableColors(i).at(0) << "\n";
			}
			copyColoring();
			checkRest(graph, copy);
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
			Instance::giveColor(inst->graph, ver, color);
			if (colorForest(v + 1, inst)) return true;
		}
		return false;
		
	}
}

void Coloring::copyColoring()
{
	for (int i = 0; i < instance->graph->getVertexCount(); i++)
	{
		if (instance->indexes[i] < n)
		{
			int amount = instance->graph->getAvailableColors(instance->indexes[i]).size();
			int color = instance->graph->getAvailableColors(instance->indexes[i]).at(0);
			Instance::giveColor(graph, instance->indexes[i], color);
		}
		else
		{
			std::vector<int> unmerged = *instance->unMerge(instance->indexes[i],n);
			int color = graph->getAvailableColors(unmerged[0]).at(0);
			for (int ver : unmerged)
			{
				Instance::giveColor(graph, ver, color);
			}
		}
		
	}
}


bool Coloring::checkRest(Graph* copy, Instance* copyInst)
{
	int ver;
	for (int i = copyInst->deleted.size() - 1; i >= 0;  i--)
	{
		ver = copyInst->deleted[i];
		if (ver < n)
		{
			Rule* rul = copyInst->rules[ver];
			Rule* rule = instances->at(0)->rules[2];
			copyInst->rules[ver]->apply(copyInst, graph);
			if (copyInst->rules[ver]->apply(copyInst, graph))
			{
				continue;
			}
			if (copy->getAvailableColors(ver).size() > 0)
			{
				Instance::giveColor(copy, ver, copy->getAvailableColors(ver).at(0));
			}
			else return false;
		}
		else
		{
			std::vector<int> unMerged = *copyInst->unMerge(ver, n);
			for (int mer : unMerged)
			{
				if (copyInst->rules[mer]->apply(copyInst, graph))
				{
					continue;
				}
				if (copy->getAvailableColors(mer).size() > 0)
				{
					Instance::giveColor(copy, mer, copy->getAvailableColors(mer).at(0));
				}
				else return false;
			}
		}
	}
	graph = copy;
	return true;
}