/*
 * Created by flashbeard.
 * number-theory
 */

#include <iostream>
#include <cassert>
#include <chrono>
#include <iomanip>

#include "Everything.h"

int main() {
    Graph<int> g;
    g.addEdge(1, 4);
    g.addEdge(2, 4);
    g.addEdge(4, 5);
    g.addEdge(4, 6);
    g.addEdge(4, 7);
    g.addEdge(7, 1);
    std::cout << g << std::endl;
    std::cout << g(1, 4) << std::endl;

    std::cout << std::boolalpha << g.hasVertex(10) << std::endl;

    for (int e: g.getVertexesSet()) {
        std::cout << e << ' ';
    }
    std::cout  << std::endl;

    for (int e: g.getNeighborsSet(4)) {
        std::cout << e << ' ';
    }
    std::cout  << std::endl;

    DirectedGraph<int> gg;
    gg.addEdge(1, 2);
    gg.addEdge(1, 2);
    std::cout << gg << std::endl;
}