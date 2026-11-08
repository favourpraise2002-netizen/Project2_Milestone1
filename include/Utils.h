// Purpose: Shared text-file parsing and validation helpers.
// Author: Project 2 team starter; confirm individual authors before submission.
#pragma once
#include <cstddef>
#include <string>
#include <vector>

struct LoadCounts { std::size_t accepted = 0; std::size_t rejected = 0; };

// Remove outer spaces, tabs, and carriage returns without touching internal spaces.
std::string trim(const std::string& s);
// Uppercase ASCII characters for case-insensitive matching.
std::string upper(const std::string& s);
// Split a line into fields while preserving empty fields.
std::vector<std::string> splitFields(const std::string& s, char separator);
// Skip empty and full-line comment records.
bool ignoreLine(const std::string& s);
// Print a location and explanation for a rejected record.
void warnRecord(const std::string& path, std::size_t line, const std::string& reason);
// Print a file's accepted and rejected record counts.
void printSummary(const std::string& filename, LoadCounts count);
// Add slash between data directory and a filename.
std::string dataPath(const std::string& dir, const std::string& name);
