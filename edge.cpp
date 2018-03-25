#include "edge.h"

Edge::Edge() : nextEdge(nullptr), destVertex(nullptr) {}

Edge::Edge(const int& w) : Edge() {
    weight = w;
}

int Edge::getWeight() {
    return weight;
}

Edge* Edge::getNextEdge() {
    return nextEdge;
}

Vertex* Edge::getDestVertex() {
    return destVertex;
}

void Edge::setWeight(const int& w) {
    weight = w;
}

void Edge::setNextEdge(Edge* e) {
    nextEdge = e;
}

void Edge::setDestVertex(Vertex* v) {
    destVertex = v;
}

