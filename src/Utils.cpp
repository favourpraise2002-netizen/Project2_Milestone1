// Purpose: Shared parsing helpers used by the four data loaders.
// Author: Project 2 team starter; confirm individual authors before submission.
#include "Utils.h"
#include <iostream>
#include <cctype>

// Trim only outside whitespace, including Windows carriage return.
std::string trim(const std::string& s) {
    auto first = s.find_first_not_of(" \t\r\n\v\f");
    if (first == std::string::npos) return "";
    auto last = s.find_last_not_of(" \t\r\n\v\f");
    return s.substr(first, last - first + 1);
}
// Normalize ASCII letters, keeping internal whitespace and original data elsewhere.
std::string upper(const std::string& s) {
    std::string result = s;
    for (char& ch : result) {
        if (ch >= 'a' && ch <= 'z') ch = static_cast<char>(ch - 'a' + 'A');
    }
    return result;
}
// Split on every separator; a final separator creates an empty last field.
std::vector<std::string> splitFields(const std::string& s, char separator) {
    std::vector<std::string> fields;
    std::size_t start = 0;
    while (true) {
        auto pos = s.find(separator, start);
        fields.push_back(trim(s.substr(start, pos == std::string::npos ? pos : pos - start)));
        if (pos == std::string::npos) break;
        start = pos + 1;
    }
    return fields;
}
// A # begins a comment only when it is the first non-whitespace character.
bool ignoreLine(const std::string& s) {
    const std::string t = trim(s);
    return t.empty() || t[0] == '#';
}
// Include source filename, line number, and the specific cause of rejection.
void warnRecord(const std::string& path, std::size_t line, const std::string& reason) {
    std::cout << "WARNING " << path << ':' << line << ": " << reason << '\n';
}
// Each input file reports counts independently.
void printSummary(const std::string& filename, LoadCounts count) {
    std::cout << filename << ": " << count.accepted << " accepted, " << count.rejected << " rejected\n";
}
// Support absolute or relative directory arguments, defaulted elsewhere to data/.
std::string dataPath(const std::string& dir, const std::string& name) {
    if (dir.empty()) return name;
    return dir.back() == '/' || dir.back() == '\\' ? dir + name : dir + '/' + name;
}
