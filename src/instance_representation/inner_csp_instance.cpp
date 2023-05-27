#include "inner_csp_instance.hpp"

InnerCSPInstance::InnerCSPInstance(int variableCount) : variables(variableCount) {
	for(int i = 0; i < variables.size(); i++) {
		variables[i].setIndex(i);
	}
}

InnerCSPInstance::InnerCSPInstance(const InnerCSPInstance& cspInstance) : InnerCSPInstance(cspInstance.getVariableCount()) {
	for(int variable = 0; variable < variables.size(); variable++) {
		std::vector<int> neighbors = cspInstance.variables[variable].getNeighbors();
		for(int neighbor = 0; neighbor < neighbors.size(); neighbor++) {
			this->addEdge(variable, neighbor);
		}
	}
}

int InnerCSPInstance::getVariableCount() const {
	return variables.size();
}

int InnerCSPInstance::getVertexCount() const {
	return variables.size();
}

std::vector<int> InnerCSPInstance::getNeighbors(int vertex) const {
	if(vertex < 0 || vertex >= variables.size()) {
		throw "InnerCSPInstance::getNeighbors: Vertex doesn't exist";
	}
	return variables[vertex].getNeighbors();
}

void InnerCSPInstance::removeVertex(int vertex) {
	if(vertex < 0 || vertex >= variables.size()) {
		throw "InnerCSPInstance::removeVertex: Vertex doesn't exist";
	}
	variables.erase(variables.begin() + vertex);
	for(int i = vertex; i < variables.size(); i++) {
		variables[i].setIndex(i);
	}
}

bool InnerCSPInstance::hasEdge(int start, int end) {
	if(start < 0 || start >= variables.size() || end < 0 || end >= variables.size()) {
		throw "InnerCSPInstance::hasEdge: Vertex index out of bounds";
	}
	return variables[start].hasEdge(variables[end]);
}

void InnerCSPInstance::addEdge(int start, int end) {
	if(start < 0 || start >= variables.size() || end < 0 || end >= variables.size()) {
		throw "InnerCSPInstance::addEdge: Vertex index out of bounds";
	}
	if(hasEdge(start, end)) {
		throw "InnerCSPInstance::addEdge: Edge already exists";
	}
	variables[start].addEdge(variables[end]);
}

void InnerCSPInstance::removeEdge(int start, int end) {
	if(start < 0 || start >= variables.size() || end < 0 || end >= variables.size()) {
		throw "InnerCSPInstance::removeEdge: Vertex index out of bounds";
	}
	if(!hasEdge(start, end)) {
		throw "InnerCSPInstance::removeEdge: Edge doesn't exist";
	}
	variables[start].removeEdge(variables[end]);
}
