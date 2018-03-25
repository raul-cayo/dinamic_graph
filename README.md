# dinamic_graph
Dynamic graph C++ library

The only header needed is "graph.h"

Brief description of the class Graph provided by this library:
  - Works with string vertices and int weights.
  - Has a set of functions that allow the Graph to be any combination of directed/undirected and 
    weighted/unweighted type. When the changes are made the Graph has to be adjusted and therefore
    some weights could be overwritten.
  - Has a method toString() that returns the Graph in a single string to be printed out as an Adjacency List
  - Has a couple of methods (saveToFile() / getFromFile()) that allows the Graph to be kept on a single file.
  - Breadth-first and depth-first traversing available
  - Breadth-first and depth-first routes available
  - Prim's algorithm returning the minimum spanning tree as a Graph
