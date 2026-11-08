// Purpose: Implement custom string hashing, lookup and separate-chain insertion.
// Module owner: Favour (review and commit your own work).
#include "HashTable.h"
#include "Utils.h"

// A minimum one bucket prevents modulus by zero for a bad constructor input.
HashTable::HashTable(std::size_t capacity) : buckets_(capacity ? capacity : 1) {}
// Compute hash only from normalized string bytes using no library hashing.
std::size_t HashTable::bucketFor(const std::string& normalizedKey) const {
    std::size_t h = 0;
    const auto mod = buckets_.size();
    for (unsigned char ch : normalizedKey) h = (h * 31 + ch) % mod;
    return h;
}
// Find via the computed bucket only; never scan all facilities.
const std::size_t* HashTable::find(const std::string& key) const {
    const std::string normalized = upper(trim(key));
    const auto& chain = buckets_[bucketFor(normalized)];
    for (const auto& entry : chain) {
        if (entry.key == normalized) return &entry.index;
    }
    return nullptr;
}
// Duplicates do not change any diagnostics; a nonempty destination bucket counts once.
bool HashTable::insert(const std::string& key, std::size_t facilityIndex) {
    const std::string normalized = upper(trim(key));
    if (find(normalized) != nullptr) return false;
    auto& chain = buckets_[bucketFor(normalized)];
    if (!chain.empty()) ++collisions_;
    chain.push_back({normalized, facilityIndex});
    ++entries_;
    return true;
}
