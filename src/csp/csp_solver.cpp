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
	&lemma11,
	&lemma12,
	&lemma13,
	&lemma14,
	&lemma15,
	&lemma16,
	&lemma17,
	&lemma18
};

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
		if(beVerbose) {
			std::cerr << "L0 ";
		}
		return Result::Success;
	}

	for(int variable = 0; variable < variableCount; variable++) {
		std::vector<int> availableColors = cspInstance->getAvailableColors(variable);
		if(availableColors.size() == 0) {
			if(beVerbose) {
				std::cerr << "L0 ";
			}
			return Result::Failure;
		} else if(availableColors.size() == 1) {
			if(beVerbose) {
				std::cerr << "L0 ";
			}
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
	if(beVerbose) {
		std::cerr << "L1 ";
	}

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
	std::vector<ColorPair> RConflicts = reduced->getConstraints(v, R);
	std::vector<ColorPair> GConflicts = reduced->getConstraints(v, G);
	for(auto RConflict = RConflicts.begin(); RConflict != RConflicts.end(); RConflict++) {
		for(auto GConflict = GConflicts.begin(); GConflict != GConflicts.end(); GConflict++) {
			reduced->addConstraint(RConflict->variable, RConflict->color, GConflict->variable, GConflict->color);
		}
	}
	reduced->removeVariable(v);
}

void CSPSolver::lemma1Color(CSPInstance* cspInstance, const CSPInstance* reduced, int v, int R, int G) {
	std::vector<int> removedVariables;
	removedVariables.push_back(v);
	cspInstance->copyColoring(reduced, removedVariables);
	if(cspInstance->getConstraints(v, R).size() == 0) {
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
	if(beVerbose) {
		std::cerr << "L2 ";
	}

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
	std::vector<int> removedVariables;
	removedVariables.push_back(v);
	removedVariables.push_back(w);
	reduced->removeVariables(removedVariables);
}

void CSPSolver::lemma2Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vX, const ColorPair& wY) {
	std::vector<int> removedVariables;
	removedVariables.push_back(vX.variable);
	removedVariables.push_back(wY.variable);
	cspInstance->copyColoring(reduced, removedVariables);
	cspInstance->setColor(vX.variable, vX.color);
	cspInstance->setColor(wY.variable, wY.color);
}

Result CSPSolver::lemma3(CSPInstance* cspInstance) {
	ColorPair vB(-1, -1);
	lemma3Match(cspInstance, vB);
	if(vB.variable == -1) {
		return Result::NoMatch;
	}
	if(beVerbose) {
		std::cerr << "L3 ";
	}

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
	if(beVerbose) {
		std::cerr << "L4 ";
	}

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
	if(beVerbose) {
		std::cerr << "L5 ";
	}

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
	if(beVerbose) {
		std::cerr << "L6 ";
	}

	int vDegree = (int)cspInstance->getAvailableColors(vR.variable).size();
	int wDegree = (int)cspInstance->getAvailableColors(wR.variable).size();
	if(vDegree == 3 && wDegree == 3) {
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
	ColorPair vG(vR.variable, -1);
	ColorPair vB(vR.variable, -1);
	ColorPair wG(wR.variable, -1);
	ColorPair wB(wR.variable, -1);
	lemma6Case1Reduce(reduced, vR, wR, vG, vB, wG, wB);

	Result result = solve(reduced);
	if(result == Result::Success) {
		lemma6Case1Color(cspInstance, reduced, vR, wR, vG, vB, wG, wB);
	}
	delete reduced;
	return result;
}

void CSPSolver::lemma6Case1Reduce(CSPInstance* reduced, const ColorPair& vR, const ColorPair& wR, ColorPair& vG,
	ColorPair& vB, ColorPair& wG, ColorPair& wB) {
	std::vector<int> vColors = reduced->getAvailableColors(vR.variable);
	for(auto vColor = vColors.begin(); vColor != vColors.end(); vColor++) {
		if(*vColor == vR.color) {
			continue;
		}
		if(vG.color == -1) {
			vG.color = *vColor;
			continue;
		}
		vB.color = *vColor;
		break;
	}
	std::vector<int> wColors = reduced->getAvailableColors(wR.variable);
	for(auto wColor = wColors.begin(); wColor != wColors.end(); wColor++) {
		if(*wColor == wR.color) {
			continue;
		}
		if(wG.color == -1) {
			wG.color = *wColor;
			continue;
		}
		wB.color = *wColor;
		break;
	}

	std::vector<ColorPair> vGConstraints = reduced->getConstraints(vG.variable, vG.color);
	std::vector<ColorPair> vBConstraints = reduced->getConstraints(vB.variable, vB.color);
	std::vector<ColorPair> wGConstraints = reduced->getConstraints(wG.variable, wG.color);
	std::vector<ColorPair> wBConstraints = reduced->getConstraints(wB.variable, wB.color);

	int u = reduced->getVariableCount();
	reduced->addVariable();
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

	std::vector<int> removedVariables;
	removedVariables.push_back(vR.variable);
	removedVariables.push_back(wR.variable);
	reduced->removeVariables(removedVariables);
}

void CSPSolver::lemma6Case1Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR,
	const ColorPair& wR, const ColorPair& vG, const ColorPair& vB, const ColorPair& wG, const ColorPair& wB) {
	std::vector<int> removedVariables;
	removedVariables.push_back(vR.variable);
	removedVariables.push_back(wR.variable);
	cspInstance->copyColoring(reduced, removedVariables);
	int u = reduced->getVariableCount() - 1;
	int uColor = reduced->getAvailableColors(u)[0];
	switch(uColor) {
	case 0:
		cspInstance->setColor(vG.variable, vG.color);
		cspInstance->setColor(wR.variable, wR.color);
		break;
	case 1:
		cspInstance->setColor(vB.variable, vB.color);
		cspInstance->setColor(wR.variable, wR.color);
		break;
	case 2:
		cspInstance->setColor(vR.variable, vR.color);
		cspInstance->setColor(wG.variable, wG.color);
		break;
	case 3:
		cspInstance->setColor(vR.variable, vR.color);
		cspInstance->setColor(wB.variable, wB.color);
		break;
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
	if(beVerbose) {
		std::cerr << "L7 ";
	}

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
	if(beVerbose) {
		std::cerr << "L8 ";
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
	if(beVerbose) {
		std::cerr << "L9 ";
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
	if(beVerbose) {
		std::cerr << "L10 ";
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
	if(beVerbose) {
		std::cerr << "L11 ";
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
	if(beVerbose) {
		std::cerr << "L12 ";
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
			std::unordered_set<int> variables;
			bipartite.clear();
			component.clear();
			stack.push(current);
			bipartite.insert(current);
			while(!stack.empty()) {
				current = stack.top();
				stack.pop();
				if(!visited[current.variable][current.color]) {
					visited[current.variable][current.color] = true;
					variables.insert(current.variable);
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
			if(variables.size() != 4 || (k != 8 && k != 12)) {
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

							for(int v = 0; v < n; v++) {
								delete [] visited[v];
							}
							delete [] visited;
							return;
						}
					}
				}
			}
		}
	}

	for(int v = 0; v < n; v++) {
		delete [] visited[v];
	}
	delete [] visited;
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
	std::vector<int> removedVariables;
	for(auto colorPair = bipartite.begin(); colorPair != bipartite.end(); colorPair++) {
		removedVariables.push_back(colorPair->variable);
	}
	reduced->removeVariables(removedVariables);
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
	std::vector<int> removedVariables;
	removedVariables.push_back(xR.variable);
	removedVariables.push_back(yR.variable);
	reduced->removeVariables(removedVariables);
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
	std::vector<int> removedVariables;
	removedVariables.push_back(varR.variable);
	removedVariables.push_back(fR.variable);
	removedVariables.push_back(gR.variable);
	reduced->removeVariables(removedVariables);
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

Result CSPSolver::lemma13(CSPInstance* cspInstance) {
	ColorPair zR(-1, -1);
	lemma13Match(cspInstance, zR);
	if(zR.variable == -1) {
		return Result::NoMatch;
	}
	if(beVerbose) {
		std::cerr << "L13 ";
	}

	CSPInstance* reduced = CSPInstance::copy(cspInstance);
	lemma13Branch1Reduce(reduced, zR);

	Result result = solve(reduced);
	if(result == Result::Success) {
		lemma13Branch1Color(cspInstance, reduced, zR);
		delete reduced;
		return Result::Success;
	}

	delete reduced;
	reduced = CSPInstance::copy(cspInstance);
	lemma13Branch2Reduce(reduced, zR);

	result = solve(reduced);
	if(result == Result::Success) {
		lemma13Branch1Color(cspInstance, reduced, zR);
	}
	delete reduced;
	return result;
}

void CSPSolver::lemma13Match(const CSPInstance* cspInstance, ColorPair& zR) {
	int n = cspInstance->getVariableCount();
	for(int v = 0; v < cspInstance->getVariableCount(); v++) {
		std::vector<int> availableColors = cspInstance->getAvailableColors(v);
		for(auto R = availableColors.begin(); R != availableColors.end(); R++) {
			bool** visited = new bool*[n];
			for(int var = 0; var < n; var++) {
				visited[var] = new bool[4];
				for(int R = 0; R < 4; R++) {
					visited[var][R] = false;
				}
			}

			std::vector<ColorPair> vConstraints = cspInstance->getConstraints(v, *R);
			if(vConstraints.size() != 3 || vConstraints[0].variable == v || vConstraints[1].variable == v ||
				vConstraints[2].variable == v) {
				continue;
			}
			ColorPair current(v, *R);
			std::stack<ColorPair> stack;
			std::unordered_set<int> variables;
			stack.push(current);
			while(!stack.empty()) {
				current = stack.top();
				stack.pop();
				if(!visited[current.variable][current.color]) {
					visited[current.variable][current.color] = true;
					variables.insert(current.variable);
					std::vector<ColorPair> constraints = cspInstance->getConstraints(current.variable, current.color);
					for(auto constraint = constraints.begin(); constraint != constraints.end(); constraint++) {
						stack.push(*constraint);
					}
				}
			}
			if(variables.size() == 4) {
				continue;
			}
			for(auto wR = vConstraints.begin(); wR != vConstraints.end(); wR++) {
				std::vector<ColorPair> wConstraints = cspInstance->getConstraints(wR->variable, wR->color);
				for(auto wConstraint = wConstraints.begin(); wConstraint != wConstraints.end(); wConstraint++) {
					bool isZ = true;
					for(auto vConstraint = vConstraints.begin(); vConstraint != vConstraints.end(); vConstraint++) {
						if(wConstraint->variable == vConstraint->variable) {
							isZ = false;
						}
					}
					if(wConstraint->variable == v) {
						isZ = false;
					}
					if(isZ) {
						zR.variable = wConstraint->variable;
						zR.color = wConstraint->color;

						for(int var = 0; var < n; var++) {
							delete [] visited[var];
						}
						delete [] visited;
						return;
					}
				}
			}

			for(int var = 0; var < n; var++) {
				delete [] visited[var];
			}
			delete [] visited;
		}
	}
}

void CSPSolver::lemma13Branch1Reduce(CSPInstance* reduced, const ColorPair& zR) {
	chooseColorReduce(reduced, zR);
}

void CSPSolver::lemma13Branch1Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& zR) {
	chooseColorColor(cspInstance, reduced, zR);
}

void CSPSolver::lemma13Branch2Reduce(CSPInstance* reduced, const ColorPair& zR) {
	reduced->disableColor(zR.variable, zR.color);
}

void CSPSolver::lemma13Branch2Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& zR) {
	cspInstance->copyColoring(reduced, std::vector<int>());
}

Result CSPSolver::lemma14(CSPInstance* cspInstance) {
	ColorPair vR(-1, -1);
	ColorPair wR(-1, -1);
	ColorPair xR(-1, -1);
	ColorPair yR(-1, -1);
	lemma14Match(cspInstance, vR, wR, xR, yR);
	if(vR.variable == -1) {
		return Result::NoMatch;
	}
	if(beVerbose) {
		std::cerr << "L14 ";
	}

	CSPInstance* reduced = CSPInstance::copy(cspInstance);
	lemma14Branch1Reduce(reduced, wR);

	Result result = solve(reduced);
	if(result == Result::Success) {
		lemma14Branch1Color(cspInstance, reduced, wR);
		delete reduced;
		return Result::Success;
	}
	delete reduced;

	reduced = CSPInstance::copy(cspInstance);
	lemma14Branch2Reduce(reduced, xR);

	result = solve(reduced);
	if(result == Result::Success) {
		lemma14Branch2Color(cspInstance, reduced, xR);
		delete reduced;
		return Result::Success;
	}
	delete reduced;

	reduced = CSPInstance::copy(cspInstance);
	lemma14Branch3Reduce(reduced, vR, yR);

	result = solve(reduced);
	if(result == Result::Success) {
		lemma14Branch3Color(cspInstance, reduced, vR, yR);
	}
	delete reduced;
	return result;
}

void CSPSolver::lemma14Match(const CSPInstance* cspInstance, ColorPair& vR, ColorPair& wR, ColorPair& xR, ColorPair& yR) {
	ColorPair sequence[5] {
		ColorPair(-1, -1),
		ColorPair(-1, -1),
		ColorPair(-1, -1),
		ColorPair(-1, -1),
		ColorPair(-1, -1)
	};
	
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
			std::vector<ColorPair> vConstraints = cspInstance->getConstraints(v, *R);
			if(vConstraints.size() != 2 || visited[v][*R]) {
				continue;
			}
			ColorPair start(v, *R);

			sequence[4] = start;
			sequence[3] = cspInstance->getConstraints(sequence[4].variable, sequence[4].color)[0];
			for(int i = 2; i >= 0; i--) {
				std::vector<ColorPair> constraints = cspInstance->getConstraints(sequence[i + 1].variable,
					sequence[i + 1].color);
				sequence[i] = constraints[0] != sequence[i + 2] ? constraints[0] : constraints[1];
			}
			do {
				bool areDistinct = true;
				for(int j = 0; j < 4; j++) {
					for(int k = j + 1; k < 4; k++) {
						if(sequence[j].variable == sequence[k].variable) {
							areDistinct = false;
						}
					}
				}
				if(areDistinct) {
					if(cspInstance->getAvailableColors(sequence[4].variable).size() == 4) {
						vR = sequence[4];
						wR = sequence[3];
						xR = sequence[2];
						yR = sequence[1];
					} else {
						vR = sequence[0];
						wR = sequence[1];
						xR = sequence[2];
						yR = sequence[3];
					}

					for(int v = 0; v < n; v++) {
						delete [] visited[v];
					}
					delete [] visited;
					return;
				}

				for(int i = 4; i >= 1; i--) {
					sequence[i] = sequence[i - 1];
				}
				std::vector<ColorPair> constraints = cspInstance->getConstraints(sequence[0].variable, sequence[0].color);
				sequence[0] = constraints[0] != sequence[2] ? constraints[0] : constraints[1];
				visited[sequence[0].variable][sequence[0].color] = true;
			} while(sequence[4] != start);
		}
	}

	for(int v = 0; v < n; v++) {
		delete [] visited[v];
	}
	delete [] visited;
}

void CSPSolver::lemma14Branch1Reduce(CSPInstance* reduced, const ColorPair& wR) {
	chooseColorReduce(reduced, wR);
}

void CSPSolver::lemma14Branch1Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& wR) {
	chooseColorColor(cspInstance, reduced, wR);
}

