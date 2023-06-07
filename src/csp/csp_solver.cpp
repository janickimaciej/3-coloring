#include "csp_solver.hpp"
#include <map>
#include <set>
#include <stack>
#include <utility>
#include <iostream>
#include <stdexcept>

const Lemma CSPSolver::lemmas[lemmasCount] = {
	&lemma0,
	&lemma1,
	&lemma2,
	&lemma3,
	&lemma4,
	&lemma5,
	&lemma6,
	&lemma7,
	&lemma8,
	&lemma9,
	&lemma10,
	&lemma11
};

Result CSPSolver::solve(CSPInstance* cspInstance) {
	for(int lemma = 0; lemma < lemmasCount; lemma++) {
		Result result = (*lemmas[lemma])(cspInstance);
		if(result != Result::NoMatch) {
			return result;
		}
	}
	const char* errorMsg = "solve: None of the lemmas matched";
	std::cerr << errorMsg << std::endl;
	throw std::logic_error(errorMsg);
}

void CSPSolver::chooseColorReduce(CSPInstance* reduced, const ColorPair& varCol) {
	std::vector<ColorPair> constraints = reduced->getConstraints(varCol.variable, varCol.color);
	for(auto constraint = constraints.begin(); constraint != constraints.end(); constraint++) {
		reduced->disableColor(constraint->variable, constraint->color);
	}
	reduced->removeVariable(varCol.variable);
}

void CSPSolver::chooseColorColor(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& varCol) {
	std::vector<int> removedVariables;
	removedVariables.push_back(varCol.variable);
	cspInstance->copyColoring(reduced, removedVariables);
	cspInstance->setColor(varCol.variable, varCol.color);
}

Result CSPSolver::lemma0(CSPInstance* cspInstance) {
	int variableCount = cspInstance->getVariableCount();
	if(variableCount == 0) {
		std::cerr << "Lemma0" << std::endl;
		return Result::Success;
	}

	for(int variable = 0; variable < variableCount; variable++) {
		std::vector<int> availableColors = cspInstance->getAvailableColors(variable);
		if(availableColors.size() == 0) {
			std::cerr << "Lemma0" << std::endl;
			return Result::Failure;
		} else if(availableColors.size() == 1) {
			std::cerr << "Lemma0" << std::endl;
			ColorPair vR(variable, availableColors[0]);
			CSPInstance* reduced = CSPInstance::copy(cspInstance);
			lemma0Reduce(reduced, vR);

			Result result = solve(reduced);
			if(result == Result::Success) {
				lemma0Color(cspInstance, reduced, vR);
			}
			delete reduced;
			return result;
		}
	}

	return Result::NoMatch;
}

void CSPSolver::lemma0Reduce(CSPInstance* reduced, const ColorPair& vR) {
	chooseColorReduce(reduced, vR);
}

void CSPSolver::lemma0Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR) {
	chooseColorColor(cspInstance, reduced, vR);
}

Result CSPSolver::lemma1(CSPInstance* cspInstance) {
	int v = -1;
	int R;
	int G;
	lemma1Match(cspInstance, v, R, G);
	if(v == -1) {
		return Result::NoMatch;
	}
	std::cerr << "Lemma1" << std::endl;

	CSPInstance* reduced = CSPInstance::copy(cspInstance);
	lemma1Reduce(reduced, v, R, G);

	Result result = solve(reduced);
	if(result == Result::Success) {
		lemma1Color(cspInstance, reduced, v, R, G);
	}
	delete reduced;
	return result;
}

void CSPSolver::lemma1Match(const CSPInstance* cspInstance, int& v, int& R, int& G) {
	for(int variable = 0; variable < cspInstance->getVariableCount(); variable++) {
		std::vector<int> availableColors = cspInstance->getAvailableColors(variable);
		if(availableColors.size() == 2) {
			v = variable;
			R = availableColors[0];
			G = availableColors[1];
			return;
		}
	}
}

void CSPSolver::lemma1Reduce(CSPInstance* reduced, int v, int R, int G) {
	std::vector<ColorPair> conflictR = reduced->getConstraints(v, R);
	std::vector<ColorPair> conflictG = reduced->getConstraints(v, G);
	for(int r = 0; r < conflictR.size(); r++) {
		for(int g = 0; g < conflictG.size(); g++) {
			reduced->addConstraint(conflictR[r].variable, conflictR[r].color, conflictG[g].variable, conflictG[g].color);
		}
	}

	reduced->removeVariable(v);
}

void CSPSolver::lemma1Color(CSPInstance* cspInstance, const CSPInstance* reduced, int v, int R, int G) {
	std::vector<int> removedVariables;
	removedVariables.push_back(v);
	cspInstance->copyColoring(reduced, removedVariables);
	if(reduced->getConstraints(v, R).size() == 0) {
		cspInstance->setColor(v, R);
	} else {
		cspInstance->setColor(v, G);
	}
}

Result CSPSolver::lemma2(CSPInstance* cspInstance) {
	ColorPair vX(-1, -1);
	ColorPair wY(-1, -1);
	lemma2Match(cspInstance, vX, wY);
	if(vX.variable == -1) {
		return Result::NoMatch;
	}
	std::cerr << "Lemma2" << std::endl;

	CSPInstance* reduced = CSPInstance::copy(cspInstance);
	lemma2Reduce(reduced, vX.variable, wY.variable);
	
	Result result = solve(reduced);
	if(result == Result::Success) {
		lemma2Color(cspInstance, reduced, vX, wY);
	}
	delete reduced;
	return result;
}

