#include "inner_csp_instance.hpp"

InnerCSPInstance::InnerCSPInstance(int variableCount) {
	this->variableCount = variableCount;
	variables = new Variable[variableCount];
	for(int i = 0; i < variableCount - 1; i++) {
		variables[i].setNext(&variables[i + 1]);
	}
	head = &variables[0];
	tail = &variables[variableCount - 1];
}

int InnerCSPInstance::getVertexCount() {
	return variableCount;
}

bool InnerCSPInstance::hasEdge(int start, int end) {
	if(start < 0 || start >= variableCount || end < 0 || end >= variableCount) {
		throw "InnerCSPInstance::hasEdge: Vertex index out of bounds";
	}
	return variables[start].hasEdge(variables[end]);
}

void InnerCSPInstance::addEdge(int start, int end) {
	if(start < 0 || start >= variableCount || end < 0 || end >= variableCount) {
		throw "InnerCSPInstance::addEdge: Vertex index out of bounds";
	}
	if(hasEdge(start, end)) {
		throw "InnerCSPInstance::addEdge: Edge already exists";
	}
	variables[start].addEdge(variables[end]);
}

void InnerCSPInstance::removeEdge(int start, int end) {
	if(start < 0 || start >= variableCount || end < 0 || end >= variableCount) {
		throw "InnerCSPInstance::removeEdge: Vertex index out of bounds";
	}
	if(!hasEdge(start, end)) {
		throw "InnerCSPInstance::removeEdge: Edge doesn't exist";
	}
	variables[start].removeEdge(variables[end]);
}