void CSPSolver::lemma14Branch2Reduce(CSPInstance* reduced, const ColorPair& xR) {
	chooseColorReduce(reduced, xR);
}

void CSPSolver::lemma14Branch2Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& xR) {
	chooseColorColor(cspInstance, reduced, xR);
}

void CSPSolver::lemma14Branch3Reduce(CSPInstance* reduced, const ColorPair& vR, const ColorPair& yR) {
	std::vector<ColorPair> xConstraints = reduced->getConstraints(vR.variable, vR.color);
	for(auto constraint = xConstraints.begin(); constraint != xConstraints.end(); constraint++) {
		reduced->disableColor(constraint->variable, constraint->color);
	}
	std::vector<ColorPair> yConstraints = reduced->getConstraints(yR.variable, yR.color);
	for(auto constraint = yConstraints.begin(); constraint != yConstraints.end(); constraint++) {
		reduced->disableColor(constraint->variable, constraint->color);
	}
	std::vector<int> removedVariables;
	removedVariables.push_back(vR.variable);
	removedVariables.push_back(yR.variable);
	reduced->removeVariables(removedVariables);
}

void CSPSolver::lemma14Branch3Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR,
	const ColorPair& yR) {
	std::vector<int> removedVariables;
	removedVariables.push_back(vR.variable);
	removedVariables.push_back(yR.variable);
	cspInstance->copyColoring(reduced, removedVariables);
	cspInstance->setColor(vR.variable, vR.color);
	cspInstance->setColor(yR.variable, yR.color);
}

