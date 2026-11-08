// Purpose: Custom facility-ID hash table with separate chaining and diagnostics.
// Module owner: Favour (review and commit your own work).
#pragma once
#include <cstddef>
#include <string>
#include <vector>

class HashTable {
public:
    // Each bucket is a vector of ID/index pairs; collisions never impose a hard limit.
    static constexpr std::size_t DEFAULT_CAPACITY = 17;
    explicit HashTable(std::size_t capacity = DEFAULT_CAPACITY);
    // Insert a unique uppercase key pointing to its index in the facility vector.
    bool insert(const std::string& key, std::size_t facilityIndex);
    // Return a pointer to a stored vector index, or nullptr when not present.
    const std::size_t* find(const std::string& key) const;
    // Report chosen fixed bucket count, accepted entries, and accepted collisions.
    std::size_t capacity() const { return buckets_.size(); }
    std::size_t entries() const { return entries_; }
    std::size_t collisions() const { return collisions_; }
private:
    struct Entry { std::string key; std::size_t index; };
    std::vector<std::vector<Entry>> buckets_;
    std::size_t entries_ = 0;
    std::size_t collisions_ = 0;
    // Polynomial rolling hash: h = (h*31 + byte) modulo bucket capacity.
    std::size_t bucketFor(const std::string& normalizedKey) const;
};