void CSPSolver::lemma2Match(const CSPInstance* cspInstance, ColorPair& vX, ColorPair& wY) {
	for(int v = 0; v < cspInstance->getVariableCount(); v++) {
		std::vector<int> vColors = cspInstance->getAvailableColors(v);
		for(auto vColor = vColors.begin(); vColor != vColors.end(); vColor++) {
			std::vector<ColorPair> vConstraints = cspInstance->getConstraints(v, *vColor);
			for(int w = 0; w < cspInstance->getVariableCount(); w++) {
				if(w == v) {
					continue;
				}
				std::vector<int> wColors = cspInstance->getAvailableColors(w);
				for(auto wColor = wColors.begin(); wColor != wColors.end(); wColor++) {
					bool matching = true;
					for(auto vConstraint = vConstraints.begin(); vConstraint != vConstraints.end(); vConstraint++) {
						if(vConstraint->variable != w || vConstraint->color == *wColor) {
							matching = false;
							break;
						}
					}
					if(!matching) {
						continue;
					}

					std::vector<ColorPair> wConstraints = cspInstance->getConstraints(w, *wColor);
					for(auto wConstraint = wConstraints.begin(); wConstraint != wConstraints.end(); wConstraint++) {
						if(wConstraint->variable != v || wConstraint->color == *vColor) {
							matching = false;
							break;
						}
					}
					if(!matching) {
						continue;
					}

					vX.variable = v;
					vX.color = *vColor;
					wY.variable = w;
					wY.color = *wColor;
					return;
				}
			}
		}
	}
}

void CSPSolver::lemma2Reduce(CSPInstance* reduced, int v, int w) {
	reduced->removeVariable(v < w ? w : v);
	reduced->removeVariable(v < w ? v : w);
}

void CSPSolver::lemma2Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vX, const ColorPair& wY) {
	std::vector<int> removedVariables;
	removedVariables.push_back(vX.variable);
	removedVariables.push_back(wY.variable);
	cspInstance->copyColoring(cspInstance, removedVariables);
	cspInstance->setColor(vX.variable, vX.color);
	cspInstance->setColor(wY.variable, wY.color);
}

Result CSPSolver::lemma3(CSPInstance* cspInstance) {
	ColorPair vB(-1, -1);
	lemma3Match(cspInstance, vB);
	if(vB.variable == -1) {
		return Result::NoMatch;
	}
	std::cerr << "Lemma3" << std::endl;

	lemma3Reduce(cspInstance, vB);
	
	return solve(cspInstance);
}

void CSPSolver::lemma3Match(const CSPInstance* cspInstance, ColorPair& vB) {
	for(int v = 0; v < cspInstance->getVariableCount(); v++) {
		std::vector<int> availableColors = cspInstance->getAvailableColors(v);
		for(auto R = availableColors.begin(); R != availableColors.end(); R++) {
			std::vector<ColorPair> RConstraints = cspInstance->getConstraints(v, *R);
			for(auto B = availableColors.begin(); B != availableColors.end(); B++) {
				if(B == R) {
					continue;
				}
				std::vector<ColorPair> BConstraints = cspInstance->getConstraints(v, *B);
				bool matching = true;
				for(auto RConstraint = RConstraints.begin(); RConstraint != RConstraints.end(); RConstraint++) {
					bool exists = false;
					for(auto BConstraint = BConstraints.begin(); BConstraint != BConstraints.end(); BConstraint++) {
						if(*BConstraint == *RConstraint) {
							exists = true;
							break;
						}
					}
					if(!exists) {
						matching = false;
						break;
					}
				}
				if(!matching) {
					continue;
				}

				vB.variable = v;
				vB.color = *B;
				return;
			}
		}
	}
}

void CSPSolver::lemma3Reduce(CSPInstance* cspInstance, const ColorPair& vB) {
	cspInstance->disableColor(vB.variable, vB.color);
}

Result CSPSolver::lemma4(CSPInstance* cspInstance) {
	ColorPair vR(-1, -1);
	lemma4Match(cspInstance, vR);
	if(vR.variable == -1) {
		return Result::NoMatch;
	}
	std::cerr << "Lemma4" << std::endl;

	CSPInstance* reduced = CSPInstance::copy(cspInstance);
	lemma4Reduce(reduced, vR.variable);

	Result result = solve(reduced);
	if(result == Result::Success) {
		lemma4Color(cspInstance, reduced, vR);
	}
	delete reduced;
	return result;
}

void CSPSolver::lemma4Match(const CSPInstance* cspInstance, ColorPair& vR) {
	for(int v = 0; v < cspInstance->getVariableCount(); v++) {
		std::vector<int> availableColors = cspInstance->getAvailableColors(v);
		for(auto R = availableColors.begin(); R != availableColors.end(); R++) {
			if(cspInstance->getConstraints(v, *R).size() == 0) {
				vR.variable = v;
				vR.color = *R;
				return;
			}
		}
	}
}

void CSPSolver::lemma4Reduce(CSPInstance* reduced, int v) {
	reduced->removeVariable(v);
}

void CSPSolver::lemma4Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR) {
	std::vector<int> removedVariables;
	removedVariables.push_back(vR.variable);
	cspInstance->copyColoring(reduced, removedVariables);
	cspInstance->setColor(vR.variable, vR.color);
}

Result CSPSolver::lemma5(CSPInstance* cspInstance) {
	ColorPair vR(-1, -1);
	lemma5Match(cspInstance, vR);
	if(vR.variable == -1) {
		return Result::NoMatch;
	}
	std::cerr << "Lemma5" << std::endl;

	lemma5Reduce(cspInstance, vR);

	return solve(cspInstance);
}

void CSPSolver::lemma5Match(const CSPInstance* cspInstance, ColorPair& vR) {
	for(int v = 0; v < cspInstance->getVariableCount(); v++) {
		std::vector<int> availableColors = cspInstance->getAvailableColors(v);
		for(auto R = availableColors.begin(); R != availableColors.end(); R++) {
			std::vector<ColorPair> constraints = cspInstance->getConstraints(v, *R);
			std::map<int, int> neighborCounters;
			for(auto constraint = constraints.begin(); constraint != constraints.end(); constraint++) {
				auto neighborCounter = neighborCounters.find(constraint->variable);
				if(neighborCounter != neighborCounters.end()) {
					neighborCounter->second++;
					if(neighborCounter->second == cspInstance->getAvailableColors(neighborCounter->first).size()) {
						vR.variable = v;
						vR.color = *R;
						return;
					}
				} else {
					neighborCounters.insert(std::pair<int, int>(constraint->variable, 1));
				}
			}
		}
	}
}

