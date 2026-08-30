#pragma once

#include <istream>
#include <set>
#include <string>
#include <vector>

#include "dist.h"
#include "graph.h"

/// A named campus building, tied to a vertex in the graph.
struct BuildingInfo {
  long long id;
  Coordinates location;
  std::string name;
  std::string abbr;

  BuildingInfo() : id(-1) {}
  BuildingInfo(long long id, Coordinates location, std::string name,
               std::string abbr)
      : id(id), location(location), name(std::move(name)),
        abbr(std::move(abbr)) {}
};

/// Reads campus map JSON and populates the graph and building list.
void buildGraph(std::istream& input, graph<long long, double>& g,
                std::vector<BuildingInfo>& buildings);

/// Looks up a building by exact abbreviation or partial name.
/// Returns a BuildingInfo with id == -1 if nothing matches.
BuildingInfo getBuildingInfo(const std::vector<BuildingInfo>& buildings,
                             const std::string& query);

/// Returns the building nearest to the given coordinates.
BuildingInfo getClosestBuilding(const std::vector<BuildingInfo>& buildings,
                                Coordinates c);

/// Shortest path from start to target, skipping any vertex in ignoreNodes
/// unless it is the start or target itself. Empty if unreachable.
std::vector<long long> dijkstra(const graph<long long, double>& G,
                                long long start, long long target,
                                const std::set<long long>& ignoreNodes);

/// Total weight of a path. Returns -1 if any edge is missing.
double pathLength(const graph<long long, double>& G,
                  const std::vector<long long>& path);

/// Prints a path as id->id->id.
void outputPath(const std::vector<long long>& path);

/// Interactive loop: prompts for two buildings and routes both to a
/// meeting point.
void application(const std::vector<BuildingInfo>& buildings,
                 const graph<long long, double>& G);
