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

void Test::run(string directoryPath, Result expectedResult, bool beVerbose, string resultPath,
	bool solveAsCSP) {
	CSPSolver::beVerbose = beVerbose;
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

		cout << progress << "/" << count << " n" << graph->getVertexCount() << " m" << graph->getEdgeCount() <<
			" " << fileName << endl;

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
		auto durationMs = duration_cast<microseconds>(stop - start);
		outFile << graph->getVertexCount() << "\t" << graph->getEdgeCount() << "\t" << durationMs.count() << endl;

		progress++;
		cout << endl << endl;
	} while(FindNextFile(findHandle, &findData));
	FindClose(findHandle);
	if(outFile.is_open()) {
		outFile.close();
	}
}
