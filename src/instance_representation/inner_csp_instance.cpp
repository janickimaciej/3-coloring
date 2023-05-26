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

int InnerCSPInstance::GetVertexCount() {
	return variableCount;
}

bool InnerCSPInstance::HasEdge(int start, int end) {
	if(start < 0 || start >= variableCount || end < 0 || end >= variableCount) {
		throw "HasEdge: Vertex index out of bounds";
	}
	return false;
}

void InnerCSPInstance::AddEdge(int start, int end) {
	if(start < 0 || start >= variableCount || end < 0 || end >= variableCount) {
		throw "AddEdge: Vertex index out of bounds";
	}
	if(HasEdge(start, end)) {
		throw "AddEdge: Edge already exists";
	}
}

void InnerCSPInstance::RemoveEdge(int start, int end) {
	if(start < 0 || start >= variableCount || end < 0 || end >= variableCount) {
		throw "RemoveEdge: Vertex index out of bounds";
	}
	if(!HasEdge(start, end)) {
		throw "RemoveEdge: Edge doesn't exist";
	}
}
