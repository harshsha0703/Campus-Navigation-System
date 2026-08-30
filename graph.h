#pragma once

#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>
//#include "json.hpp"

using namespace std;

/// @brief Simple directed graph using an adjacency list.
/// @tparam VertexT vertex type
/// @tparam WeightT edge weight type
template <typename VertexT, typename WeightT>
class graph {
 private:
  unordered_map<VertexT, unordered_map<VertexT, WeightT>> adjList;
  size_t edge_count;

 public:
  graph() : edge_count(0) {

  }

  /// @brief Add the vertex `v` to the graph, must typically be O(1).
  /// @param v
  /// @return true if successfully added; false if it existed already
  bool addVertex(VertexT v) {
    auto result = adjList.emplace(v, unordered_map<VertexT, WeightT>{});
    return result.second;
  }

  /// @brief Add or overwrite directed edge in the graph, must typically be
  /// O(1).
  /// @param from starting vertex
  /// @param to ending vertex
  /// @param weight edge weight / label
  /// @return true if successfully added or overwritten;
  ///         false if either vertices isn't in graph
  bool addEdge(VertexT from, VertexT to, WeightT weight) {
    if (adjList.find(from) == adjList.end() || adjList.find(to) == adjList.end()) {
      return false;
    }
    bool edge_exists = adjList[from].find(to) != adjList[from].end();
    adjList[from][to] = weight;
    if (!edge_exists) {
      ++edge_count;
    }
    return true;
  }

  /// @brief Maybe get the weight associated with a given edge, must typically
  /// be O(1).
  /// @param from starting vertex
  /// @param to ending vertex
  /// @param weight output parameter
  /// @return true if the edge exists, and `weight` is set;
  ///         false if the edge does not exist
  bool getWeight(VertexT from, VertexT to, WeightT& weight) const {
    auto from_It = adjList.find(from);
    if (from_It != adjList.end()) {
      auto to_It = from_It->second.find(to);
      if (to_It != from_It->second.end()) {
        weight = to_It->second;
        return true;
      }
    }
    return false;
  }

  /// @brief Get the out-neighbors of `v`. Must run in at most O(|V|).
  /// @param v
  /// @return vertices that v has an edge to
  set<VertexT> neighbors(VertexT v) const {
    set<VertexT> S;
    set<VertexT> neighborSet;
    auto it = adjList.find(v);
    if (it != adjList.end()) {
      for (const auto& pair : it->second) {
        neighborSet.insert(pair.first);
      }
    }
    return neighborSet;
  
  }

  /// @brief Return a vector containing all vertices in the graph
  vector<VertexT> getVertices() const {

    vector<VertexT> vertices;
    vertices.reserve(adjList.size());
    for (const auto& pair : adjList) {
      vertices.push_back(pair.first);
    }
    return vertices;
  }

  /// @brief Get the number of vertices in the graph. Runs in O(1).
  size_t numVertices() const {
    return adjList.size();
  }

  /// @brief Get the number of directed edges in the graph. Runs in at most
  /// O(|V|), but should be O(1).
  size_t numEdges() const {
    return edge_count;
  }
};
