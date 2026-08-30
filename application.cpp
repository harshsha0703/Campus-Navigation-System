#include "application.h"

#include <iostream>
#include <limits>
#include <map>
#include <queue>  // priority_queue
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "dist.h"
#include "graph.h"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

double INF = numeric_limits<double>::max();

void buildGraph(istream& input, graph<long long, double>& g,
                vector<BuildingInfo>& buildings) {
  json graph_data;
  input >> graph_data;
    unordered_map<long long, Coordinates> nodeCoords;
    if (graph_data.contains("waypoints") && graph_data["waypoints"].is_array()) {
      for (const auto& waypoint : graph_data["waypoints"]) {
        long long id;
        double lat, lon;

        if (waypoint.is_array() && waypoint.size() == 3) {
          id = waypoint[0];
          lat = waypoint[1];
          lon = waypoint[2];
        } else if (waypoint.is_object()) {
          id = waypoint["id"];
          lat = waypoint["lat"];
          lon = waypoint["lon"];
        } else {
          continue;  
        }

        Coordinates coord(lat, lon);
        nodeCoords[id] = coord;
        g.addVertex(id);
      }
    }
  if (graph_data.contains("buildings") && graph_data["buildings"].is_array()) {
      for (const auto& building : graph_data["buildings"]) {
          long long id;
          string fullName, abbrev;
          double lat, lon;

          if (building.is_object()) {
              if (!building.contains("id") || !building.contains("name") ||
                  !building.contains("abbr") || !building.contains("lat") ||
                  !building.contains("lon")) {
                  continue;
              }
              id = building["id"];
              fullName = building["name"];
              abbrev = building["abbr"];
              lat = building["lat"];
              lon = building["lon"];
          } else {
              continue;
          }
          Coordinates coord(lat, lon);
          BuildingInfo bInfo(id, coord, fullName, abbrev);
          buildings.push_back(bInfo);
          nodeCoords[id] = coord;
          g.addVertex(id);
        }
      }
      if (graph_data.contains("footways") && graph_data["footways"].is_array()) {
          for (const auto& footway : graph_data["footways"]) {
              vector<long long> nodes;

              if (footway.is_array()) {
                  for (const auto& node_id : footway) {
                      nodes.push_back(node_id);
                  }
              } else if (footway.is_object() && footway.contains("nodes")) {
                  for (const auto& node_id : footway["nodes"]) {
                      nodes.push_back(node_id);
                  }
              } else {
                  continue;
              }
              for (size_t i = 0; i + 1 < nodes.size(); ++i) {
                  long long from = nodes[i];
                  long long to = nodes[i + 1];
                  if (nodeCoords.find(from) == nodeCoords.end() || nodeCoords.find(to) == nodeCoords.end()) {
                      continue; 
                  }
                  const Coordinates& coordFrom = nodeCoords[from];
                  const Coordinates& coordTo = nodeCoords[to];
                  double distance = distBetween2Points(coordFrom, coordTo);
                  g.addEdge(from, to, distance);
                  g.addEdge(to, from, distance);
              }
          }
      }
      const double MAX_DISTANCE = 0.036;  
      set<long long> nonBuildingNodes;
      for (const auto& [id, _] : nodeCoords) {
          nonBuildingNodes.insert(id);
      }
      for (const auto& b : buildings) {
          nonBuildingNodes.erase(b.id);
      }
      for (const auto& building : buildings) {
          const Coordinates& bCoord = building.location;

          for (const auto& nodeId : nonBuildingNodes) {
              const Coordinates& nCoord = nodeCoords[nodeId];
              double distance = distBetween2Points(bCoord, nCoord);

              if (distance <= MAX_DISTANCE) {
                  g.addEdge(building.id, nodeId, distance);
                  g.addEdge(nodeId, building.id, distance);
              }
          }
    }
  }

BuildingInfo getBuildingInfo(const vector<BuildingInfo>& buildings,
                             const string& query) {
  for (const BuildingInfo& building : buildings) {
    if (building.abbr == query) {
      return building;
    } else if (building.name.find(query) != string::npos) {
      return building;
    }
  }
  BuildingInfo failed;
  failed.id = -1;
  return failed;
}

BuildingInfo getClosestBuilding(const vector<BuildingInfo>& buildings,
                                Coordinates c) {
  double min_dist = INF;
  BuildingInfo ret = buildings.at(0);
  for (const BuildingInfo& building : buildings) {
    double dist = distBetween2Points(building.location, c);
    if (dist < min_dist) {
      min_dist = dist;
      ret = building;
    }
  }
  return ret;
}