void CSPSolver::lemma5Reduce(CSPInstance* cspInstance, const ColorPair& vR) {
	cspInstance->disableColor(vR.variable, vR.color);
}

Result CSPSolver::lemma6(CSPInstance* cspInstance) {
	ColorPair vR(-1, -1);
	ColorPair wR(-1, -1);
	lemma6Match(cspInstance, vR, wR);
	if(vR.variable == -1) {
		return Result::NoMatch;
	}
	std::cerr << "Lemma6" << std::endl;

	int vSize = (int)cspInstance->getAvailableColors(vR.variable).size();
	int wSize = (int)cspInstance->getAvailableColors(wR.variable).size();
	if(vSize == 3 && wSize == 3) {
		return lemma6Case1(cspInstance, vR, wR);
	} else {
		return lemma6Case2(cspInstance, vR, wR);
	}
}

void CSPSolver::lemma6Match(const CSPInstance* cspInstance, ColorPair& vR, ColorPair& wR) {
	for(int v = 0; v < cspInstance->getVariableCount(); v++) {
		std::vector<int> availableColors = cspInstance->getAvailableColors(v);
		for(auto R = availableColors.begin(); R != availableColors.end(); R++) {
			std::vector<ColorPair> constraints = cspInstance->getConstraints(v, *R);
			if(constraints.size() != 1) {
				continue;
			}
			std::vector<ColorPair> neighborConstraints = cspInstance->getConstraints(constraints[0].variable,
				constraints[0].color);
			if(neighborConstraints.size() != 1) {
				continue;
			}
			vR.variable = v;
			vR.color = *R;
			wR.variable = constraints[0].variable;
			wR.color = constraints[0].color;
			return;
		}
	}
}

Result CSPSolver::lemma6Case1(CSPInstance* cspInstance, const ColorPair& vR, const ColorPair& wR) {
	CSPInstance* reduced = CSPInstance::copy(cspInstance);
	lemma6Case1Reduce(reduced, vR, wR);

	Result result = solve(reduced);
	if(result == Result::Success) {
		lemma6Case1Color(cspInstance, reduced, vR, wR);
	}
	delete reduced;
	return result;
}

void CSPSolver::lemma6Case1Reduce(CSPInstance* reduced, const ColorPair& vR, const ColorPair& wR) {
	ColorPair vG(vR.variable, (vR.color + 1)%3);
	ColorPair vB(vR.variable, (vR.color + 2)%3);
	ColorPair wG(wR.variable, (wR.color + 1)%3);
	ColorPair wB(wR.variable, (wR.color + 2)%3);

	std::vector<ColorPair> vGConstraints = reduced->getConstraints(vG.variable, vG.color);
	std::vector<ColorPair> vBConstraints = reduced->getConstraints(vB.variable, vB.color);
	std::vector<ColorPair> wGConstraints = reduced->getConstraints(wG.variable, wG.color);
	std::vector<ColorPair> wBConstraints = reduced->getConstraints(wB.variable, wB.color);

	reduced->removeVariable(vR.variable < wR.variable ? wR.variable : vR.variable);
	reduced->removeVariable(vR.variable < wR.variable ? vR.variable : wR.variable);

	reduced->addVariable();
	int u = reduced->getVariableCount() - 1;
	for(auto constraint = vGConstraints.begin(); constraint != vGConstraints.end(); constraint++) {
		reduced->addConstraint(u, 0, constraint->variable, constraint->color);
	}
	for(auto constraint = vBConstraints.begin(); constraint != vBConstraints.end(); constraint++) {
		reduced->addConstraint(u, 1, constraint->variable, constraint->color);
	}
	for(auto constraint = wGConstraints.begin(); constraint != wGConstraints.end(); constraint++) {
		reduced->addConstraint(u, 2, constraint->variable, constraint->color);
	}
	for(auto constraint = wBConstraints.begin(); constraint != wBConstraints.end(); constraint++) {
		reduced->addConstraint(u, 3, constraint->variable, constraint->color);
	}
}

void CSPSolver::lemma6Case1Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR,
	const ColorPair& wR) {
	std::vector<int> removedVariables;
	removedVariables.push_back(vR.variable);
	removedVariables.push_back(wR.variable);
	cspInstance->copyColoring(reduced, removedVariables);
	int u = reduced->getVariableCount() - 1;
	int uColor = reduced->getAvailableColors(u)[0];
	if(uColor == 0 || uColor == 1) {
		cspInstance->setColor(vR.variable, uColor);
		cspInstance->setColor(wR.variable, wR.color);
	} else {
		cspInstance->setColor(wR.variable, uColor);
		cspInstance->setColor(vR.variable, vR.color);
	}
}

Result CSPSolver::lemma6Case2(CSPInstance* cspInstance, const ColorPair& vR, const ColorPair& wR) {
	CSPInstance* reduced = CSPInstance::copy(cspInstance);
	lemma6Case2Branch1Reduce(reduced, vR, wR);

	Result result = solve(reduced);
	if(result == Result::Success) {
		lemma6Case2Branch1Color(cspInstance, reduced, vR, wR);
		delete reduced;
		return Result::Success;
	}
	delete reduced;

	reduced = CSPInstance::copy(cspInstance);
	lemma6Case2Branch2Reduce(reduced, vR, wR);

	result = solve(reduced);
	if(result == Result::Success) {
		lemma6Case2Branch2Color(cspInstance, reduced, vR, wR);
	}
	delete reduced;
	return result;
}

