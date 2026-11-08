// Purpose: Facility records, validation, file-order display and hash-based ID search.
// Module owner: Favour (review and commit your own work).
#pragma once
#include "Utils.h"
#include "HashTable.h"
#include <string>
#include <vector>
class CampusGraph;

struct Facility {
    std::string id;
    std::string name;
    std::string building;
    std::string category;
};

class FacilityDatabase {
public:
    // Start with 17 dynamically growing chains; no hard limit on records.
    FacilityDatabase() = default;
    // Load valid facilities and use building spelling from accepted graph vertices.
    LoadCounts load(const std::string& path, const CampusGraph& graph);
    // Display all accepted facility records in source order.
    void display() const;
    // Report format, not-found and found as separate outcomes.
    void searchID(const std::string& userInput) const;
    // Print capacity, accepted keys and collision count.
    void printHashDiagnostics() const;
private:
    std::vector<Facility> facilities_;
    HashTable ids_;
    // Facility IDs must be exactly F followed by three digits.
    static bool validID(const std::string& id);
};