Result CSPSolver::lemma15(CSPInstance* cspInstance) {
	ColorPair wR(-1, -1);
	ColorPair xR(-1, -1);
	lemma15Match(cspInstance, wR, xR);
	if(wR.variable == -1) {
		return Result::NoMatch;
	}
	if(beVerbose) {
		std::cerr << "L15 ";
	}

	CSPInstance* reduced = CSPInstance::copy(cspInstance);
	lemma15Branch1Reduce(reduced, wR);

	Result result = solve(reduced);
	if(result == Result::Success) {
		lemma15Branch1Color(cspInstance, reduced, wR);
		delete reduced;
		return Result::Success;
	}
	delete reduced;

	reduced = CSPInstance::copy(cspInstance);
	lemma15Branch2Reduce(reduced, xR);

	result = solve(reduced);
	if(result == Result::Success) {
		lemma15Branch2Color(cspInstance, reduced, xR);
	}
	delete reduced;
	return result;
}

void CSPSolver::lemma15Match(const CSPInstance* cspInstance, ColorPair& wR, ColorPair& xR) {
	ColorPair sequence[4] {
		ColorPair(-1, -1),
		ColorPair(-1, -1),
		ColorPair(-1, -1),
		ColorPair(-1, -1)
	};
	
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
			std::vector<ColorPair> vConstraints = cspInstance->getConstraints(v, *R);
			if(vConstraints.size() != 2 || visited[v][*R]) {
				continue;
			}
			ColorPair start(v, *R);

			sequence[3] = start;
			sequence[2] = cspInstance->getConstraints(sequence[3].variable, sequence[3].color)[0];
			for(int i = 1; i >= 0; i--) {
				std::vector<ColorPair> constraints = cspInstance->getConstraints(sequence[i + 1].variable,
					sequence[i + 1].color);
				sequence[i] = constraints[0] != sequence[i + 2] ? constraints[0] : constraints[1];
			}
			do {
				bool match = true;
				for(int j = 0; j < 2; j++) {
					for(int k = j + 1; k < 2; k++) {
						if(sequence[j].variable == sequence[k].variable) {
							match = false;
						}
					}
				}
				if(sequence[0].variable != sequence[3].variable) {
					match = false;
				}
				if(match) {
					wR = sequence[1];
					xR = sequence[2];

					for(int v = 0; v < n; v++) {
						delete [] visited[v];
					}
					delete [] visited;
					return;
				}

				for(int i = 3; i >= 1; i--) {
					sequence[i] = sequence[i - 1];
				}
				std::vector<ColorPair> constraints = cspInstance->getConstraints(sequence[0].variable, sequence[0].color);
				sequence[0] = constraints[0] != sequence[2] ? constraints[0] : constraints[1];
				visited[sequence[0].variable][sequence[0].color] = true;
			} while(sequence[3] != start);
		}
	}

	for(int v = 0; v < n; v++) {
		delete [] visited[v];
	}
	delete [] visited;
}

