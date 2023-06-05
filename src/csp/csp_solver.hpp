#ifndef CSP_SOLVER
#define CSP_SOLVER

#include "../instance_representation/csp_instance.hpp"
#include "result.hpp"

class CSPSolver;

typedef Result(* Lemma)(CSPInstance*);

class CSPSolver {
	static const int lemmasCount = 5;
	static const Lemma lemmas[lemmasCount];

	static Result lemma1(CSPInstance* cspInstance);
	static void lemma1Match(const CSPInstance* cspInstance, int& v);
	static void lemma1Reduce(CSPInstance* reduced, std::vector<int>& removedVariables, int& R, int& G, int v);
	static void lemma1Color(CSPInstance* cspInstance, const CSPInstance* reduced, std::vector<int>& removedVariables,
		int v, int R, int G);

	static Result lemma2(CSPInstance* cspInstance);
	static void lemma2Match(const CSPInstance* cspInstance, ColorPair& vX, ColorPair& wY);
	static void lemma2Reduce(CSPInstance* reduced, std::vector<int>& removedVariables, int v, int w);
	static void lemma2Color(CSPInstance* cspInstance, const CSPInstance* reduced, std::vector<int>& removedVariables,
		const ColorPair& vX, const ColorPair& wY);

	static Result lemma3(CSPInstance* cspInstance);
	static void lemma3Match(const CSPInstance* cspInstance, ColorPair& vB);
	static void lemma3Reduce(CSPInstance* cspInstance, const ColorPair& vB);

	static Result lemma4(CSPInstance* cspInstance);
	static void lemma4Match(const CSPInstance* cspInstance, ColorPair& vR);
	static void lemma4Reduce(CSPInstance* reduced, std::vector<int>& removedVariables, const ColorPair& vR);
	static void lemma4Color(CSPInstance* cspInstance, const CSPInstance* reduced, std::vector<int>& removedVariables,
		const ColorPair& vR);

	static Result lemma5(CSPInstance* cspInstance);
public:
	CSPSolver() = delete;
	~CSPSolver() = delete;
	static Result solve(CSPInstance* cspInstance);
};

#endif
