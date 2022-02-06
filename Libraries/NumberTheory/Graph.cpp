//
// Created by Daniel Pustotin on 06.02.2022.
//

#include "Graph.h"

#include <vector>

template<class T>
class Graph {

private:
    size_t nodeCount;
    enum GraphKind {
        adjaecencyList, adjaecencyMatrix, edgeList
    } graphKind;
    union GraphData {
        std::vector<std::vector<T>> adjaecencyList;
        std::vector<std::vector<bool>> adjaecencyMatrix;
        std::vector<std::pair<T, T>> edgeList;
    } graph;

public:
    Graph() {
        nodeCount = 0;
        graphKind = GraphKind::adjaecencyList;
        graph.adjaecencyList = std::vector<std::vector<T>>();
    }

    void constructFromAdjaecencyList (const std::vector<std::vector<T>>& data) {
        nodeCount = data.size();
        graphKind = GraphKind::adjaecencyList;
        graph.adjaecencyList = data;
    }

    void constructFromAdjaecencyMatrix (const std::vector<std::vector<bool>>& data) {
        nodeCount = data.size();
        graphKind = GraphKind::adjaecencyMatrix;
        graph.adjaecencyMatrix = data;
    }

    void constructFromEdgeList (const std::vector<std::pair<T, T>>& data, int size) {
        nodeCount = size;
        graphKind = GraphKind::edgeList;
        graph.edgeList = data;
    }

    void addEdge(T from, T to) {
        switch (graphKind) {
            case GraphKind::adjaecencyList:
                graph.adjaecencyList[from][to]
        }
    }
};