void CSPSolver::lemma6Case2Branch1Reduce(CSPInstance* reduced, const ColorPair& vR, const ColorPair& wR) {
	reduced->disableColor(wR.variable, wR.color);
	reduced->removeVariable(vR.variable);
}

void CSPSolver::lemma6Case2Branch1Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR,
	const ColorPair& wR) {
	std::vector<int> removedVariables;
	removedVariables.push_back(vR.variable);
	cspInstance->copyColoring(reduced, removedVariables);
	cspInstance->setColor(vR.variable, vR.color);
}

void CSPSolver::lemma6Case2Branch2Reduce(CSPInstance* reduced, const ColorPair& vR, const ColorPair& wR) {
	reduced->disableColor(vR.variable, vR.color);
	reduced->removeVariable(wR.variable);
}

void CSPSolver::lemma6Case2Branch2Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR,
	const ColorPair& wR) {
	std::vector<int> removedVariables;
	removedVariables.push_back(wR.variable);
	cspInstance->copyColoring(reduced, removedVariables);
	cspInstance->setColor(wR.variable, wR.color);
}

Result CSPSolver::lemma7(CSPInstance* cspInstance) {
	ColorPair vR(-1, -1);
	ColorPair wR(-1, -1);
	lemma7Match(cspInstance, vR, wR);
	if(vR.variable == -1) {
		return Result::NoMatch;
	}
	std::cerr << "Lemma7" << std::endl;

	CSPInstance* reduced = CSPInstance::copy(cspInstance);
	lemma7Branch1Reduce(reduced, vR, wR);

	Result result = solve(reduced);
	if(result == Result::Success) {
		lemma7Branch1Color(cspInstance, reduced, vR, wR);
		delete reduced;
		return Result::Success;
	}
	delete reduced;

	reduced = CSPInstance::copy(cspInstance);
	lemma7Branch2Reduce(reduced, wR);

	result = solve(reduced);
	if(result == Result::Success) {
		lemma7Branch2Color(cspInstance, reduced, wR);
	}
	delete reduced;
	return result;
}

void CSPSolver::lemma7Match(const CSPInstance* cspInstance, ColorPair& vR, ColorPair& wR) {
	for(int v = 0; v < cspInstance->getVariableCount(); v++) {
		std::vector<int> availableColors = cspInstance->getAvailableColors(v);
		for(auto R = availableColors.begin(); R != availableColors.end(); R++) {
			std::vector<ColorPair> constraints = cspInstance->getConstraints(v, *R);
			if(constraints.size() != 1) {
				continue;
			}
			vR.variable = v;
			vR.color = *R;
			wR.variable = constraints[0].variable;
			wR.color = constraints[0].color;
			return;
		}
	}
}

void CSPSolver::lemma7Branch1Reduce(CSPInstance* reduced, const ColorPair& vR, const ColorPair& wR) {
	reduced->disableColor(wR.variable, wR.color);
	reduced->removeVariable(vR.variable);
}

void CSPSolver::lemma7Branch1Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR,
	const ColorPair& wR) {
	std::vector<int> removedVariables;
	removedVariables.push_back(vR.variable);
	cspInstance->copyColoring(reduced, removedVariables);
	cspInstance->setColor(vR.variable, vR.color);
}

void CSPSolver::lemma7Branch2Reduce(CSPInstance* reduced, const ColorPair& wR) {
	chooseColorReduce(reduced, wR);
}

void CSPSolver::lemma7Branch2Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& wR) {
	chooseColorColor(cspInstance, reduced, wR);
}

Result CSPSolver::lemma8(CSPInstance* cspInstance) {
	ColorPair vR(-1, -1);
	ColorPair wR(-1, -1);
	ColorPair wB(-1, -1);
	ColorPair wG(-1, -1);
	bool isImplication = false;
	lemma8Match(cspInstance, vR, wR, wB, wG, isImplication);
	if(vR.variable == -1) {
		return Result::NoMatch;
	}

	if(isImplication) {
		return lemma8Case1(cspInstance, vR, wR);
	} else {
		return lemma8Case2(cspInstance, vR, wB, wG);
	}
}

void CSPSolver::lemma8Match(const CSPInstance* cspInstance, ColorPair& vR, ColorPair& wR, ColorPair& wB, ColorPair& wG,
	bool& isImplication) {
	for(int v = 0; v < cspInstance->getVariableCount(); v++) {
		std::vector<int> availableColors = cspInstance->getAvailableColors(v);
		for(auto R = availableColors.begin(); R != availableColors.end(); R++) {
			std::vector<ColorPair> constraints = cspInstance->getConstraints(v, *R);
			std::set<int> neighbors;
			for(auto constraint = constraints.begin(); constraint != constraints.end(); constraint++) {
				if(neighbors.find(constraint->variable) != neighbors.end()) {
					vR.variable = v;
					vR.color = *R;
					int w = constraint->variable;
					wR.variable = w;
					wB.variable = w;
					wG.variable = w;
					std::vector<int> wAvailableColors = cspInstance->getAvailableColors(w);
					for(auto wColor = wAvailableColors.begin(); wColor != wAvailableColors.end(); wColor++) {
						if(cspInstance->hasConstraint(v, *R, w, *wColor)) {
							if(wB.color == -1) {
								wB.color = *wColor;
							} else if(wG.color == -1) {
								wG.color = *wColor;
							} else {
								isImplication = true;
							}
						} else if(wR.color == -1) {
							wR.color = *wColor;
						}
					}
					return;
				} else {
					neighbors.insert(constraint->variable);
				}
			}
		}
	}
}

