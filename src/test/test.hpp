#ifndef TEST
#define TEST

#include <string>
#include "../csp/csp_solver.hpp"

class Test {
public:
	Test() = delete;
	~Test() = delete;
	static void run(std::string path, Result expectedResult, bool beVerbose, std::string resultPath = "",
		bool solveAsCSP = false);
};

#endif