vector<long long> dijkstra(const graph<long long, double>& G, long long start,
                           long long target,
                           const set<long long>& ignoreNodes) {
  if (start == target) {
    return {start};
  }
  class prioritize {
   public:
    bool operator()(const pair<long long, double>& p1,
                    const pair<long long, double>& p2) const {
      return p1.second > p2.second;
    }
  };
  priority_queue<pair<long long, double>,
                 vector<pair<long long, double>>,
                 prioritize>
      worklist;
  unordered_map<long long, double> distances;
  unordered_map<long long, long long> predecessors;
  unordered_set<long long> visited;

  distances[start] = 0.0;
  worklist.push({start, 0.0});
  while (!worklist.empty()) {
    auto current = worklist.top();
    worklist.pop();
    long long u = current.first;


    if (visited.count(u)) {
      continue;
    }
    visited.insert(u);
    if (ignoreNodes.count(u) && u != start && u != target) {
      continue;
    }
    if (u == target) {
      vector<long long> path;
      long long v = target;
      while (v != start) {
        path.push_back(v);
        v = predecessors[v];
      }
      path.push_back(start);
      reverse(path.begin(), path.end());
      return path;
    }
    set<long long> neighbors = G.neighbors(u);
    for (const auto& v : neighbors) {
      if (ignoreNodes.count(v) && v != start && v != target) {
        continue;
      }
      double edgeWeight;
      if (!G.getWeight(u, v, edgeWeight)) {
        continue;
      }

      double altDistance = distances[u] + edgeWeight;
      if (!distances.count(v) || altDistance < distances[v]) {
        distances[v] = altDistance;
        predecessors[v] = u;
        worklist.push({v, altDistance});
      }
    }
  }
  return {};
}

double pathLength(const graph<long long, double>& G,
                  const vector<long long>& path) {
  double length = 0.0;
  double weight;
  for (size_t i = 0; i + 1 < path.size(); i++) {
    bool res = G.getWeight(path.at(i), path.at(i + 1), weight);
    if (!res) {
      return -1;
    }
    length += weight;
  }
  return length;
}

void outputPath(const vector<long long>& path) {
  for (size_t i = 0; i < path.size(); i++) {
    cout << path.at(i);
    if (i != path.size() - 1) {
      cout << "->";
    }
  }
  cout << endl;
}

void application(const vector<BuildingInfo>& buildings,
                 const graph<long long, double>& G) {
  string person1Building, person2Building;

  set<long long> buildingNodes;
  for (const auto& building : buildings) {
    buildingNodes.insert(building.id);
  }

  cout << endl;
  cout << "Enter person 1's building (partial name or abbreviation), or #> ";
  getline(cin, person1Building);

  while (person1Building != "#") {
    cout << "Enter person 2's building (partial name or abbreviation)> ";
    getline(cin, person2Building);

    
    BuildingInfo p1 = getBuildingInfo(buildings, person1Building);
    BuildingInfo p2 = getBuildingInfo(buildings, person2Building);
    Coordinates P1Coords, P2Coords;
    string P1Name, P2Name;

    if (p1.id == -1) {
      cout << "Person 1's building not found" << endl;
    } else if (p2.id == -1) {
      cout << "Person 2's building not found" << endl;
    } else {
      cout << endl;
      cout << "Person 1's point:" << endl;
      cout << " " << p1.name << endl;
      cout << " " << p1.id << endl;
      cout << " (" << p1.location.lat << ", " << p1.location.lon << ")" << endl;
      cout << "Person 2's point:" << endl;
      cout << " " << p2.name << endl;
      cout << " " << p2.id << endl;
      cout << " (" << p2.location.lat << ", " << p2.location.lon << ")" << endl;

      Coordinates centerCoords = centerBetween2Points(p1.location, p2.location);
      BuildingInfo dest = getClosestBuilding(buildings, centerCoords);

      cout << "Destination Building:" << endl;
      cout << " " << dest.name << endl;
      cout << " " << dest.id << endl;
      cout << " (" << dest.location.lat << ", " << dest.location.lon << ")"
           << endl;

      vector<long long> P1Path = dijkstra(G, p1.id, dest.id, buildingNodes);
      vector<long long> P2Path = dijkstra(G, p2.id, dest.id, buildingNodes);

      // This should NEVER happen with how the graph is built
      if (P1Path.empty() || P2Path.empty()) {
        cout << endl;
        cout << "At least one person was unable to reach the destination "
                "building. Is an edge missing?"
             << endl;
        cout << endl;
      } else {
        cout << endl;
        cout << "Person 1's distance to dest: " << pathLength(G, P1Path);
        cout << " miles" << endl;
        cout << "Path: ";
        outputPath(P1Path);
        cout << endl;
        cout << "Person 2's distance to dest: " << pathLength(G, P2Path);
        cout << " miles" << endl;
        cout << "Path: ";
        outputPath(P2Path);
      }
    }

    //
    // another navigation?
    //
    cout << endl;
    cout << "Enter person 1's building (partial name or abbreviation), or #> ";
    getline(cin, person1Building);
  }
}