Result CSPSolver::lemma8Case1(CSPInstance* cspInstance, const ColorPair& vR, const ColorPair& wR) {
	CSPInstance* reduced = CSPInstance::copy(cspInstance);
	lemma8Case1Branch1Reduce(reduced, vR, wR);

	Result result = solve(reduced);
	if(result == Result::Success) {
		lemma8Case1Branch1Color(cspInstance, reduced);
		delete reduced;
		return Result::Success;
	}
	delete reduced;

	reduced = CSPInstance::copy(cspInstance);
	lemma8Case1Branch2Reduce(reduced, wR);

	result = solve(reduced);
	if(result == Result::Success) {
		lemma8Case1Branch2Color(cspInstance, reduced, wR);
	}
	delete reduced;
	return result;
}

void CSPSolver::lemma8Case1Branch1Reduce(CSPInstance* reduced, const ColorPair& vR, const ColorPair& wR) {
	reduced->disableColor(vR.variable, vR.color);
	reduced->disableColor(wR.variable, wR.color);
}

void CSPSolver::lemma8Case1Branch1Color(CSPInstance* cspInstance, const CSPInstance* reduced) {
	cspInstance->copyColoring(reduced, std::vector<int>());
}

void CSPSolver::lemma8Case1Branch2Reduce(CSPInstance* reduced, const ColorPair& wR) {
	chooseColorReduce(reduced, wR);
}

void CSPSolver::lemma8Case1Branch2Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& wR) {
	chooseColorColor(cspInstance, reduced, wR);
}

Result CSPSolver::lemma8Case2(CSPInstance* cspInstance, const ColorPair& vR, const ColorPair& wB, const ColorPair& wG) {
	CSPInstance* reduced = CSPInstance::copy(cspInstance);
	lemma8Case2Branch1Reduce(reduced, wB, wG);

	Result result = solve(reduced);
	if(result == Result::Success) {
		lemma8Case2Branch1Color(cspInstance, reduced);
		delete reduced;
		return Result::Success;
	}
	delete reduced;

	reduced = CSPInstance::copy(cspInstance);
	lemma8Case2Branch2Reduce(reduced, vR, wB, wG);

	result = solve(reduced);
	if(result == Result::Success) {
		lemma8Case2Branch2Color(cspInstance, reduced);
	}
	delete reduced;
	return result;
}

void CSPSolver::lemma8Case2Branch1Reduce(CSPInstance* reduced, const ColorPair& wB, const ColorPair& wG) {
	reduced->disableColor(wB.variable, wB.color);
	reduced->disableColor(wG.variable, wG.color);
}

void CSPSolver::lemma8Case2Branch1Color(CSPInstance* cspInstance, const CSPInstance* reduced) {
	cspInstance->copyColoring(reduced, std::vector<int>());
}

void CSPSolver::lemma8Case2Branch2Reduce(CSPInstance* reduced, const ColorPair& vR, const ColorPair& wB, const ColorPair& wG) {
	int w = wB.variable;
	std::vector<int> availableColors = reduced->getAvailableColors(w);
	for(auto availableColor = availableColors.begin(); availableColor != availableColors.end(); availableColor++) {
		if(*availableColor != wB.color && *availableColor != wG.color) {
			reduced->disableColor(w, *availableColor);
		}
	}
	reduced->disableColor(vR.variable, vR.color);
}

void CSPSolver::lemma8Case2Branch2Color(CSPInstance* cspInstance, const CSPInstance* reduced) {
	cspInstance->copyColoring(reduced, std::vector<int>());
}

Result CSPSolver::lemma9(CSPInstance* cspInstance) {
	ColorPair vR(-1, -1);
	lemma9Match(cspInstance, vR);
	if(vR.variable == -1) {
		return Result::NoMatch;
	}

	CSPInstance* reduced = CSPInstance::copy(cspInstance);
	lemma9Branch1Reduce(reduced, vR);
	
	Result result = solve(reduced);
	if(result == Result::Success) {
		lemma9Branch1Color(cspInstance, reduced, vR);
		delete reduced;
		return Result::Success;
	}
	delete reduced;

	reduced = CSPInstance::copy(cspInstance);
	lemma9Branch2Reduce(reduced, vR);

	result = solve(reduced);
	if(result == Result::Success) {
		lemma9Branch2Color(cspInstance, reduced);
	}
	delete reduced;
	return result;
}

void CSPSolver::lemma9Match(const CSPInstance* cspInstance, ColorPair& vR) {
	for(int v = 0; v < cspInstance->getVariableCount(); v++) {
		std::vector<int> availableColors = cspInstance->getAvailableColors(v);
		for(auto R = availableColors.begin(); R != availableColors.end(); R++) {
			std::vector<ColorPair> constraints = cspInstance->getConstraints(v, *R);
			if((availableColors.size() == 4 && constraints.size() >= 3) ||
				(availableColors.size() == 3 && constraints.size() >= 4)) {
				vR.variable = v;
				vR.color = *R;
			}
		}
	}
}

void CSPSolver::lemma9Branch1Reduce(CSPInstance* reduced, const ColorPair& vR) {
	chooseColorReduce(reduced, vR);
}

void CSPSolver::lemma9Branch1Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR) {
	chooseColorColor(cspInstance, reduced, vR);
}

void CSPSolver::lemma9Branch2Reduce(CSPInstance* reduced, const ColorPair& vR) {
	reduced->disableColor(vR.variable, vR.color);
}

void CSPSolver::lemma9Branch2Color(CSPInstance* cspInstance, const CSPInstance* reduced) {
	cspInstance->copyColoring(reduced, std::vector<int>());
}

Result CSPSolver::lemma10(CSPInstance* cspInstance) {
	ColorPair vR(-1, -1);
	ColorPair wR(-1, -1);
	ColorPair xR(-1, -1);
	lemma10Match(cspInstance, vR, wR, xR);
	if(vR.variable == -1) {
		return Result::NoMatch;
	}

	if(xR.variable == -1) {
		return lemma10Case1(cspInstance, vR);
	} else {
		return lemma10Case2(cspInstance, vR, wR, xR);
	}
}

