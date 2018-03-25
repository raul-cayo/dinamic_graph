#include "graph.h"
#include <fstream>
#include <set>
#include <stack>
#include <queue>
#include <utility>
#include <tuple>

using namespace std;

/// --- PRIVATE METHODS ---
string Graph::getRoute(stack <pair <string, string>>& pairs, const string& dest) {
    string currentDest(dest), route;

    while(!pairs.empty()){
        route += currentDest + " <- ";
        while(!pairs.empty() && pairs.top().second != currentDest){
            pairs.pop();
        }
        if(!pairs.empty()){
            currentDest = pairs.top().first;
        }
    }

    return route.substr(0, route.length() - 3);
}

int Graph::getNumVertices(){
    int num(0);
    Vertex* aux(anchor);
    while(aux != nullptr){
        num++;
        aux = aux->getNextVertex();
    }

    return num;
}

int Graph::getNumEdges(Vertex* v){
    if(findVertex(v->getLabel()) == nullptr){
        throw GraphException("getNumEdges: there is not such vertex.");
    }

    int num(0);
    Edge* aux(v->getFirstEdge());
    while(aux != nullptr){
        num++;
        aux = aux->getNextEdge();
    }

    return num;
}

void Graph::copyGraph(const Graph& g){
    deleteAll();

    isDirected = g.isDirected;
    isWeighted = g.isWeighted;

    Vertex* auxVer(g.anchor);
    while(auxVer != nullptr){
        insertVertex(auxVer->getLabel());
        auxVer = auxVer->getNextVertex();
    }

    auxVer = g.anchor;
    while(auxVer != nullptr){
        Edge* auxAri(auxVer->getFirstEdge());
        while(auxAri != nullptr){
            insertEdge(auxVer->getLabel(), auxAri->getDestVertex()->getLabel(), auxAri->getWeight());
            auxAri = auxAri->getNextEdge();
        }

        auxVer = auxVer->getNextVertex();
    }
}

/// --- PUBLIC METHODS ---
Graph::Graph() : anchor(nullptr), isDirected(true), isWeighted(true) {}

Graph::Graph(const Graph& g) : Graph() {
    copyGraph(g);
}

Graph::~Graph() {
    deleteAll();
}

Graph& Graph::operator = (const Graph& g){
    deleteAll();
    copyGraph(g);
    return *this;
}

bool Graph::isEmpty() {
    return anchor == nullptr;
}

bool Graph::getIsDirected() {
    return isDirected;
}

bool Graph::getIsWeighted() {
    return isWeighted;
}

void Graph::setIsDirected(const bool& b) {
    if(isDirected && !b){
        Vertex* verAux(anchor);
        while(verAux != nullptr){

            Edge* edgeAux(verAux->getFirstEdge());
            while(edgeAux != nullptr){
                if(existsEdge(edgeAux->getDestVertex()->getLabel(), verAux->getLabel())){
                    editEdge(edgeAux->getDestVertex()->getLabel(), verAux->getLabel(), edgeAux->getWeight());
                }
                else{
                    insertEdge(edgeAux->getDestVertex()->getLabel(), verAux->getLabel(), edgeAux->getWeight());
                }

                edgeAux = edgeAux->getNextEdge();
            }

            verAux = verAux->getNextVertex();
        }
    }

    isDirected = b;
}

void Graph::setIsWeighted(const bool& b) {
    isWeighted = b;
}

string Graph::getGraphType() {
    string result;

    if(isDirected){
        result += "Directed";
    }
    else{
        result += "Undirected";
    }

    if(isWeighted){
        result += " Weighted";
    }
    else{
        result += " Unweighted";
    }

    return result;
}

void Graph::insertVertex(const string& label) {
    Vertex* aux = new Vertex(label);
    if(aux == nullptr || findVertex(label) != nullptr){
        throw GraphException("insertVertex: not memory available or that vertex already exists.");
    }

    if(isEmpty()){
        anchor = aux;
    }
    else{
        Vertex* last(anchor);
        while(last->getNextVertex() != nullptr){
            last = last->getNextVertex();
        }
        last->setNextVertex(aux);
    }
}

void Graph::deleteVertex(const string& label) {
    if(findVertex(label) == nullptr){
        throw GraphException("deleteVertex: there is not such vertex.");
    }

    Vertex* vertexToBeDeleted(anchor);
    while(vertexToBeDeleted->getLabel() != label){
        vertexToBeDeleted = vertexToBeDeleted->getNextVertex();
    }

    Vertex* verAux(anchor);
    while(verAux != nullptr){

        Edge* edgeAux(verAux->getFirstEdge());
        while(edgeAux != nullptr && edgeAux->getDestVertex() != vertexToBeDeleted){
            edgeAux = edgeAux->getNextEdge();
        }
        if(edgeAux != nullptr){
            deleteEdge(verAux->getLabel(), vertexToBeDeleted->getLabel());
        }

        verAux = verAux->getNextVertex();
    }

    if(vertexToBeDeleted == anchor){
        anchor = vertexToBeDeleted->getNextVertex();
    }
    else{
        verAux = anchor;
        while(verAux->getNextVertex() != vertexToBeDeleted){
            verAux = verAux->getNextVertex();
        }
        verAux->setNextVertex(vertexToBeDeleted->getNextVertex());
    }

    delete vertexToBeDeleted;
}