void CSPSolver::lemma15Branch1Reduce(CSPInstance* reduced, const ColorPair& wR) {
	chooseColorReduce(reduced, wR);
}

void CSPSolver::lemma15Branch1Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& wR) {
	chooseColorColor(cspInstance, reduced, wR);
}

void CSPSolver::lemma15Branch2Reduce(CSPInstance* reduced, const ColorPair& xR) {
	chooseColorReduce(reduced, xR);
}

void CSPSolver::lemma15Branch2Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& xR) {
	chooseColorColor(cspInstance, reduced, xR);
}

Result CSPSolver::lemma16(CSPInstance* cspInstance) {
	ColorPair vR(-1, -1);
	lemma16Match(cspInstance, vR);
	if(vR.variable == -1) {
		return Result::NoMatch;
	}
	if(beVerbose) {
		std::cerr << "L16 ";
	}

	CSPInstance* reduced = CSPInstance::copy(cspInstance);
	lemma16Branch1Reduce(reduced, vR);

	Result result = solve(reduced);
	if(result == Result::Success) {
		lemma16Branch1Color(cspInstance, reduced, vR);
		delete reduced;
		return Result::Success;
	}
	delete reduced;

	reduced = CSPInstance::copy(cspInstance);
	lemma16Branch2Reduce(reduced, vR);

	result = solve(reduced);
	if(result == Result::Success) {
		lemma16Branch2Color(cspInstance, reduced, vR);
	}
	delete reduced;
	return result;
}