void CSPSolver::lemma10Match(const CSPInstance* cspInstance, ColorPair& vR, ColorPair& wR, ColorPair& xR) {
	for(int v = 0; v < cspInstance->getVariableCount(); v++) {
		std::vector<int> availableColors = cspInstance->getAvailableColors(v);
		for(auto R = availableColors.begin(); R != availableColors.end(); R++) {
			std::vector<ColorPair> vConstraints = cspInstance->getConstraints(v, *R);
			if(vConstraints.size() != 3) {
				continue;
			}
			for(auto vConstraint = vConstraints.begin(); vConstraint != vConstraints.end(); vConstraint++) {
				if(cspInstance->getAvailableColors(vConstraint->variable).size() != 4) {
					continue;
				}
				vR.variable = v;
				vR.color = *R;
				wR.variable = vConstraint->variable;
				wR.color = vConstraint->color;
				std::vector<ColorPair> wConstraints = cspInstance->getConstraints(wR.variable, wR.color);
				if(!cspInstance->hasConstraint(wConstraints[0].variable, wConstraints[0].color,
					wConstraints[1].variable, wConstraints[1].color)) {
					return;
				}
				if(wConstraints[0].variable != v) {
					xR.variable = wConstraints[0].variable;
					xR.color = wConstraints[0].color;
				} else {
					xR.variable = wConstraints[1].variable;
					xR.color = wConstraints[1].color;
				}
				return;
			}
		}
	}
}

Result CSPSolver::lemma10Case1(CSPInstance* cspInstance, const ColorPair& vR) {
	CSPInstance* reduced = CSPInstance::copy(cspInstance);
	lemma10Case1Branch1Reduce(reduced, vR);

	Result result = solve(reduced);
	if(result == Result::Success) {
		lemma10Case1Branch1Color(cspInstance, reduced, vR);
		delete reduced;
		return Result::Success;
	}
	delete reduced;

	reduced = CSPInstance::copy(cspInstance);
	lemma10Case1Branch2Reduce(reduced, vR);

	result = solve(reduced);
	if(result == Result::Success) {
		lemma10Case1Branch2Color(cspInstance, reduced, vR);
	}
	delete reduced;
	return result;
}

void CSPSolver::lemma10Case1Branch1Reduce(CSPInstance* reduced, const ColorPair& vR) {
	chooseColorReduce(reduced, vR);
}

void CSPSolver::lemma10Case1Branch1Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR) {
	chooseColorColor(cspInstance, reduced, vR);
}

void CSPSolver::lemma10Case1Branch2Reduce(CSPInstance* reduced, const ColorPair& vR) {
	reduced->disableColor(vR.variable, vR.color);
}

void CSPSolver::lemma10Case1Branch2Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR) {
	cspInstance->copyColoring(reduced, std::vector<int>());
}

Result CSPSolver::lemma10Case2(CSPInstance* cspInstance, const ColorPair& vR, const ColorPair& wR, const ColorPair& xR) {
	CSPInstance* reduced = CSPInstance::copy(cspInstance);
	lemma10Case2Reduce(reduced, vR);

	Result result = solve(reduced);
	if(result == Result::Success) {
		lemma10Case2Color(cspInstance, reduced, vR);
		delete reduced;
		return Result::Success;
	}
	delete reduced;

	reduced = CSPInstance::copy(cspInstance);
	lemma10Case2Reduce(reduced, wR);

	result = solve(reduced);
	if(result == Result::Success) {
		lemma10Case2Color(cspInstance, reduced, wR);
		delete reduced;
		return Result::Success;
	}
	delete reduced;

	reduced = CSPInstance::copy(cspInstance);
	lemma10Case2Reduce(reduced, xR);

	result = solve(reduced);
	if(result == Result::Success) {
		lemma10Case2Color(cspInstance, reduced, xR);
	}
	delete reduced;
	return result;
}

void CSPSolver::lemma10Case2Reduce(CSPInstance* reduced, const ColorPair& varR) {
	chooseColorReduce(reduced, varR);
}

void CSPSolver::lemma10Case2Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& varR) {
	chooseColorColor(cspInstance, reduced, varR);
}

Result CSPSolver::lemma11(CSPInstance* cspInstance) {
	ColorPair vR(-1, -1);
	lemma11Match(cspInstance, vR);
	if(vR.variable == -1) {
		return Result::NoMatch;
	}

	CSPInstance* reduced = CSPInstance::copy(cspInstance);
	lemma11Branch1Reduce(reduced, vR);

	Result result = solve(reduced);
	if(result == Result::Success) {
		lemma11Branch1Color(cspInstance, reduced, vR);
		delete reduced;
		return Result::Success;
	}
	delete reduced;

	reduced = CSPInstance::copy(cspInstance);
	lemma11Branch2Reduce(reduced, vR);

	result = solve(reduced);
	if(result == Result::Success) {
		lemma11Branch2Color(cspInstance, reduced, vR);
	}
	delete reduced;
	return result;
}

void CSPSolver::lemma11Match(const CSPInstance* cspInstance, ColorPair& vR) {
	for(int v = 0; v < cspInstance->getVariableCount(); v++) {
		std::vector<int> availableColors = cspInstance->getAvailableColors(v);
		for(auto R = availableColors.begin(); R != availableColors.end(); R++) {
			std::vector<ColorPair> constraints = cspInstance->getConstraints(v, *R);
			if(constraints.size() != 3) {
				continue;
			}
			for(auto constraint = constraints.begin(); constraint != constraints.end(); constraint++) {
				if(cspInstance->getConstraints(constraint->variable, constraint->color).size() != 2) {
					continue;
				}
				vR.variable = v;
				vR.color = *R;
				return;
			}
		}
	}
}

void CSPSolver::lemma11Branch1Reduce(CSPInstance* reduced, const ColorPair& vR) {
	chooseColorReduce(reduced, vR);
}

void CSPSolver::lemma11Branch1Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR) {
	chooseColorColor(cspInstance, reduced, vR);
}

