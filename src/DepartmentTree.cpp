// Purpose: Validated dynamic general tree with arbitrary children and preorder display.
// Module owner: Ademola (starter interface; review, implement, and commit changes).
#include "DepartmentTree.h"
#include <fstream>
#include <iostream>

// Recursion checks all descendants in insertion order.
const DepartmentTree::Node* DepartmentTree::find(const Node* node, const std::string& normalized) {
    if (!node) return nullptr;
    if (upper(node->name) == normalized) return node;
    for (const auto& child : node->children)
        if (auto found = find(child.get(), normalized)) return found;
    return nullptr;
}
// Mutable search delegates to const search, preserving a single traversal definition.
DepartmentTree::Node* DepartmentTree::find(Node* node, const std::string& normalized) {
    return const_cast<Node*>(find(static_cast<const Node*>(node), normalized));
}
// The first valid root uses no parent; later additions require an existing parent.
bool DepartmentTree::add(const std::string& name, const std::string& parent, std::string& reason) {
    const auto cleanName = trim(name);
    const auto cleanParent = trim(parent);
    if (cleanName.empty()) { reason = "empty department name"; return false; }
    if (find(root_.get(), upper(cleanName))) { reason = "duplicate department name: " + cleanName; return false; }
    if (!root_) {
        if (upper(cleanParent) != "NONE" && !cleanParent.empty()) {
            reason = "first department must have NONE as parent"; return false;
        }
        root_ = std::make_unique<Node>(Node{cleanName, nullptr, {}});
        return true;
    }
    if (cleanParent.empty() || upper(cleanParent) == "NONE") {
        reason = "parent required for non-root department"; return false;
    }
    Node* owner = find(root_.get(), upper(cleanParent));
    if (!owner) { reason = "unknown parent department: " + cleanParent; return false; }
    owner->children.push_back(std::make_unique<Node>(Node{cleanName, owner, {}}));
    return true;
}
// Validate two fields and parent references before accepting a record.
LoadCounts DepartmentTree::load(const std::string& path) {
    LoadCounts counts;
    std::ifstream in(path);
    if (!in) { std::cout << "ERROR cannot open " << path << '\n'; return counts; }
    std::string line;
    std::size_t lineNum = 0;
    while (std::getline(in, line)) {
        ++lineNum;
        if (ignoreLine(line)) continue;
        const auto f = splitFields(line, '|');
        std::string reason;
        if (f.size() != 2) reason = "expected 2 fields: department|parent";
        else if (f[0].empty() || f[1].empty()) reason = "empty required field";
        else if (!root_ && upper(f[1]) != "NONE") reason = "first accepted department must have parent NONE";
        else add(f[0], f[1], reason);
        if (!reason.empty()) { warnRecord(path, lineNum, reason); ++counts.rejected; }
        else ++counts.accepted;
    }
    return counts;
}
// Traverse up parent pointers to compute depth measured in edges.
void DepartmentTree::search(const std::string& name) const {
    const Node* node = find(root_.get(), upper(trim(name)));
    if (!node) { std::cout << "Department not found: " << trim(name) << '\n'; return; }
    std::size_t depth = 0;
    for (auto p = node->parent; p; p = p->parent) ++depth;
    std::cout << "Name: " << node->name << "\nParent: " << (node->parent ? node->parent->name : "None")
              << "\nChildren: " << node->children.size() << "\nDepth: " << depth << '\n';
}
// Four spaces per depth; sibling insertion order stays unchanged.
void DepartmentTree::hierarchy(const Node* node, std::size_t depth) {
    if (!node) return;
    std::cout << std::string(depth * 4, ' ') << node->name << '\n';
    for (const auto& child : node->children) hierarchy(child.get(), depth + 1);
}
// For an empty tree, display a clear explanatory message.
void DepartmentTree::displayHierarchy() const {
    if (!root_) { std::cout << "Department tree is empty.\n"; return; }
    hierarchy(root_.get(), 0);
}
// Visit node before each child, separated by arrows.
void DepartmentTree::preorder(const Node* node, bool& first) {
    if (!node) return;
    if (!first) std::cout << " -> ";
    first = false;
    std::cout << node->name;
    for (const auto& child : node->children) preorder(child.get(), first);
}
// Print traversal as one sequence, distinct from the indented hierarchy.
void DepartmentTree::printPreorder() const {
    if (!root_) { std::cout << "Department tree is empty.\n"; return; }
    bool first = true;
    preorder(root_.get(), first);
    std::cout << '\n';
}