void CSPSolver::lemma16Match(const CSPInstance* cspInstance, ColorPair& vR) {
	for(int v = 0; v < cspInstance->getVariableCount(); v++) {
		std::vector<int> availableColors = cspInstance->getAvailableColors(v);
		for(auto R = availableColors.begin(); R != availableColors.end(); R++) {
			std::vector<ColorPair> vConstraints = cspInstance->getConstraints(v, *R);
			if(vConstraints.size() != 2) {
				continue;
			}
			int cycleLength = 1;
			ColorPair end(vConstraints[0].variable, vConstraints[0].color);
			ColorPair current(v, *R);
			ColorPair previous = end;
			while(current != end) {
				std::vector<ColorPair> constraints = cspInstance->getConstraints(current.variable, current.color);
				ColorPair tmp = current;
				current = constraints[0] != previous ? constraints[0] : constraints[1];
				previous = tmp;
				cycleLength++;
			}
			if(cycleLength > 3) {
				vR.variable = v;
				vR.color = *R;
				return;
			}
		}
	}
}

void CSPSolver::lemma16Branch1Reduce(CSPInstance* reduced, const ColorPair& vR) {
	chooseColorReduce(reduced, vR);
}

void CSPSolver::lemma16Branch1Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR) {
	chooseColorColor(cspInstance, reduced, vR);
}

