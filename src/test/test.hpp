#ifndef TEST
#define TEST

#include <string>
#include "../csp/csp_solver.hpp"

class Test {
public:
	Test() = delete;
	~Test() = delete;
	static void run(std::string directoryPath, Result expectedResult, std::string resultPath = "", bool solveAsCSP = false,
		bool displayLemmas = false);
};

#endif
