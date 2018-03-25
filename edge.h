#ifndef EDGE_H_INCLUDED
#define EDGE_H_INCLUDED

#include "vertex.h"

class Vertex;
class Edge{
private:
    int weight;
    Edge* nextEdge;
    Vertex* destVertex;

public:
    Edge();
    Edge(const int&);

    int getWeight();
    Edge* getNextEdge();
    Vertex* getDestVertex();

    void setWeight(const int&);
    void setNextEdge(Edge*);
    void setDestVertex(Vertex*);
};

#endif // EDGE_H_INCLUDED