void CSPSolver::lemma16Branch2Reduce(CSPInstance* reduced, const ColorPair& vR) {
	reduced->disableColor(vR.variable, vR.color);
}

void CSPSolver::lemma16Branch2Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR) {
	cspInstance->copyColoring(reduced, std::vector<int>());
}

Result CSPSolver::lemma17(CSPInstance* cspInstance) {
	Graph* bipartiteGraph = nullptr;

	int n = cspInstance->getVariableCount();
	int** variableColorComponent = new int*[n];
	for(int v = 0; v < n; v++) {
		variableColorComponent[v] = new int[4];
		for(int R = 0; R < 4; R++) {
			variableColorComponent[v][R] = -1;
		}
	}

	lemma17Match(cspInstance, &bipartiteGraph, variableColorComponent);
	if(bipartiteGraph == nullptr) {
		for(int v = 0; v < n; v++) {
			delete [] variableColorComponent[v];
		}
		delete [] variableColorComponent;
		return Result::NoMatch;
	}
	if(beVerbose) {
		std::cerr << "L17 ";
	}

	std::vector<std::pair<int, int>> variableComponent;
	Result result = lemma17Solve(bipartiteGraph, n, variableComponent);
	if(result == Result::Success) {
		lemma17Color(cspInstance, bipartiteGraph, variableColorComponent, variableComponent);
	}

	for(int v = 0; v < n; v++) {
		delete [] variableColorComponent[v];
	}
	delete [] variableColorComponent;
	delete bipartiteGraph;
	return result;
}

