#include "inner_csp_instance.hpp"
#include <iostream>
#include <algorithm>
#include <stdexcept>

namespace InnerRepresentation {
	InnerCSPInstance::InnerCSPInstance(int variableCount) : variables(variableCount) {
		for(int variable = 0; variable < variables.size(); variable++) {
			variables[variable] = new Variable(variable, InitialType::Vertex);
		}
	}

	InnerCSPInstance::InnerCSPInstance(const InnerCSPInstance& cspInstance) :
		InnerCSPInstance(cspInstance.getVariableCount()) {
		for(int variable = 0; variable < variables.size(); variable++) {
			std::vector<ConstraintInfo> constraints = cspInstance.variables[variable]->getConstraints();
			for(int constraint = 0; constraint < constraints.size(); constraint++) {
				if(!hasConstraint(constraints[constraint].startVariable, constraints[constraint].startColor,
					constraints[constraint].endVariable, constraints[constraint].endColor)) {
					addConstraint(constraints[constraint].startVariable, constraints[constraint].startColor,
						constraints[constraint].endVariable, constraints[constraint].endColor);
				}
			}
		}
	}

	void InnerCSPInstance::copyColoring(const InnerCSPInstance* reducedCSPInstance, std::vector<int> removedVariables) {
		std::sort(removedVariables.begin(), removedVariables.end());
		int original = 0;
		int reduced = 0;
		int removed = 0;
		while(original < variables.size()) {
			if(removed < removedVariables.size() && removedVariables[removed] == original) {
				removed++;
			} else {
				std::vector<int> availableColors = reducedCSPInstance->getAvailableColors(reduced);
				if(availableColors.size() != 1) {
					error("copyColoring: Reduced instance isn't properly colored");
				}
				variables[original]->setColor(availableColors[0]);
				reduced++;
			}
			original++;
		}
	}

	void InnerCSPInstance::error(const char* errorMsg) const {
		std::cerr << errorMsg << std::endl;
		throw std::invalid_argument(errorMsg);
	}

	InnerCSPInstance::~InnerCSPInstance() {
		for(int variable = 0; variable < variables.size(); variable++) {
			delete variables[variable];
		}
	}

	void InnerCSPInstance::copyColoring(const CSPInstance* reducedCSPInstance, std::vector<int> removedVariables) {
		copyColoring((const InnerCSPInstance*) reducedCSPInstance, removedVariables);
	}

	int InnerCSPInstance::getVariableCount() const {
		return (int)variables.size();
	}

	std::vector<ColorPair> InnerCSPInstance::getConstraints(int variable, int color) const {
		return variables[variable]->getConstraints(color);
	}

	void InnerCSPInstance::addVariable() {
		variables.push_back(new Variable((int)variables.size(), InitialType::Variable));
	}

	void InnerCSPInstance::removeVariable(int variable) {
		if(variable < 0 || variable >= variables.size()) {
			error("removeVariable: Variable index out of bounds");
		}
		delete variables[variable];
		variables.erase(variables.begin() + variable);
		for(int updatedVariable = variable; updatedVariable < variables.size(); updatedVariable++) {
			variables[updatedVariable]->setIndex(updatedVariable);
		}
	}

	bool InnerCSPInstance::hasConstraint(int startVariable, int startColor, int endVariable, int endColor) const {
		if(startVariable < 0 || startVariable >= variables.size() || endVariable < 0 || endVariable >= variables.size()) {
			error("hasConstraint: Variable index out of bounds");
		}
		if(startColor < 0 || startColor >= 4 || endColor < 0 || endColor >= 4) {
			error("hasConstraint: Color index of bounds");
		}
		return variables[startVariable]->hasConstraint(startColor, *variables[endVariable], endColor);
	}

	void InnerCSPInstance::addConstraint(int startVariable, int startColor, int endVariable, int endColor) {
		if(startVariable < 0 || startVariable >= variables.size() || endVariable < 0 || endVariable >= variables.size()) {
			error("addConstraint: Variable index out of bounds");
		}
		if(startColor < 0 || startColor >= 4 || endColor < 0 || endColor >= 4) {
			error("addConstraint: Color index of bounds");
		}
		if(hasConstraint(startVariable, startColor, endVariable, endColor)) {
			error("addConstraint: Constraint already exists");
		}
		variables[startVariable]->addConstraint(startColor, *variables[endVariable], endColor);
	}

