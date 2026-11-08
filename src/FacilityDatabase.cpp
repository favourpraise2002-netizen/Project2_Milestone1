// Purpose: Validate input facilities and search IDs using only the custom hash table.
// Module owner: Favour (review and commit your own work).
#include "FacilityDatabase.h"
#include "CampusGraph.h"
#include <fstream>
#include <iostream>

// Format is intentionally strict after trimming and uppercasing.
bool FacilityDatabase::validID(const std::string& id) {
    return id.size() == 4 && id[0] == 'F'
        && id[1] >= '0' && id[1] <= '9'
        && id[2] >= '0' && id[2] <= '9'
        && id[3] >= '0' && id[3] <= '9';
}
// Skip bad records, warn with line and reason, and keep the first duplicate ID.
LoadCounts FacilityDatabase::load(const std::string& path, const CampusGraph& graph) {
    LoadCounts counts;
    std::ifstream in(path);
    if (!in) { std::cout << "ERROR cannot open " << path << '\n'; return counts; }
    std::string line;
    std::size_t lineNum = 0;
    while (std::getline(in, line)) {
        ++lineNum;
        if (ignoreLine(line)) continue;
        auto f = splitFields(line, '|');
        std::string reason;
        const std::string* building = nullptr;
        std::string id;
        if (f.size() != 4) reason = "expected 4 fields: ID|name|building|category";
        else if (f[0].empty() || f[1].empty() || f[2].empty() || f[3].empty()) reason = "empty required field";
        else {
            id = upper(f[0]);
            if (!validID(id)) reason = "invalid facility ID (F plus exactly three digits)";
            else if (!(building = graph.canonicalName(f[2]))) reason = "unknown building: " + f[2];
            else if (ids_.find(id) != nullptr) reason = "duplicate facility ID: " + id;
        }
        if (!reason.empty()) { warnRecord(path, lineNum, reason); ++counts.rejected; continue; }
        facilities_.push_back({id, f[1], *building, f[3]});
        ids_.insert(id, facilities_.size() - 1);
        ++counts.accepted;
    }
    return counts;
}
// The vector preserves file order, even if distinct facilities have the same name.
void FacilityDatabase::display() const {
    if (facilities_.empty()) std::cout << "No facilities available.\n";
    for (const auto& f : facilities_)
        std::cout << f.id << " | " << f.name << " | " << f.building << " | " << f.category << '\n';
    std::cout << "Total facilities: " << facilities_.size() << '\n';
}
// ID search never walks the complete facility vector, only the selected hash bucket.
void FacilityDatabase::searchID(const std::string& userInput) const {
    const auto id = upper(trim(userInput));
    if (!validID(id)) {
        std::cout << "Invalid ID format: enter F followed by exactly three digits.\n";
        return;
    }
    const std::size_t* position = ids_.find(id);
    if (!position) { std::cout << "Not found: no facility with ID " << id << '\n'; return; }
    const Facility& f = facilities_[*position];
    std::cout << "Found: " << f.id << " | " << f.name << " | " << f.building << " | " << f.category << '\n';
}
// Collisions are accepted insertions into previously nonempty chains.
void FacilityDatabase::printHashDiagnostics() const {
    std::cout << "Hash capacity: " << ids_.capacity() << ", entries: " << ids_.entries()
              << ", collisions: " << ids_.collisions() << '\n';
}