void Graph::editVertex(const string& oldLabel, const string& newLabel) {
    Vertex* aux(findVertex(oldLabel));
    if(aux == nullptr || findVertex(newLabel) != nullptr){
        throw GraphException("editVertex: there is not such vertex or new vertex already exists");
    }

    aux->setLabel(newLabel);
}

Vertex* Graph::findVertex(const string& label) {
    Vertex* aux(anchor);

    while(aux != nullptr){
        if(aux->getLabel() == label){
            return aux;
        }
        aux = aux->getNextVertex();
    }

    return aux;
}

void Graph::insertEdge(const string& originLabel, const string& destLabel, const int& weight = 1) {
    Vertex* origin(findVertex(originLabel));
    Vertex* destination(findVertex(destLabel));

    if(origin == nullptr || destination == nullptr){
        throw GraphException("insertEdge: at least one vertex doesn't exists.");
    }
    if(!existsEdge(originLabel, destLabel)){
        Edge* aux = new Edge(weight);
        if(aux == nullptr){
            throw GraphException("insertEdge: memory not available.");
        }

        if(origin->getFirstEdge() == nullptr){
            origin->setFirstEdge(aux);
        }
        else{
            Edge* last(origin->getFirstEdge());
            while(last->getNextEdge() != nullptr){
                last = last->getNextEdge();
            }
            last->setNextEdge(aux);
        }
        aux->setDestVertex(destination);

        if(!isDirected && !existsEdge(destLabel, originLabel)){
            Edge* aux2 = new Edge(weight);
            if(aux2 == nullptr){
                throw GraphException("insertEdge: memory not available.");
            }

            if(destination->getFirstEdge() == nullptr){
                destination->setFirstEdge(aux2);
            }
            else{
                Edge* last2(destination->getFirstEdge());
                while(last2->getNextEdge() != nullptr){
                    last2 = last2->getNextEdge();
                }
                last2->setNextEdge(aux2);
            }
            aux2->setDestVertex(origin);
        }
    }
}

void Graph::deleteEdge(const string& originLabel, const string& destLabel) {
    if(!existsEdge(originLabel, destLabel)){
        throw GraphException("deleteEdge: there is not such edge.");
    }

    Vertex* origin(findVertex(originLabel));
    Vertex* destination(findVertex(destLabel));

    Edge* aux(origin->getFirstEdge());
    while(aux->getDestVertex() != destination){
        aux = aux->getNextEdge();
    }

    if(origin->getFirstEdge() == aux){
        origin->setFirstEdge(aux->getNextEdge());
    }
    else{
        Edge* prev(origin->getFirstEdge());
        while(prev->getNextEdge() != aux){
            prev = prev->getNextEdge();
        }
        prev->setNextEdge(aux->getNextEdge());
    }
    delete aux;

    if(!isDirected){
        Edge* aux2(destination->getFirstEdge());
        while(aux2->getDestVertex() != origin){
            aux2 = aux2->getNextEdge();
        }

        if(destination->getFirstEdge() == aux2){
            destination->setFirstEdge(aux2->getNextEdge());
        }
        else{
            Edge* prev2(destination->getFirstEdge());
            while(prev2->getNextEdge() != aux2){
                prev2 = prev2->getNextEdge();
            }
            prev2->setNextEdge(aux2->getNextEdge());
        }
        delete aux2;
    }
}

void Graph::editEdge(const string& originLabel, const string& destLabel, const int& p) {
    if(!existsEdge(originLabel, destLabel)){
        throw GraphException("deleteEdge: there is not such edge.");
    }

    Vertex* origin(findVertex(originLabel));
    Vertex* destination(findVertex(destLabel));

    Edge* aux(origin->getFirstEdge());
    while(aux->getDestVertex() != destination){
        aux = aux->getNextEdge();
    }
    aux->setWeight(p);

    if(!isDirected){
        Edge* aux2(destination->getFirstEdge());
        while(aux2->getDestVertex() != origin){
            aux2 = aux2->getNextEdge();
        }
        aux2->setWeight(p);
    }
}

