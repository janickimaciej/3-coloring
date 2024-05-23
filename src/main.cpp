#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <windows.h>
#include <cstdlib>
#include "instance_representation/graph.hpp"
#include "csp/csp_solver.hpp"
#include "graph6/G6Parser.hpp"
#include "instance_reduction/cycle_reduction.hpp"
#include "instance_reduction/low_degree_reduction.hpp"
#include "brute_force/brute_force.hpp"
#include "coloring/coloring.hpp"
#include "test/test.hpp"

int main() {
	Test::runDirectory("chr_e3/", Result::Success, "", false, true);
}
