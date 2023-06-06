#include "csp_solver.hpp"
#include <map>
#include <set>
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
	&lemma9
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
	std::vector<ColorPair> constraints = reduced->getConstraints(vR.variable, vR.color);
	for(auto constraint = constraints.begin(); constraint != constraints.end(); constraint++) {
		reduced->disableColor(constraint->variable, constraint->color);
	}
	reduced->removeVariable(vR.variable);
}

void CSPSolver::lemma0Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR) {
	std::vector<int> removedVariables;
	removedVariables.push_back(vR.variable);
	cspInstance->copyColoring(reduced, removedVariables);
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
		lemma7Branch2Color(cspInstance, reduced, vR, wR);
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
	std::vector<ColorPair> constraints = reduced->getConstraints(wR.variable, wR.color);
	for(auto constraint = constraints.begin(); constraint != constraints.end(); constraint++) {
		reduced->disableColor(constraint->variable, constraint->color);
	}
	reduced->removeVariable(wR.variable);
}

void CSPSolver::lemma7Branch2Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR,
	const ColorPair& wR) {
	std::vector<int> removedVariables;
	removedVariables.push_back(wR.variable);
	cspInstance->copyColoring(reduced, removedVariables);
	cspInstance->setColor(wR.variable, wR.color);
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
	std::vector<ColorPair> constraints = reduced->getConstraints(wR.variable, wR.color);
	for(auto constraint = constraints.begin(); constraint != constraints.end(); constraint++) {
		reduced->disableColor(constraint->variable, constraint->color);
	}
	reduced->removeVariable(wR.variable);
}

void CSPSolver::lemma8Case1Branch2Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& wR) {
	std::vector<int> removedVariables;
	removedVariables.push_back(wR.variable);
	cspInstance->copyColoring(reduced, removedVariables);
	cspInstance->setColor(wR.variable, wR.color);
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
		for(auto availableColor = availableColors.begin(); availableColor != availableColors.end(); availableColor++) {
		}
	}
}

void CSPSolver::lemma9Branch1Reduce(CSPInstance* reduced, const ColorPair& vR) {
	std::vector<ColorPair> constraints = reduced->getConstraints(vR.variable, vR.color);
	for(auto constraint = constraints.begin(); constraint != constraints.end(); constraint++) {
		reduced->disableColor(constraint->variable, constraint->color);
	}
	reduced->removeVariable(vR.variable);
}

void CSPSolver::lemma9Branch1Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR) {
	std::vector<int> removedVariables;
	removedVariables.push_back(vR.variable);
	cspInstance->copyColoring(reduced, removedVariables);
	cspInstance->setColor(vR.variable, vR.color);
}

void CSPSolver::lemma9Branch2Reduce(CSPInstance* reduced, const ColorPair& vR) {
	reduced->disableColor(vR.variable, vR.color);
}

void CSPSolver::lemma9Branch2Color(CSPInstance* cspInstance, const CSPInstance* reduced) {
	cspInstance->copyColoring(reduced, std::vector<int>());
}