bool Graph::existsEdge(const string& originLabel, const string& destLabel) {
    Vertex* origin(findVertex(originLabel));
    Vertex* destination(findVertex(destLabel));

    if(origin == nullptr || destination == nullptr){
        return false;
    }

    Edge* aux(origin->getFirstEdge());
    while(aux != nullptr){
        if(aux->getDestVertex() == destination){
            return true;
        }
        aux = aux->getNextEdge();
    }
    return false;
}

string Graph::toString() {
    Vertex* verAux(anchor);
    string result;

    while(verAux != nullptr){
        result += verAux->getLabel();

        Edge* edgeAux(verAux->getFirstEdge());
        while(edgeAux != nullptr){
            result += " -> " + edgeAux->getDestVertex()->getLabel();
            if(isWeighted){
                result += ":" + to_string(edgeAux->getWeight());
            }
            edgeAux = edgeAux->getNextEdge();
        }

        verAux = verAux->getNextVertex();
        result += "\n";
    }

    return result;
}

void Graph::saveToFile(const string& fileName) {
    ofstream myFile(fileName, ios::trunc);

    myFile << isDirected << '|' << isWeighted << "\n";

    myFile << getNumVertices() << "\n";
    Vertex* auxVer(anchor);
    while(auxVer != nullptr){
        myFile << auxVer->getLabel() << "\n";
        auxVer = auxVer->getNextVertex();
    }

    auxVer = anchor;
    while(auxVer != nullptr){
        myFile << getNumEdges(auxVer) << "\n";

        Edge* auxAri(auxVer->getFirstEdge());
        while(auxAri != nullptr){
            myFile << auxVer->getLabel() << '|' << auxAri->getDestVertex()->getLabel() << '|' << auxAri->getWeight() << "\n";
            auxAri = auxAri->getNextEdge();
        }

        auxVer = auxVer->getNextVertex();
    }

    myFile.close();
}

bool Graph::getFromFile(const string& fileName) {
    deleteAll();
    ifstream myFile(fileName);

    if(myFile.good()){
        int numVer, numEdges, weight;
        string origin, destination;

        myFile >> isDirected;
        myFile.ignore(10, '|');
        myFile >> isWeighted;
        myFile.ignore(10, '\n');

        myFile >> numVer;
        myFile.ignore(10, '\n');
        for(int i(0); i < numVer; i++){
            getline(myFile, origin, '\n');
            insertVertex(origin);
        }

        for(int i(0); i < numVer; i++){
            myFile >> numEdges;
            myFile.ignore(10, '\n');

            for(int j(0); j < numEdges; j++){
                getline(myFile, origin, '|');
                getline(myFile, destination, '|');
                myFile >> weight;
                myFile.ignore(10, '\n');

                insertEdge(origin, destination, weight);
            }
        }

        myFile.close();
        return true;
    }
    return false;
}

string Graph::breadthFirstTraverse(const string& origin) {
    if(findVertex(origin) == nullptr){
        return "There is not such origin vertex.";
    }

    set <string> visited;
    queue <string> myQueue;
    string currentVertex;
    string traverse;

    myQueue.push(origin);
    while(!myQueue.empty()){
        currentVertex = myQueue.front();
        myQueue.pop();

        if(!visited.count(currentVertex)){
            traverse += currentVertex + " -> ";

            visited.insert(currentVertex);

            Edge* aux(findVertex(currentVertex)->getFirstEdge());
            while(aux != nullptr){
                if(!visited.count(aux->getDestVertex()->getLabel())){
                    myQueue.push(aux->getDestVertex()->getLabel());
                }
                aux = aux->getNextEdge();
            }
        }
    }

    return traverse.substr(0, traverse.length() - 3);
}

string Graph::depthFirstTraverse(const string& origin) {
    if(findVertex(origin) == nullptr){
        return "There is not such origin vertex.";
    }

    set <string> visited;
    stack <string> myStack;
    string currentVertex;
    string traverse;

    myStack.push(origin);
    while(!myStack.empty()){
        currentVertex = myStack.top();
        myStack.pop();

        if(!visited.count(currentVertex)){
            traverse += currentVertex + " -> ";

            visited.insert(currentVertex);

            Edge* aux(findVertex(currentVertex)->getFirstEdge());
            while(aux != nullptr){
                if(!visited.count(aux->getDestVertex()->getLabel())){
                    myStack.push(aux->getDestVertex()->getLabel());
                }
                aux = aux->getNextEdge();
            }
        }
    }

    return traverse.substr(0, traverse.length() - 3);
}

