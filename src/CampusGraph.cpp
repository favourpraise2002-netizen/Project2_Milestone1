// Purpose: Campus graph validation, undirected adjacency, degrees and BFS reachability.
// Module owner: Ken (starter interface; review, implement, and commit changes).
#include "CampusGraph.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>

// Linear building-name matching is permitted; only facility ID lookup requires hashing.
std::size_t CampusGraph::indexOf(const std::string& name) const {
    const std::string normalized = upper(trim(name));
    for (std::size_t i = 0; i < buildings_.size(); ++i)
        if (upper(buildings_[i].name) == normalized) return i;
    return buildings_.size();
}
// Canonical building names always come from accepted buildings.txt records.
const std::string* CampusGraph::canonicalName(const std::string& name) const {
    auto index = indexOf(name);
    return index == buildings_.size() ? nullptr : &buildings_[index].name;
}
// Each nonempty, noncomment line has one unique building name.
LoadCounts CampusGraph::loadBuildings(const std::string& path) {
    LoadCounts counts;
    std::ifstream in(path);
    if (!in) { std::cout << "ERROR cannot open " << path << '\n'; return counts; }
    std::string line;
    std::size_t lineNum = 0;
    while (std::getline(in, line)) {
        ++lineNum;
        if (ignoreLine(line)) continue;
        auto fields = splitFields(line, '|');
        std::string reason;
        if (fields.size() != 1) reason = "expected one building-name field";
        else if (fields[0].empty()) reason = "empty building name";
        else if (indexOf(fields[0]) != buildings_.size()) reason = "duplicate building name";
        if (!reason.empty()) { warnRecord(path, lineNum, reason); ++counts.rejected; continue; }
        buildings_.push_back({fields[0], {}});
        ++counts.accepted;
    }
    return counts;
}
// Connections are two known, different buildings and each unordered pair appears once.
LoadCounts CampusGraph::loadConnections(const std::string& path) {
    LoadCounts counts;
    std::ifstream in(path);
    if (!in) { std::cout << "ERROR cannot open " << path << '\n'; return counts; }
    std::string line;
    std::size_t lineNum = 0;
    while (std::getline(in, line)) {
        ++lineNum;
        if (ignoreLine(line)) continue;
        auto fields = splitFields(line, '|');
        std::string reason;
        std::size_t a = buildings_.size(), b = buildings_.size();
        if (fields.size() != 2) reason = "expected two building fields";
        else if (fields[0].empty() || fields[1].empty()) reason = "empty building name";
        else {
            a = indexOf(fields[0]); b = indexOf(fields[1]);
            if (a == buildings_.size()) reason = "unknown building: " + fields[0];
            else if (b == buildings_.size()) reason = "unknown building: " + fields[1];
            else if (a == b) reason = "self-connection";
            else if (std::find(buildings_[a].neighbors.begin(), buildings_[a].neighbors.end(), b)
                        != buildings_[a].neighbors.end()) reason = "duplicate or reversed connection";
        }
        if (!reason.empty()) { warnRecord(path, lineNum, reason); ++counts.rejected; continue; }
        buildings_[a].neighbors.push_back(b);
        buildings_[b].neighbors.push_back(a);
        ++edgeCount_;
        ++counts.accepted;
    }
    return counts;
}
// Show empty vertices explicitly instead of silently omitting them.
void CampusGraph::display() const {
    if (buildings_.empty()) { std::cout << "No buildings available.\n"; return; }
    for (const auto& b : buildings_) {
        std::cout << b.name << ": ";
        if (b.neighbors.empty()) std::cout << "(no connections)";
        else {
            for (std::size_t i = 0; i < b.neighbors.size(); ++i) {
                if (i) std::cout << ", ";
                std::cout << buildings_[b.neighbors[i]].name;
            }
        }
        std::cout << '\n';
    }
}
// Every undirected connection was counted once at insertion.
void CampusGraph::statistics() const {
    std::cout << "Total buildings: " << buildings_.size() << "\nTotal connections: " << edgeCount_ << '\n';
    if (buildings_.empty()) std::cout << "No buildings available.\n";
    for (const auto& b : buildings_) std::cout << b.name << " - degree " << b.neighbors.size() << '\n';
}
// FIFO queue ensures shortest distances; visited is set at enqueue, not dequeue.
void CampusGraph::bfs(const std::string& startName) const {
    auto start = indexOf(startName);
    if (start == buildings_.size()) { std::cout << "Error: unknown building: " << trim(startName) << '\n'; return; }
    std::vector<bool> visited(buildings_.size(), false);
    std::vector<int> distances(buildings_.size(), -1);
    std::queue<std::size_t> queue;
    visited[start] = true;
    distances[start] = 0;
    queue.push(start);
    std::size_t reached = 0;
    while (!queue.empty()) {
        auto at = queue.front(); queue.pop();
        ++reached;
        std::cout << buildings_[at].name << " (distance " << distances[at] << ")\n";
        for (auto next : buildings_[at].neighbors) {
            if (!visited[next]) {
                visited[next] = true;
                distances[next] = distances[at] + 1;
                queue.push(next);
            }
        }
    }
    std::cout << "Reachable: " << reached << " of " << buildings_.size() << " buildings\n";
}
