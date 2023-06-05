#include "csp_solver.hpp"
#include <iostream>
#include <stdexcept>

const Lemma CSPSolver::lemmas[lemmasCount] = {
	&lemma1,
	&lemma2,
	&lemma3,
	&lemma4,
	&lemma5
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

Result CSPSolver::lemma1(CSPInstance* cspInstance) {
	std::cerr << "Lemma1" << std::endl;
	int v = -1;
	lemma1Match(cspInstance, v);
	if(v == -1) {
		return Result::NoMatch;
	}

	CSPInstance* reduced = CSPInstance::copy(cspInstance);
	std::vector<int> removedVariables;
	int R;
	int G;
	lemma1Reduce(reduced, removedVariables, R, G, v);

	Result result = solve(reduced);
	if(result == Result::Failure) {
		delete reduced;
		return Result::Failure;
	}

	lemma1Color(cspInstance, reduced, removedVariables, v, R, G);
	delete reduced;
	return Result::Success;
}

void CSPSolver::lemma1Match(const CSPInstance* cspInstance, int& v) {
	for(int variable = 0; variable < cspInstance->getVariableCount(); variable++) {
		if(cspInstance->getAvailableColors(variable).size() == 2) {
			v = variable;
			return;
		}
	}
}

void CSPSolver::lemma1Reduce(CSPInstance* reduced, std::vector<int>& removedVariables, int& R, int& G, int v) {
	std::vector<int> availableColors = reduced->getAvailableColors(v);
	R = availableColors[0];
	G = availableColors[1];
	std::vector<ColorPair> conflictR = reduced->getConstraints(v, R);
	std::vector<ColorPair> conflictG = reduced->getConstraints(v, G);

	for(int r = 0; r < conflictR.size(); r++) {
		for(int g = 0; g < conflictG.size(); g++) {
			reduced->addConstraint(conflictR[r].variable, conflictR[r].color, conflictG[g].variable, conflictG[g].color);
		}
	}
	
	removedVariables.push_back(v);
	reduced->removeVariable(v);
}

void CSPSolver::lemma1Color(CSPInstance* cspInstance, const CSPInstance* reduced, std::vector<int>& removedVariables,
	int v, int R, int G) {
	cspInstance->copyColoring(reduced, removedVariables);
	if(reduced->getConstraints(v, R).size() == 0) {
		cspInstance->setColor(v, R);
	} else {
		cspInstance->setColor(v, G);
	}
}

Result CSPSolver::lemma2(CSPInstance* cspInstance) {
	std::cerr << "Lemma2" << std::endl;
	ColorPair vX(-1, -1);
	ColorPair wY(-1, -1);
	lemma2Match(cspInstance, vX, wY);
	if(vX.variable == -1) {
		return Result::NoMatch;
	}

	CSPInstance* reduced = CSPInstance::copy(cspInstance);
	std::vector<int> removedVariables;
	lemma2Reduce(reduced, removedVariables, vX.variable, wY.variable);
	
	Result result = solve(reduced);
	if(result == Result::Failure) {
		delete reduced;
		return Result::Failure;
	}

	lemma2Color(cspInstance, reduced, removedVariables, vX, wY);
	delete reduced;
	return Result::Success;
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

void CSPSolver::lemma2Reduce(CSPInstance* reduced, std::vector<int>& removedVariables, int v, int w) {
	removedVariables.push_back(v);
	removedVariables.push_back(w);
	reduced->removeVariable(v);
	if(v < w) {
		w--;
	}
	reduced->removeVariable(w);
}

void CSPSolver::lemma2Color(CSPInstance* cspInstance, const CSPInstance* reduced, std::vector<int>& removedVariables,
	const ColorPair& vX, const ColorPair& wY) {
	cspInstance->copyColoring(cspInstance, removedVariables);
	cspInstance->setColor(vX.variable, vX.color);
	cspInstance->setColor(wY.variable, wY.color);
}

Result CSPSolver::lemma3(CSPInstance* cspInstance) {
	std::cerr << "Lemma3" << std::endl;
	ColorPair vB(-1, -1);
	lemma3Match(cspInstance, vB);
	if(vB.variable == -1) {
		return Result::NoMatch;
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
	std::cerr << "Lemma4" << std::endl;
	ColorPair vR(-1, -1);
	lemma4Match(cspInstance, vR);
	if(vR.variable == -1) {
		return Result::NoMatch;
	}

	CSPInstance* reduced = CSPInstance::copy(cspInstance);
	std::vector<int> removedVariables;
	lemma4Reduce(reduced, removedVariables, vR);

	Result result = solve(reduced);
	if(result == Result::Failure) {
		delete reduced;
		return Result::Failure;
	}

	lemma4Color(cspInstance, reduced, removedVariables, vR);
	delete reduced;
	return Result::Success;
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

void CSPSolver::lemma4Reduce(CSPInstance* reduced, std::vector<int>& removedVariables, const ColorPair& vR) {
	removedVariables.push_back(vR.variable);
	reduced->removeVariable(vR.variable);
}

void CSPSolver::lemma4Color(CSPInstance* cspInstance, const CSPInstance* reduced, std::vector<int>& removedVariables,
	const ColorPair& vR) {
	cspInstance->copyColoring(reduced, removedVariables);
	cspInstance->setColor(vR.variable, vR.color);
}

Result CSPSolver::lemma5(CSPInstance* cspInstance) {
	std::cerr << "Lemma5" << std::endl;
	return Result::Failure;
}
