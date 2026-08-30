# Campus Navigation System

A C++ application that finds the shortest walking route between
two people on a university campus and suggests a meeting point
between them.

## What it does

Given two building names or abbreviations, the program finds the
building closest to their geographic midpoint, then computes the
shortest footpath route from each person to that destination.

## How it works

**Graph** — A templated directed graph class (`graph.h`) backed by
an adjacency list. Supports arbitrary vertex and weight types, with
O(1) vertex insertion, edge insertion, and weight lookup.

**Map data** — Campus data is parsed from JSON into waypoints,
buildings, and footways. Footway segments become weighted edges
using real geographic distance. Buildings are then connected to any
non-building node within a set distance threshold, so a route can
enter and leave a building.

**Routing** — Dijkstra's algorithm with a min-heap priority queue
using a custom comparator, tracking predecessors so the full path
can be reconstructed. Building nodes other than the start and target
are excluded from routing, so paths don't cut through buildings.

## Built with

C++ · STL (`map`, `set`, `unordered_map`, `priority_queue`) ·
templates · nlohmann/json

## Running it

​```bash
make
./app
​```

Enter a building name or abbreviation for each person, or `#` to quit.
