//
// Created by Daniel Pustotin on 06.02.2022.
//

#ifndef ADS_CPP_GRAPH_H
#define ADS_CPP_GRAPH_H

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <string>

template<class T, class U = bool>
class Graph {
public:
    Graph() = default;

    virtual void addEdge(const T &from, const T  &to) {
        mData[from][to] = mData[to][from] = U();
    }

    virtual void addEdge(const T &from, const T  &to, const U &w) {
        mData[from][to] = mData[to][from] = w;
    }

    bool hasVertex(const T &v) {
        return mData.count(v) != 0;
    }

    U &operator()(const T &from, const T &to) {
        if (!hasVertex(from) || !hasVertex(to)) {
            std::stringstream ss;
            ss << "Edge {" << from << ", " << to << "} is missing.";
            throw std::runtime_error(ss.str());
        }
        return mData[from][to];
    }

    const U &operator()(const T &from, const T  &to) const {
        return this(from, to);
    }

    std::unordered_set<T> getVertexesSet() {
        std::unordered_set<T> result;

        for (const auto &[from, adjaecencyMap]: mData) {
            for (auto &[to, weight]: adjaecencyMap) {
                result.insert(from);
                result.insert(to);
            }
        }

        return result;
    }

    std::unordered_set<T> getNeighborsSet(const T &from) {
        std::unordered_set<T> result;

        for (auto &[to, weight]: mData[from]) {
            result.insert(to);
        }

        return result;
    }

    friend std::ostream &operator<<(std::ostream &out, const Graph &g) {
        for (const auto &[from, adjaecencyMap]: g.mData) {
            out << from << ": [ ";
            for (auto &[to, weight]: adjaecencyMap) {
                out << "{ " <<  to << " (" << weight << ") } ";
            }
            out << "]" << std::endl;
        }
        return out;
    }

protected:
    std::unordered_map<T, std::unordered_map<T, U>> mData;
};


template<class T, class U = bool>
class DirectedGraph : public Graph<T, U> {
public:
    DirectedGraph() : Graph<T, U>() {}

    void addEdge(const T &from, const T  &to) override {
        this->mData[from][to] = U();
    }

    void addEdge(const T &from, const T  &to, const U &w) override {
        this->mData[from][to] = w;
    }
};

#endif //ADS_CPP_GRAPH_H
