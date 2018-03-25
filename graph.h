#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <string>
#include <stack>
#include <utility>
#include "vertex.h"
#include "edge.h"

/// -------- EXCEPTIONS --------
class GraphException : public std::exception {
  private:
    std::string msg;

  public:
    explicit GraphException(const char* message) : msg(message) {}
    explicit GraphException(const std::string& message): msg(message) {}
    virtual ~GraphException() throw() {}
    virtual const char* what() const throw() {
        return msg.c_str();
    }
};

/// -------- GRAPH PROTOTYPE --------
class Graph{
private:
    Vertex* anchor;

    bool isDirected;
    bool isWeighted;

    std::string getRoute(std::stack <std::pair <std::string, std::string>>&, const std::string&);
    int getNumVertices();
    int getNumEdges(Vertex*);
    void copyGraph(const Graph&);

public:
    Graph();
    Graph(const Graph&);
    ~Graph();

    Graph& operator = (const Graph&);

    bool isEmpty();

    /* Set of functions that allow the Graph to be any combination
    of directed/undirected and weighted/unweighted type. When the
    changes are made the Graph has to be adjusted and therefore
    some weights could be overwritten. */
    bool getIsDirected();
    bool getIsWeighted();
    void setIsDirected(const bool&);
    void setIsWeighted(const bool&);
    std::string getGraphType();

    void insertVertex(const std::string&);
    void deleteVertex(const std::string&);
    void editVertex(const std::string&, const std::string&);
    Vertex* findVertex(const std::string&);

    /* When working with an unweighted graph, the weight is automatically
    set to 1 and ignored*/
    void insertEdge(const std::string&, const std::string&, const int&);
    void deleteEdge(const std::string&, const std::string&);
    void editEdge(const std::string&, const std::string&, const int&);
    bool existsEdge(const std::string&, const std::string&);

    /* Return the Graph in a single string to be printed out. */
    std::string toString();

    /* Pair of functions that allows the Graph to be kept on a
    single file. The name of the file is the only parameter. */
    void saveToFile(const std::string&);
    bool getFromFile(const std::string&);

    /* Set of methods returning a string of traverses or routes. */
    std::string breadthFirstTraverse(const std::string&);
    std::string depthFirstTraverse(const std::string&);
    std::string breadthFirstRoute(const std::string&, const std::string&);
    std::string depthFirstRoute(const std::string&, const std::string&);

    /* This method generates a minimum spanning tree using Prim's Algorithm */
    Graph primSpanningTree(const std::string&);

    void deleteAll();
};

#endif // GRAPH_H_INCLUDED