string Graph::breadthFirstRoute(const string& origin, const string& destination) {
    if(findVertex(origin) == nullptr || findVertex(destination) == nullptr){
        return "At least one of the vertices doesn't exists";
    }

    set <string> visited;
    queue <string> myQueue;
    stack <pair <string, string>> pairs;
    pair <string, string> myPair;
    string currentVertex;

    myQueue.push(origin);
    while(!myQueue.empty()){
        currentVertex = myQueue.front();
        myQueue.pop();

        if(!visited.count(currentVertex)){
            if(currentVertex == destination){
                return getRoute(pairs, destination);
            }

            visited.insert(currentVertex);

            Edge* aux(findVertex(currentVertex)->getFirstEdge());
            while(aux != nullptr){
                if(!visited.count(aux->getDestVertex()->getLabel())){
                    myQueue.push(aux->getDestVertex()->getLabel());

                    myPair.first = currentVertex;
                    myPair.second = aux->getDestVertex()->getLabel();
                    pairs.push(myPair);
                }
                aux = aux->getNextEdge();
            }
        }
    }

    return "\nThere is no conection between those vertices\n";
}

string Graph::depthFirstRoute(const string& origin, const string& destination) {
    if(findVertex(origin) == nullptr || findVertex(destination) == nullptr){
        return "At least one of the vertices doesn't exists";
    }

    set <string> visited;
    stack <string> myStack;
    stack <pair <string, string>> pairs;
    pair <string, string> myPair;
    string currentVertex;

    myStack.push(origin);
    while(!myStack.empty()){
        currentVertex = myStack.top();
        myStack.pop();

        if(!visited.count(currentVertex)){
            if(currentVertex == destination){
                return getRoute(pairs, destination);
            }

            visited.insert(currentVertex);

            Edge* aux(findVertex(currentVertex)->getFirstEdge());
            while(aux != nullptr){
                if(!visited.count(aux->getDestVertex()->getLabel())){
                    myStack.push(aux->getDestVertex()->getLabel());

                    myPair.first = currentVertex;
                    myPair.second  = aux->getDestVertex()->getLabel();
                    pairs.push(myPair);
                }
                aux = aux->getNextEdge();
            }
        }
    }

    return "\nThere is no conection between those vertices\n";
}

Graph Graph::primSpanningTree(const string& origin){
    Vertex* auxVer(findVertex(origin));
    if(auxVer == nullptr){
        throw GraphException("primSpanningTree: there is not such vertex.");
    }

    Graph spanningTree;
    tuple <string, string, int> tempEdge, tempEdge2;
    set <string> visited;

    class myGreater {
    public:
      bool operator () (const tuple <string, string, int> a, const tuple <string, string, int> b) const {
        return get<2>(a) > get<2>(b);
      }
    };

    priority_queue <tuple <string, string, int>, vector <tuple <string, string, int>>, myGreater> priQueue;

    visited.insert(origin);
    Edge* auxEdge(auxVer->getFirstEdge());
    while(auxEdge != nullptr){
        get<0>(tempEdge) = auxVer->getLabel();
        get<1>(tempEdge) = auxEdge->getDestVertex()->getLabel();
        get<2>(tempEdge) = auxEdge->getWeight();
        priQueue.push(tempEdge);

        auxEdge = auxEdge->getNextEdge();
    }

    while(!priQueue.empty()){
        tempEdge = priQueue.top();
        priQueue.pop();

        if(!visited.count(get<1>(tempEdge))){
            visited.insert(get<1>(tempEdge));

            auxEdge = findVertex(get<1>(tempEdge))->getFirstEdge();
            while(auxEdge != nullptr){
                get<0>(tempEdge2) = findVertex(get<1>(tempEdge))->getLabel();
                get<1>(tempEdge2) = auxEdge->getDestVertex()->getLabel();
                get<2>(tempEdge2) = auxEdge->getWeight();
                priQueue.push(tempEdge2);

                auxEdge = auxEdge->getNextEdge();
            }

            if(spanningTree.findVertex(get<0>(tempEdge)) == nullptr){
                spanningTree.insertVertex(get<0>(tempEdge));
            }
            if(spanningTree.findVertex(get<1>(tempEdge)) == nullptr){
                spanningTree.insertVertex(get<1>(tempEdge));
            }
            spanningTree.insertEdge(get<0>(tempEdge), get<1>(tempEdge), get<2>(tempEdge));
        }
    }
    return spanningTree;
}

void Graph::deleteAll() {
    Vertex* verAux(anchor);
    Vertex* vertexToBeDeleted;
    while(verAux != nullptr){

        Edge* edgeAux(verAux->getFirstEdge());
        Edge* edgeToBeDeleted;
        while(edgeAux != nullptr){
            edgeToBeDeleted = edgeAux;
            edgeAux = edgeAux->getNextEdge();
            delete edgeToBeDeleted;
        }

        vertexToBeDeleted = verAux;
        verAux = verAux->getNextVertex();
        delete vertexToBeDeleted;
    }
    anchor = nullptr;
}