	void InnerCSPInstance::removeConstraint(int startVariable, int startColor, int endVariable, int endColor) {
		if(startVariable < 0 || startVariable >= variables.size() || endVariable < 0 || endVariable >= variables.size()) {
			error("removeConstraint: Variable index out of bounds");
		}
		if(startColor < 0 || startColor >= 4 || endColor < 0 || endColor >= 4) {
			error("removeConstraint: Color index of bounds");
		}
		if(!hasConstraint(startVariable, startColor, endVariable, endColor)) {
			error("removeConstraint: Constraint doesn't exist");
		}
		variables[startVariable]->removeConstraint(startColor, *variables[endVariable], endColor);
	}

	void InnerCSPInstance::copyColoring(const Graph* reducedGraph, std::vector<int> removedVertices) {
		copyColoring((const InnerCSPInstance*)reducedGraph, removedVertices);
	}

	int InnerCSPInstance::getVertexCount() const {
		return (int)variables.size();
	}

	std::vector<int> InnerCSPInstance::getNeighbors(int vertex) const {
		if(vertex < 0 || vertex >= variables.size()) {
			error("getNeighbors: Vertex index out of bounds");
		}
		return variables[vertex]->getNeighbors();
	}

	void InnerCSPInstance::addVertex() {
		variables.push_back(new Variable((int)variables.size(), InitialType::Vertex));
	}

	void InnerCSPInstance::removeVertex(int vertex) {
		removeVariable(vertex);
	}

	bool InnerCSPInstance::hasEdge(int start, int end) const {
		if(start < 0 || start >= variables.size() || end < 0 || end >= variables.size()) {
			error("hasEdge: Vertex index out of bounds");
		}
		return variables[start]->hasEdge(*variables[end]);
	}

	void InnerCSPInstance::addEdge(int start, int end) {
		if(start < 0 || start >= variables.size() || end < 0 || end >= variables.size()) {
			error("addEdge: Vertex index out of bounds");
		}
		if(hasEdge(start, end)) {
			error("addEdge: Edge already exists");
		}
		variables[start]->addEdge(*variables[end]);
	}

	void InnerCSPInstance::removeEdge(int start, int end) {
		if(start < 0 || start >= variables.size() || end < 0 || end >= variables.size()) {
			error("removeEdge: Vertex index out of bounds");
		}
		if(!hasEdge(start, end)) {
			error("removeEdge: Edge doesn't exist");
		}
		variables[start]->removeEdge(*variables[end]);
	}

	bool InnerCSPInstance::isColorAvailable(int variable, int color) const {
		if(variable < 0 || variable >= variables.size()) {
			error("isColorAvailable: Vertex index out of bounds");
		}
		if(color < 0 || color >= 4) {
			error("isColorAvailable: Color index of bounds");
		}
		return variables[variable]->isColorAvailable(color);
	}

	std::vector<int> InnerCSPInstance::getAvailableColors(int variable) const {
		return variables[variable]->getAvailableColors();
	}

	void InnerCSPInstance::disableColor(int variable, int color) {
		if(variable < 0 || variable >= variables.size()) {
			error("disableColor: Vertex index out of bounds");
		}
		if(color < 0 || color >= 4) {
			error("disableColor: Color index of bounds");
		}
		if(!isColorAvailable(variable, color)) {
			error("disableColor: Color is unavailable");
		}
		variables[variable]->disableColor(color);
	}

	void InnerCSPInstance::setColor(int variable, int color) {
		if(variable < 0 || variable >= variables.size()) {
			error("setColor: Vertex index out of bounds");
		}
		if(color < 0 || color >= 4) {
			error("setColor: Color index of bounds");
		}
		if(!isColorAvailable(variable, color)) {
			error("setColor: Color is unavailable");
		}
		variables[variable]->setColor(color);
	}
}
