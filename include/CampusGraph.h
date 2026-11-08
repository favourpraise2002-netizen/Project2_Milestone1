// Purpose: Dynamic undirected campus graph, validation, statistics and BFS.
// Module owner: Ken (starter interface; review, implement, and commit changes).
#pragma once
#include "Utils.h"
#include <cstddef>
#include <string>
#include <vector>

class CampusGraph {
public:
    // Read accepted vertices in the order they appear in buildings.txt.
    LoadCounts loadBuildings(const std::string& path);
    // Read undirected edges, preserving neighbor order and rejecting reverse duplicates.
    LoadCounts loadConnections(const std::string& path);
    // Resolve a trimmed, case-insensitive building name to its canonical spelling.
    const std::string* canonicalName(const std::string& name) const;
    // Print vertices and neighbors in insertion order.
    void display() const;
    // Print undirected edge total and degree of each vertex.
    void statistics() const;
    // Queue-based breadth-first search, marking nodes when enqueued.
    void bfs(const std::string& startName) const;
    std::size_t buildingCount() const { return buildings_.size(); }
private:
    struct Building { std::string name; std::vector<std::size_t> neighbors; };
    std::vector<Building> buildings_;
    std::size_t edgeCount_ = 0;
    // Resolve a name to its vertex index, returning buildingCount() when missing.
    std::size_t indexOf(const std::string& name) const;
};
