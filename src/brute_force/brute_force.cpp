#include "brute_force.hpp"
#include <iostream>


bool BruteForce::checkCombination(Graph* g)
{
    for (int i = 0; i < n; i++)
    {
        int a = 5;
        if (g->isColorAvailable(i, colors[i]))
        {
            giveColor(g, i, colors[i]);
        }
        else
        {
            return false;
        }
    }
    return true;
}

BruteForce::BruteForce(Graph* graph)
{
    this->graph = graph;
    n = graph->getVertexCount();
    colors = new int[n];
    for (int i = 0; i < n; i++)
    {
        colors[i] = 0;
    }
}

bool BruteForce::Solve()
{
    Graph* copy;
    int index = 0;
    while (1)
    {
        copy = Graph::copy(graph);
        /*if (checkCombination(copy))
        {
            graph = copy; 
            return true;
        }*/
        /*for (int i = 0; i < n; i++)
        {
            std::cout << colors[i];
        }
        std::cout << "\n";*/
        checkCombination(copy);
        if (colors[index] == 2)
        {
			while (colors[index] == 2)
			{
                colors[index] = 0;
                index++;
                if (index == n)
                {
                    break;
                }
			}
            if (index == n) break;
            colors[index]++;
            index = 0;
        }
        else
        {
            colors[index]++;
        }
        delete copy;
    }
    return false;
}


void BruteForce::giveColor(Graph *g, int v, int color)
{
    g->setColor(v, color);
    for (int ver : g->getNeighbors(v))
    {
        if (g->isColorAvailable(ver, color))
        {
            g->disableColor(ver, color);
        }
    }
}