void CSPSolver::lemma11Branch2Reduce(CSPInstance* reduced, const ColorPair& vR) {
	reduced->disableColor(vR.variable, vR.color);
}

void CSPSolver::lemma11Branch2Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR) {
	cspInstance->copyColoring(reduced, std::vector<int>());
}

Result CSPSolver::lemma12(CSPInstance* cspInstance) {
	ColorPair vR(-1, -1);
	int lemmaCase;
	std::unordered_set<ColorPair> bipartite;
	ColorPair xR(-1, -1);
	ColorPair yR(-1, -1);
	std::unordered_set<ColorPair> component;
	lemma12Match(cspInstance, vR, lemmaCase, bipartite, xR, yR, component);
	if(vR.variable == -1) {
		return Result::NoMatch;
	}

	if(lemmaCase == 1) {
		return lemma12Case1(cspInstance, vR);
	} else if(lemmaCase == 2) {
		return lemma12Case2(cspInstance, bipartite);
	} else {
		return lemma12Case3(cspInstance, xR, yR, component);
	}
}

void CSPSolver::lemma12Match(const CSPInstance* cspInstance, ColorPair& vR, int& lemmaCase,
	std::unordered_set<ColorPair>& bipartite, ColorPair& xR, ColorPair& yR, std::unordered_set<ColorPair>& component) {
	int n = cspInstance->getVariableCount();
	bool** visited = new bool*[n];
	for(int v = 0; v < n; v++) {
		visited[v] = new bool[4];
		for(int R = 0; R < 4; R++) {
			visited[v][R] = false;
		}
	}

	for(int v = 0; v < cspInstance->getVariableCount(); v++) {
		std::vector<int> availableColors = cspInstance->getAvailableColors(v);
		for(auto R = availableColors.begin(); R != availableColors.end(); R++) {
			if(visited[v][*R] || cspInstance->getConstraints(v, *R).size() != 3) {
				continue;
			}
			int k = 0;
			ColorPair current(v, *R);
			std::stack<ColorPair> stack;
			bipartite.clear();
			component.clear();
			stack.push(current);
			bipartite.insert(current);
			while(!stack.empty()) {
				current = stack.top();
				stack.pop();
				if(!visited[current.variable][current.color]) {
					visited[current.variable][current.color] = true;
					component.insert(current);
					k++;
					bool isBipartite = bipartite.find(current) != bipartite.end();
					std::vector<ColorPair> constraints = cspInstance->getConstraints(current.variable, current.color);
					for(auto constraint = constraints.begin(); constraint != constraints.end(); constraint++) {
						stack.push(*constraint);
						if(!isBipartite) {
							bipartite.insert(*constraint);
						}
					}
				}
			}
			if(k != 8 && k != 12) {
				continue;
			}

			vR.variable = v;
			vR.color = *R;
			if(k == 12) {
				lemmaCase = 1;
				return;
			} else if(bipartite.size() == 4) {
				lemmaCase = 2;
				return;
			} else {
				lemmaCase = 3;
				for(auto aR = component.begin(); aR != component.end(); aR++) {
					for(auto bR = component.begin(); bR != component.end(); bR++) {
						if(aR->variable == bR->variable ||
							cspInstance->hasConstraint(aR->variable, aR->color, bR->variable, bR->color)) {
							continue;
						}
						bool haveCommonConstraint = false;
						std::vector<ColorPair> XConstraints = cspInstance->getConstraints(aR->variable, aR->color);
						std::vector<ColorPair> YConstraints = cspInstance->getConstraints(bR->variable, bR->color);
						for(auto XConstraint = XConstraints.begin(); XConstraint != XConstraints.end(); XConstraint++) {
							for(auto YConstraint = YConstraints.begin(); YConstraint != YConstraints.end(); YConstraint++) {
								if(XConstraint == YConstraint) {
									haveCommonConstraint = true;
								}
							}
						}
						if(!haveCommonConstraint) {
							xR.variable = aR->variable;
							xR.color = aR->color;
							yR.variable = bR->variable;
							yR.color = bR->color;
							return;
						}
					}
				}
			}
		}
	}
}

Result CSPSolver::lemma12Case1(CSPInstance* cspInstance, const ColorPair& vR) {
	CSPInstance* reduced = CSPInstance::copy(cspInstance);
	lemma12Case1Branch1Reduce(reduced, vR);

	Result result = solve(reduced);
	if(result == Result::Success) {
		lemma12Case1Branch1Color(cspInstance, reduced, vR);
		delete reduced;
		return Result::Success;
	}
	delete reduced;

	reduced = CSPInstance::copy(cspInstance);
	lemma12Case1Branch2Reduce(reduced, vR);

	result = solve(reduced);
	if(result == Result::Success) {
		lemma12Case1Branch2Color(cspInstance, reduced, vR);
	}
	delete reduced;
	return result;
}

void CSPSolver::lemma12Case1Branch1Reduce(CSPInstance* reduced, const ColorPair& vR) {
	chooseColorReduce(reduced, vR);
}

void CSPSolver::lemma12Case1Branch1Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR) {
	chooseColorColor(cspInstance, reduced, vR);
}

void CSPSolver::lemma12Case1Branch2Reduce(CSPInstance* reduced, const ColorPair& vR) {
	reduced->disableColor(vR.variable, vR.color);
}

void CSPSolver::lemma12Case1Branch2Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR) {
	cspInstance->copyColoring(reduced, std::vector<int>());
}

Result CSPSolver::lemma12Case2(CSPInstance* cspInstance, std::unordered_set<ColorPair> bipartite) {
	CSPInstance* reduced = CSPInstance::copy(cspInstance);
	lemma12Case2Reduce(cspInstance, bipartite);

	Result result = solve(reduced);
	if(result == Result::Success) {
		lemma12Case2Color(cspInstance, reduced, bipartite);
	}
	delete reduced;
	return result;
}

