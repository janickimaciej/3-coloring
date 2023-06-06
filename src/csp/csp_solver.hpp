#ifndef CSP_SOLVER
#define CSP_SOLVER

#include "../instance_representation/csp_instance.hpp"
#include "result.hpp"

class CSPSolver;

typedef Result(* Lemma)(CSPInstance*);

class CSPSolver {
	static const int lemmasCount = 8;
	static const Lemma lemmas[lemmasCount];

	static Result lemma1(CSPInstance* cspInstance);
	static void lemma1Match(const CSPInstance* cspInstance, int& v, int& R, int& G);
	static void lemma1Reduce(CSPInstance* reduced, int v, int R, int G);
	static void lemma1Color(CSPInstance* cspInstance, const CSPInstance* reduced, int v, int R, int G);

	static Result lemma2(CSPInstance* cspInstance);
	static void lemma2Match(const CSPInstance* cspInstance, ColorPair& vX, ColorPair& wY);
	static void lemma2Reduce(CSPInstance* reduced, int v, int w);
	static void lemma2Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vX, const ColorPair& wY);

	static Result lemma3(CSPInstance* cspInstance);
	static void lemma3Match(const CSPInstance* cspInstance, ColorPair& vB);
	static void lemma3Reduce(CSPInstance* cspInstance, const ColorPair& vB);

	static Result lemma4(CSPInstance* cspInstance);
	static void lemma4Match(const CSPInstance* cspInstance, ColorPair& vR);
	static void lemma4Reduce(CSPInstance* reduced, int v);
	static void lemma4Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR);

	static Result lemma5(CSPInstance* cspInstance);
	static void lemma5Match(const CSPInstance* cspInstance, ColorPair& vR);
	static void lemma5Reduce(CSPInstance* cspInstance, const ColorPair& vR);

	static Result lemma6(CSPInstance* cspInstance);
	static void lemma6Match(const CSPInstance* cspInstance, ColorPair& vR, ColorPair& wR);
	static Result lemma6Case1(CSPInstance* cspInstance, const ColorPair& vR, const ColorPair& wR);
	static void lemma6Case1Reduce(CSPInstance* reduced, const ColorPair& vR, const ColorPair& wR);
	static void lemma6Case1Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR,
		const ColorPair& wR);
	static Result lemma6Case2(CSPInstance* cspInstance, const ColorPair& vR, const ColorPair& wR);
	static void lemma6Case2Branch1Reduce(CSPInstance* reduced, const ColorPair& vR, const ColorPair& wR);
	static void lemma6Case2Branch1Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR,
		const ColorPair& wR);
	static void lemma6Case2Branch2Reduce(CSPInstance* reduced, const ColorPair& vR, const ColorPair& wR);
	static void lemma6Case2Branch2Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR,
		const ColorPair& wR);

	static Result lemma7(CSPInstance* cspInstance);
	static void lemma7Match(const CSPInstance* cspInstance, ColorPair& vR, ColorPair& wR);
	static void lemma7Branch1Reduce(CSPInstance* reduced, const ColorPair& vR, const ColorPair& wR);
	static void lemma7Branch1Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR,
		const ColorPair& wR);
	static void lemma7Branch2Reduce(CSPInstance* reduced, const ColorPair& wR);
	static void lemma7Branch2Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR,
		const ColorPair& wR);

	static Result lemma8(CSPInstance* cspInstance);
public:
	CSPSolver() = delete;
	~CSPSolver() = delete;
	static Result solve(CSPInstance* cspInstance);
};

#endif
