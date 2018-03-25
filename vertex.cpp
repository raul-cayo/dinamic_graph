#include "vertex.h"

Vertex::Vertex() : nextVertex(nullptr), firstEdge(nullptr) {}

Vertex::Vertex(const std::string& l) : Vertex() {
    label = l;
}

std::string Vertex::getLabel() {
    return label;
}

Vertex* Vertex::getNextVertex() {
    return nextVertex;
}

Edge* Vertex::getFirstEdge() {
    return firstEdge;
}

void Vertex::setLabel(const std::string& l) {
    label = l;
}

void Vertex::setNextVertex(Vertex* v) {
    nextVertex = v;
}

void Vertex::setFirstEdge(Edge* e) {
    firstEdge = e;
}
