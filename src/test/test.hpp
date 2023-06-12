#ifndef TEST
#define TEST

#include <string>
#include "../csp/csp_solver.hpp"

class Test {
public:
	Test() = delete;
	~Test() = delete;
	static void runFile(std::string filePath, bool solveAsCSP = false, bool showResult = false, bool displayLemmas = false);
	static void runDirectory(std::string directoryPath, Result expectedResult, std::string resultPath = "",
		bool solveAsCSP = false, bool showResult = false, bool displayLemmas = false);
};

#endif
