#define _CRT_SECURE_NO_WARNINGS
#include "test.hpp"
#include <iostream>
#include <windows.h>
#include <fstream>
#include <chrono>
#include "../graph6/G6Parser.hpp"
#include "../coloring/coloring.hpp"

using namespace std;
using namespace chrono;

static wstring strToWstr(string str) {
	const char* cStr = str.c_str();
	size_t size = strlen(cStr);
	wstring wstr;
	wstr.resize(size);
	mbstowcs(&wstr[0], cStr, size);
	return wstr;
}

static string wstrToStr(wstring wstr) {
	const wchar_t* cWstr = wstr.c_str();
	size_t size = wcslen(cWstr);
	size_t wSize = (wcslen(cWstr) + 1)*sizeof(wchar_t);
	string str;
	str.resize(size);
	wcstombs(&str[0], cWstr, wSize);
	return str;
}

void Test::runFile(std::string filePath, bool solveAsCSP, bool displayLemmas) {
	CSPSolver::beVerbose = displayLemmas;
	G6Parser parser;

	parser.openFile(filePath);
	Graph* graph = parser.parse();

	int n = graph->getVertexCount();
	int m = graph->getEdgeCount();
	cout << "n" << n << " m" << m << " " << filePath << endl;

	auto start = high_resolution_clock::now();
	bool result;
	if(solveAsCSP) {
		result = (bool)CSPSolver::solve(dynamic_cast<CSPInstance*>(graph));
	} else {
		Coloring coloring(Graph::copy(graph));
		result = coloring.Solve();
	}
	auto stop = high_resolution_clock::now();
	auto durationMs = duration_cast<milliseconds>(stop - start);
	if(displayLemmas) {
		cout << endl;
	}
	if(result) {
		cout << "Success" << endl;
	} else {
		cout << "Failure" << endl;
	}
	cout << durationMs.count() << " ms" << endl << endl;
}

void Test::runDirectory(std::string directoryPath, Result expectedResult, std::string resultPath,
	bool solveAsCSP, bool displayLemmas) {
	CSPSolver::beVerbose = displayLemmas;
	G6Parser parser;

	ofstream outFile;
	if(resultPath != "") {
		outFile = ofstream(resultPath, ios::out);
	}

	wstring pattern = strToWstr(directoryPath);
	pattern.append(L"*.g6");

	WIN32_FIND_DATA findData;
	HANDLE findHandle = NULL;

	if((findHandle = FindFirstFile(pattern.c_str(), &findData)) == INVALID_HANDLE_VALUE) {
		const char* errorMsg = "Invalid path";
		cerr << errorMsg << endl;
		throw errorMsg;
	}
	int count = 1;
	while(FindNextFile(findHandle, &findData)) {
		count++;
	}
	FindClose(findHandle);

	bool testPassed = true;
	findHandle = FindFirstFile(pattern.c_str(), &findData);
	int progress = 1;
	do {
		string fileName(wstrToStr(wstring(findData.cFileName)));
		parser.openFile(directoryPath + fileName);
		Graph* graph = parser.parse();

		int n = graph->getVertexCount();
		int m = graph->getEdgeCount();
		cout << progress << "/" << count << " n" << n << " m" << m << " " << fileName << endl;

		auto start = high_resolution_clock::now();
		bool result;
		if(solveAsCSP) {
			result = (bool)CSPSolver::solve(dynamic_cast<CSPInstance*>(graph));
		} else {
			Coloring coloring(Graph::copy(graph));
			result = coloring.Solve();
		}
		auto stop = high_resolution_clock::now();
		if(result != (bool)expectedResult) {
			FindClose(findHandle);
			if(outFile.is_open()) {
				outFile.close();
			}
			const char* errorMsg = "Test failed";
			cerr << errorMsg;
			throw errorMsg;
		}
		auto durationMs = duration_cast<milliseconds>(stop - start);
		outFile << n << "\t" << m << "\t" << durationMs.count() << endl;
		if(displayLemmas) {
			cout << endl;
		}
		cout << durationMs.count() << " ms" << endl << endl;

		progress++;
	} while(FindNextFile(findHandle, &findData));
	FindClose(findHandle);
	if(outFile.is_open()) {
		outFile.close();
	}
}