void CSPSolver::lemma17Match(const CSPInstance* cspInstance, Graph** bipartiteGraph, int** variableColorComponent) {
	int n = cspInstance->getVariableCount();
	Graph* graph = Graph::create(n);

	bool** visited = new bool*[n];
	for(int v = 0; v < n; v++) {
		visited[v] = new bool[4];
		for(int R = 0; R < 4; R++) {
			visited[v][R] = false;
		}
	}

	for(int v = 0; v < n; v++) {
		std::vector<int> availableColors = cspInstance->getAvailableColors(v);
		for(auto R = availableColors.begin(); R != availableColors.end(); R++) {
			if(visited[v][*R]) {
				continue;
			}
			int degree = cspInstance->getConstraints(v, *R).size();
			int count = 0;
			int newVertex = graph->getVertexCount();
			graph->addVertex();
			ColorPair current(v, *R);
			std::stack<ColorPair> stack;
			stack.push(current);
			while(!stack.empty()) {
				current = stack.top();
				stack.pop();
				if(!visited[current.variable][current.color]) {
					visited[current.variable][current.color] = true;
					variableColorComponent[current.variable][current.color] = newVertex;
					graph->addEdge(newVertex, current.variable);
					count++;
					std::vector<ColorPair> constraints = cspInstance->getConstraints(current.variable, current.color);
					if(constraints.size() != degree) {
						for(int v = 0; v < n; v++) {
							delete [] visited[v];
						}
						delete [] visited;
						delete graph;
						return;
					}
					for(auto constraint = constraints.begin(); constraint != constraints.end(); constraint++) {
						stack.push(*constraint);
					}
				}
			}
			if(!(degree == 2 && count == 3) && !(degree == 3 && count == 4)) {
				for(int v = 0; v < n; v++) {
					delete [] visited[v];
				}
				delete [] visited;
				delete graph;
				return;
			}
		}
	}

	for(int v = 0; v < n; v++) {
		delete [] visited[v];
	}
	delete [] visited;
	*bipartiteGraph = graph;
}

Result CSPSolver::lemma17Solve(Graph* bipartiteGraph, int variableCount, std::vector<std::pair<int, int>>& variableComponent) {
	int source = bipartiteGraph->getVertexCount();
	bipartiteGraph->addVertex();
	int sink = bipartiteGraph->getVertexCount();
	bipartiteGraph->addVertex();
	int n = bipartiteGraph->getVertexCount();

	for(int i = 0; i < variableCount; i++) {
		bipartiteGraph->addEdge(source, i);
	}
	for(int i = variableCount; i < source; i++) {
		bipartiteGraph->addEdge(i, sink);
	}

	int** flow = new int*[n];
	for(int i = 0; i < n; i++) {
		flow[i] = new int[n];
		for(int j = 0; j < n; j++) {
			flow[i][j] = 0;
		}
	}

	int** residual = new int*[n];
	for(int i = 0; i < n; i++) {
		residual[i] = new int[n];
		for(int j = 0; j < n; j++) {
			residual[i][j] = 0;
		}
	}
	for(int variable = 0; variable < variableCount; variable++) {
		residual[source][variable] = 1;
		for(int component = variableCount; component < source; component++) {
			residual[variable][component] = 1;
		}
	}
	for(int component = variableCount; component < source; component++) {
		residual[component][sink] = 1;
	}

	std::vector<int> residualPath;
	while((residualPath = lemma17SolveFindPath(bipartiteGraph, residual)).size() >= 2) {
		for(auto vertex = residualPath.begin(); vertex + 1 != residualPath.end(); vertex++) {
			if(flow[*(vertex + 1)][*vertex] == 0) {
				flow[*vertex][*(vertex + 1)] = 1;
			} else {
				flow[*(vertex + 1)][*vertex] = 0;
			}
			residual[*vertex][*(vertex + 1)] = 0;
			residual[*(vertex + 1)][*vertex] = 1;
		}
	}

	int flowValue = 0;
	for(int component = variableCount; component < source; component++) {
		flowValue += flow[component][sink];
	}
	if(flowValue == variableCount) {
		for(int variable = 0; variable < variableCount; variable++) {
			for(int component = variableCount; component < source; component++) {
				if(flow[variable][component] == 1) {
					variableComponent.push_back(std::pair<int, int>(variable, component));
					break;
				}
			}
		}
	}

	for(int i = 0; i < n; i++) {
		delete [] flow[i];
	}
	delete [] flow;

	for(int i = 0; i < n; i++) {
		delete [] residual[i];
	}
	delete [] residual;

	return flowValue == variableCount ? Result::Success : Result::Failure;
}

