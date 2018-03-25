#ifndef VERTEX_H_INCLUDED
#define VERTEX_H_INCLUDED

#include <string>
#include "edge.h"

class Edge;
class Vertex {
  private:
    std::string label;
    Vertex* nextVertex;
    Edge* firstEdge;

  public:
    Vertex();
    Vertex(const std::string&);

    std::string getLabel();
    Vertex* getNextVertex();
    Edge* getFirstEdge();

    void setLabel(const std::string&);
    void setNextVertex(Vertex*);
    void setFirstEdge(Edge*);
};

#endif // VERTEX_H_INCLUDED