void CSPSolver::lemma12Case2Reduce(CSPInstance* reduced, std::unordered_set<ColorPair> bipartite) {
	for(auto colorPair = bipartite.begin(); colorPair != bipartite.end(); colorPair++) {
		reduced->removeVariable(colorPair->variable);
	}
}

void CSPSolver::lemma12Case2Color(CSPInstance* cspInstance, const CSPInstance* reduced, std::unordered_set<ColorPair> bipartite) {
	std::vector<int> removedVariables;
	for(auto colorPair = bipartite.begin(); colorPair != bipartite.end(); colorPair++) {
		removedVariables.push_back(colorPair->variable);
	}
	cspInstance->copyColoring(reduced, removedVariables);
	for(auto colorPair = bipartite.begin(); colorPair != bipartite.end(); colorPair++) {
		cspInstance->setColor(colorPair->variable, colorPair->color);
	}
}

Result CSPSolver::lemma12Case3(CSPInstance* cspInstance, const ColorPair& xR, const ColorPair& yR,
	std::unordered_set<ColorPair> component) {
	CSPInstance* reduced = CSPInstance::copy(cspInstance);
	lemma12Case3Branch1Reduce(reduced, xR, yR);

	Result result = solve(reduced);
	if(result == Result::Success) {
		lemma12Case3Branch1Color(cspInstance, reduced, xR, yR);
		delete reduced;
		return Result::Success;
	}
	delete reduced;

	ColorPair fR(-1, -1);
	ColorPair gR(-1, -1);
	for(auto colorPair = component.begin(); colorPair != component.end(); colorPair++) {
		if(colorPair->variable != xR.variable && colorPair->variable != yR.variable &&
			!cspInstance->hasConstraint(xR.variable, xR.color, colorPair->variable, colorPair->color)) {
			if(fR.variable == -1) {
				fR.variable = colorPair->variable;
				fR.color = colorPair->color;
			} else {
				gR.variable = colorPair->variable;
				gR.color = colorPair->color;
			}
		}
	}
	reduced = CSPInstance::copy(cspInstance);
	lemma12Case3Branch2And3Reduce(reduced, xR, fR, gR);

	result = solve(reduced);
	if(result == Result::Success) {
		lemma12Case3Branch2And3Color(cspInstance, reduced, xR, fR, gR);
		delete reduced;
		return Result::Success;
	}
	delete reduced;

	for(auto colorPair = component.begin(); colorPair != component.end(); colorPair++) {
		if(colorPair->variable == fR.variable && colorPair->color != fR.color) {
			fR.color = colorPair->color;
		}
		if(colorPair->variable == gR.variable && colorPair->color != gR.color) {
			gR.color = colorPair->color;
		}
	}
	reduced = CSPInstance::copy(cspInstance);
	lemma12Case3Branch2And3Reduce(reduced, yR, fR, gR);

	result = solve(reduced);
	if(result == Result::Success) {
		lemma12Case3Branch2And3Color(cspInstance, reduced, yR, fR, gR);
	}
	delete reduced;
	return result;
}

void CSPSolver::lemma12Case3Branch1Reduce(CSPInstance* reduced, const ColorPair& xR, const ColorPair& yR) {
	std::vector<ColorPair> xConstraints = reduced->getConstraints(xR.variable, xR.color);
	for(auto constraint = xConstraints.begin(); constraint != xConstraints.end(); constraint++) {
		reduced->disableColor(constraint->variable, constraint->color);
	}
	std::vector<ColorPair> yConstraints = reduced->getConstraints(yR.variable, yR.color);
	for(auto constraint = yConstraints.begin(); constraint != yConstraints.end(); constraint++) {
		reduced->disableColor(constraint->variable, constraint->color);
	}
	reduced->removeVariable(xR.variable);
	reduced->removeVariable(yR.variable);
}

void CSPSolver::lemma12Case3Branch1Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& xR,
	const ColorPair& yR) {
	std::vector<int> removedVariables;
	removedVariables.push_back(xR.variable);
	removedVariables.push_back(yR.variable);
	cspInstance->copyColoring(reduced, removedVariables);
	cspInstance->setColor(xR.variable, xR.color);
	cspInstance->setColor(yR.variable, yR.color);
}

void CSPSolver::lemma12Case3Branch2And3Reduce(CSPInstance* reduced, const ColorPair& varR, const ColorPair& fR,
	const ColorPair& gR) {
	std::vector<ColorPair> xConstraints = reduced->getConstraints(varR.variable, varR.color);
	for(auto constraint = xConstraints.begin(); constraint != xConstraints.end(); constraint++) {
		reduced->disableColor(constraint->variable, constraint->color);
	}
	std::vector<ColorPair> fConstraints = reduced->getConstraints(fR.variable, fR.color);
	for(auto constraint = fConstraints.begin(); constraint != fConstraints.end(); constraint++) {
		reduced->disableColor(constraint->variable, constraint->color);
	}
	std::vector<ColorPair> gConstraints = reduced->getConstraints(gR.variable, gR.color);
	for(auto constraint = gConstraints.begin(); constraint != gConstraints.end(); constraint++) {
		reduced->disableColor(constraint->variable, constraint->color);
	}
	reduced->removeVariable(varR.variable);
	reduced->removeVariable(fR.variable);
	reduced->removeVariable(gR.variable);
}

void CSPSolver::lemma12Case3Branch2And3Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& varR,
	const ColorPair& fR, const ColorPair& gR) {
	std::vector<int> removedVariables;
	removedVariables.push_back(varR.variable);
	removedVariables.push_back(fR.variable);
	removedVariables.push_back(gR.variable);
	cspInstance->copyColoring(reduced, removedVariables);
	cspInstance->setColor(varR.variable, varR.color);
	cspInstance->setColor(fR.variable, fR.color);
	cspInstance->setColor(gR.variable, gR.color);
}