std::vector<int> CSPSolver::lemma17SolveFindPath(Graph* bipartiteGraph, int** residual) {
	int n = bipartiteGraph->getVertexCount();
	int source = n - 2;
	int sink = n - 1;
	int* visited = new int[n];
	for(int i = 0; i < n; i++) {
		visited[i] = false;
	}

	int current = source;
	int parent = -1;
	std::stack<int> stack;
	std::stack<int> parentStack;
	std::stack<int> pathStack;
	stack.push(current);
	parentStack.push(parent);
	pathStack.push(parent);
	while(!stack.empty()) {
		current = stack.top();
		stack.pop();
		parent = parentStack.top();
		parentStack.pop();
		if(!visited[current]) {
			visited[current] = true;
			while(pathStack.top() != parent) {
				pathStack.pop();
			}
			pathStack.push(current);
			if(current == sink) {
				std::vector<int> path;
				while(pathStack.top() != -1) {
					path.push_back(pathStack.top());
					pathStack.pop();
				}

				delete [] visited;
				std::reverse(path.begin(), path.end());
				return path;
			}
			std::vector<int> neighbors = bipartiteGraph->getNeighbors(current);
			for(auto neighbor = neighbors.begin(); neighbor != neighbors.end(); neighbor++) {
				if(residual[current][*neighbor] == 1) {
					stack.push(*neighbor);
					parentStack.push(current);
				}
			}
		}
	}

	delete [] visited;
	return std::vector<int>();
}

void CSPSolver::lemma17Color(CSPInstance* cspInstance, const Graph* bipartiteGraph, int** variableColorComponent,
	std::vector<std::pair<int, int>>& variableComponent) {
	for(auto vC = variableComponent.begin(); vC != variableComponent.end(); vC++) {
		for(int i = 0; i < 4; i++) {
			if(variableColorComponent[vC->first][i] == vC->second) {
				cspInstance->setColor(vC->first, i);
				break;
			}
		}
	}
}

Result CSPSolver::lemma18(CSPInstance* cspInstance) {
	ColorPair vR(-1, -1);
	lemma18Match(cspInstance, vR);
	if(beVerbose) {
		std::cerr << "L18 ";
	}
	
	CSPInstance* reduced = CSPInstance::copy(cspInstance);
	lemma18Branch1Reduce(reduced, vR);

	Result result = solve(reduced);
	if(result == Result::Success) {
		lemma18Branch1Color(cspInstance, reduced, vR);
		delete reduced;
		return Result::Success;
	}

	delete reduced;
	reduced = CSPInstance::copy(cspInstance);
	lemma18Branch2Reduce(reduced, vR);

	result = solve(reduced);
	if(result == Result::Success) {
		lemma18Branch2Color(cspInstance, reduced);
	}
	delete reduced;
	return result;
}

void CSPSolver::lemma18Match(const CSPInstance* cspInstance, ColorPair& vR) {
	vR.variable = 0;
	vR.color = cspInstance->getAvailableColors(vR.variable)[0];
}

void CSPSolver::lemma18Branch1Reduce(CSPInstance* reduced, const ColorPair& vR) {
	chooseColorReduce(reduced, vR);
}

void CSPSolver::lemma18Branch1Color(CSPInstance* cspInstance, const CSPInstance* reduced, const ColorPair& vR) {
	chooseColorColor(cspInstance, reduced, vR);
}

void CSPSolver::lemma18Branch2Reduce(CSPInstance* reduced, const ColorPair& vR) {
	reduced->disableColor(vR.variable, vR.color);
}

void CSPSolver::lemma18Branch2Color(CSPInstance* cspInstance, const CSPInstance* reduced) {
	cspInstance->copyColoring(reduced, std::vector<int>());
}

bool CSPSolver::beVerbose = false;

Result CSPSolver::solve(CSPInstance* cspInstance) {
	for(int lemma = 0; lemma < lemmasCount; lemma++) {
		Result result = (*lemmas[lemma])(cspInstance);
		if(result != Result::NoMatch) {
			return result;
		}
	}
	const char* errorMsg = "solve: None of the lemmas matched ";
	std::cerr << errorMsg << std::endl;
	throw std::logic_error(errorMsg);
}
