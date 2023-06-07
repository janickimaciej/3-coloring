#ifndef CSP_SOLVER
#define CSP_SOLVER

#include "../instance_representation/csp_instance.hpp"
#include <unordered_set>
#include "result.hpp"

class CSPSolver;

typedef Result(* Lemma)(CSPInstance*);

class CSPSolver {
	static const int lemmasCount = 18;
	static const Lemma lemmas[lemmasCount];

	static void chooseColorReduce(CSPInstance* reduced, const ColorPair& varCol);
	static void chooseColorColor(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& varCol);

	static Result lemma0(CSPInstance* cspInstance);
	static void lemma0Reduce(CSPInstance* reduced, const ColorPair& vR);
	static void lemma0Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR);

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
	static void lemma7Branch2Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& wR);

	static Result lemma8(CSPInstance* cspInstance);
	static void lemma8Match(const CSPInstance* cspInstance, ColorPair& vR, ColorPair& wR, ColorPair& wB, ColorPair& wG,
		bool& isImplication);
	static Result lemma8Case1(CSPInstance* cspInstance, const ColorPair& vR, const ColorPair& wR);
	static void lemma8Case1Branch1Reduce(CSPInstance* reduced, const ColorPair& vR, const ColorPair& wR);
	static void lemma8Case1Branch1Color(CSPInstance* cspInstance, const CSPInstance* reduced);
	static void lemma8Case1Branch2Reduce(CSPInstance* reduced, const ColorPair& wR);
	static void lemma8Case1Branch2Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& wR);
	static Result lemma8Case2(CSPInstance* cspInstance, const ColorPair& vR, const ColorPair& wB, const ColorPair& wG);
	static void lemma8Case2Branch1Reduce(CSPInstance* reduced, const ColorPair& wB, const ColorPair& wG);
	static void lemma8Case2Branch1Color(CSPInstance* cspInstance, const CSPInstance* reduced);
	static void lemma8Case2Branch2Reduce(CSPInstance* reduced, const ColorPair& vR, const ColorPair& wB, const ColorPair& wG);
	static void lemma8Case2Branch2Color(CSPInstance* cspInstance, const CSPInstance* reduced);

	static Result lemma9(CSPInstance* cspInstance);
	static void lemma9Match(const CSPInstance* cspInstance, ColorPair& vR);
	static void lemma9Branch1Reduce(CSPInstance* reduced, const ColorPair& vR);
	static void lemma9Branch1Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR);
	static void lemma9Branch2Reduce(CSPInstance* reduced, const ColorPair& vR);
	static void lemma9Branch2Color(CSPInstance* cspInstance, const CSPInstance* reduced);

	static Result lemma10(CSPInstance* cspInstance);
	static void lemma10Match(const CSPInstance* cspInstance, ColorPair& vR, ColorPair& wR, ColorPair& xR);
	static Result lemma10Case1(CSPInstance* cspInstance, const ColorPair& vR);
	static void lemma10Case1Branch1Reduce(CSPInstance* reduced, const ColorPair& vR);
	static void lemma10Case1Branch1Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR);
	static void lemma10Case1Branch2Reduce(CSPInstance* reduced, const ColorPair& vR);
	static void lemma10Case1Branch2Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR);
	static Result lemma10Case2(CSPInstance* cspInstance, const ColorPair& vR, const ColorPair& wR, const ColorPair& xR);
	static void lemma10Case2Reduce(CSPInstance* reduced, const ColorPair& varR);
	static void lemma10Case2Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& varR);

	static Result lemma11(CSPInstance* cspInstance);
	static void lemma11Match(const CSPInstance* cspInstance, ColorPair& vR);
	static void lemma11Branch1Reduce(CSPInstance* reduced, const ColorPair& vR);
	static void lemma11Branch1Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR);
	static void lemma11Branch2Reduce(CSPInstance* reduced, const ColorPair& vR);
	static void lemma11Branch2Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR);

	static Result lemma12(CSPInstance* cspInstance);
	static void lemma12Match(const CSPInstance* cspInstance, ColorPair& vR, int& lemmaCase,
		std::unordered_set<ColorPair>& bipartite, ColorPair& xR, ColorPair& yR, std::unordered_set<ColorPair>& component);
	static Result lemma12Case1(CSPInstance* cspInstance, const ColorPair& vR);
	static void lemma12Case1Branch1Reduce(CSPInstance* reduced, const ColorPair& vR);
	static void lemma12Case1Branch1Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR);
	static void lemma12Case1Branch2Reduce(CSPInstance* reduced, const ColorPair& vR);
	static void lemma12Case1Branch2Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR);
	static Result lemma12Case2(CSPInstance* cspInstance, std::unordered_set<ColorPair> bipartite);
	static void lemma12Case2Reduce(CSPInstance* reduced, std::unordered_set<ColorPair> bipartite);
	static void lemma12Case2Color(CSPInstance* cspInstance, const CSPInstance* reduced, std::unordered_set<ColorPair> bipartite);
	static Result lemma12Case3(CSPInstance* cspInstance, const ColorPair& xR, const ColorPair& yR,
		std::unordered_set<ColorPair> component);
	static void lemma12Case3Branch1Reduce(CSPInstance* reduced, const ColorPair& xR, const ColorPair& yR);
	static void lemma12Case3Branch1Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& xR,
		const ColorPair& yR);
	static void lemma12Case3Branch2And3Reduce(CSPInstance* reduced, const ColorPair& varR, const ColorPair& fR,
		const ColorPair& gR);
	static void lemma12Case3Branch2And3Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& varR,
		const ColorPair& fR, const ColorPair& gR);

	static Result lemma13(CSPInstance* cspInstance);
	static void lemma13Match(const CSPInstance* cspInstance, ColorPair& zR);
	static void lemma13Branch1Reduce(CSPInstance* reduced, const ColorPair& zR);
	static void lemma13Branch1Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& zR);
	static void lemma13Branch2Reduce(CSPInstance* reduced, const ColorPair& zR);
	static void lemma13Branch2Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& zR);

	static Result lemma14(CSPInstance* cspInstance);
	static void lemma14Match(const CSPInstance* cspInstance, ColorPair& vR, ColorPair& wR, ColorPair& xR, ColorPair& yR);
	static void lemma14Branch1Reduce(CSPInstance* reduced, const ColorPair& wR);
	static void lemma14Branch1Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& wR);
	static void lemma14Branch2Reduce(CSPInstance* reduced, const ColorPair& xR);
	static void lemma14Branch2Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& xR);
	static void lemma14Branch3Reduce(CSPInstance* reduced, const ColorPair& vR, const ColorPair& yR);
	static void lemma14Branch3Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR,
		const ColorPair& yR);

	static Result lemma15(CSPInstance* cspInstance);
	static void lemma15Match(const CSPInstance* cspInstance, ColorPair& wR, ColorPair& xR);
	static void lemma15Branch1Reduce(CSPInstance* reduced, const ColorPair& wR);
	static void lemma15Branch1Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& wR);
	static void lemma15Branch2Reduce(CSPInstance* reduced, const ColorPair& xR);
	static void lemma15Branch2Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& xR);

	static Result lemma16(CSPInstance* cspInstance);
	static void lemma16Match(const CSPInstance* cspInstance, ColorPair& vR);
	static void lemma16Branch1Reduce(CSPInstance* reduced, const ColorPair& vR);
	static void lemma16Branch1Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR);
	static void lemma16Branch2Reduce(CSPInstance* reduced, const ColorPair& vR);
	static void lemma16Branch2Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR);

	static Result lemma18(CSPInstance* cspInstance);
	static void lemma18Match(const CSPInstance* cspInstance, ColorPair& vR);
	static void lemma18Branch1Reduce(CSPInstance* reduced, const ColorPair& vR);
	static void lemma18Branch1Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR);
	static void lemma18Branch2Reduce(CSPInstance* reduced, const ColorPair& vR);
	static void lemma18Branch2Color(CSPInstance* cspInstance, const CSPInstance* reduced);
public:
	CSPSolver() = delete;
	~CSPSolver() = delete;
	static Result solve(CSPInstance* cspInstance);
};

#endif
