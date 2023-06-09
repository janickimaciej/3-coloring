#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "instance_representation/graph.hpp"
#include "csp/csp_solver.hpp"
#include "graph6/G6Parser.hpp"
#include "instance_reduction/cycle_reduction.hpp"
#include "instance_reduction/low_degree_reduction.hpp"
#include <vector>
#include "brute_force/brute_force.hpp"
#include "coloring/coloring.hpp"
#include <chrono>
#include <string>
#include <windows.h>
#include <cstdlib>

using namespace std;
using namespace std::chrono;

void K();
void M();
std::wstring strToWstr(std::string str);
std::string wstrToStr(std::wstring wstr);

int main() {
	
	K();
	//M();
}

void K() {
	G6Parser parser;

	vector<string> files;
	vector<int> edges;
	/*files.push_back("C:\\Users\\User\\Desktop\\Graphs\\n5e7.g6");
	edges.push_back(7);
	files.push_back("C:\\Users\\User\\Desktop\\Graphs\\n7e16_not3.g6");
	edges.push_back(16);
	files.push_back("C:\\Users\\User\\Desktop\\Graphs\\n11e32.g6");
	edges.push_back(32);
	files.push_back("C:\\Users\\User\\Desktop\\Graphs\\n21e59.g6");
	edges.push_back(56);
	files.push_back("C:\\Users\\User\\Desktop\\Graphs\\n31e62.g6");
	edges.push_back(62);*/
	files.push_back("n31e62.g6");
	edges.push_back(62);
	files.push_back("n103e1133.g6");
	edges.push_back(1133);
	files.push_back("n64e672c4.g6");
	edges.push_back(672);

	/*files.push_back("n250e375c3.g6");
	edges.push_back(375);*/

	files.push_back("n12e22.g6");
	edges.push_back(22);

	files.push_back("n11e19.g6");
	edges.push_back(19);

	//.push_back("sierpinski.g6");
	//edges.push_back(375);


	files.push_back("n250e375c3.g6");
	edges.push_back(672);



	Graph* g;
	Graph* copy;
	for (int i = 0; i < files.size(); i++)
	{
		parser.openFile(files[i]);
		Graph* g = parser.parse();

		cout << "n" << g->getVertexCount() << " e" << edges[i] << "\n";

		Coloring coloring(Graph::copy(g));
		auto start = high_resolution_clock::now();
		cout << coloring.Solve() << "\n";
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<seconds>(stop - start);
		if (duration.count() < 5)
		{
			auto durationMs = duration_cast<microseconds>(stop - start);
			cout << "Our in " << durationMs.count() / 1000000.0 << " seconds\n";
		}
		else
		{
			cout << "Our in " << duration.count() << " s\n";
		}
		cout << "\n";
		/*BruteForce brute(Graph::copy(g));
		auto startB = high_resolution_clock::now();
		cout << brute.Solve() << "\n";
		auto stopB = high_resolution_clock::now();
		auto durationB = duration_cast<seconds>(stopB - startB);
		if (durationB.count() < 5)
		{
			auto durationMsB = duration_cast<microseconds>(stopB - startB);
			cout << "Brute in " << durationMsB.count() << " microseconds\n";
		}
		else
		{
			cout << "Brute in " << durationB.count() << " s\n";
		}

		cout << "\n\n";*/
	}
}

void M() {
	CSPSolver::beVerbose = true;
	G6Parser parser;

	std::string basePath("D:/Downloads/chr_3/");
	std::wstring pattern = strToWstr(basePath);
	pattern.append(L"*.g6");

	WIN32_FIND_DATA fd;
	HANDLE hFind = NULL;

	if((hFind = FindFirstFile(pattern.c_str(), &fd)) == INVALID_HANDLE_VALUE) {
		const char* errorMsg = "Invalid path";
		std::cerr << errorMsg << std::endl;
		throw errorMsg;
	}
	int count = 1;
	while(FindNextFile(hFind, &fd)) {
		count++;
	}

	hFind = FindFirstFile(pattern.c_str(), &fd);
	int progress = 1;
	do {
		std::string fileName(wstrToStr(std::wstring(fd.cFileName)));
		parser.openFile(basePath + fileName);
		Graph* g = parser.parse();

		cout << progress << "/" << count << " n" << g->getVertexCount() << " e" << g->getEdgeCount() << " " << fileName << endl;

		switch(CSPSolver::solve(dynamic_cast<CSPInstance*>(g))) {
		case Result::Success:
			cout << "Success" << endl;
			progress++;
			break;
		case Result::Failure:
			cout << "Failure!!!" << endl;
			return;
			break;
		}

		cout << endl;
	} while(FindNextFile(hFind, &fd));
}

std::wstring strToWstr(std::string str) {
	const char* cStr = str.c_str();
	size_t size = std::strlen(cStr);
	std::wstring wstr;
	wstr.resize(size);
	std::mbstowcs(&wstr[0], cStr, size);
	return wstr;
}

std::string wstrToStr(std::wstring wstr) {
	const wchar_t* cWstr = wstr.c_str();
	size_t size = wcslen(cWstr);
	size_t wSize = (wcslen(cWstr) + 1)*sizeof(wchar_t);
	std::string str;
	str.resize(size);
	std::wcstombs(&str[0], cWstr, wSize);
	return str;
}
