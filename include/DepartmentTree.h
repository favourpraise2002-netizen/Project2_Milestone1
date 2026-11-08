// Purpose: General academic department tree with preorder traversal and menu operations.
// Module owner: Ademola (starter interface; review, implement, and commit changes).
#pragma once
#include "Utils.h"
#include <memory>
#include <string>
#include <vector>

class DepartmentTree {
public:
    // Load root and children, only accepting previously loaded parents.
    LoadCounts load(const std::string& path);
    // Add one department in memory, returning false and setting reason on failure.
    bool add(const std::string& name, const std::string& parent, std::string& reason);
    // Display name, parent, child count and depth or not-found.
    void search(const std::string& name) const;
    // Display hierarchy with four spaces per depth level.
    void displayHierarchy() const;
    // Display preorder visit sequence without indentation.
    void printPreorder() const;
    // Signal that an added department should be the root, with no parent prompt.
    bool empty() const { return !root_; }
private:
    struct Node {
        std::string name;
        Node* parent;
        std::vector<std::unique_ptr<Node>> children;
    };
    std::unique_ptr<Node> root_;
    // Search a general tree for a case-insensitive name.
    static const Node* find(const Node* node, const std::string& normalized);
    // Mutable version used to attach children.
    static Node* find(Node* node, const std::string& normalized);
    // Recursively print preorder with depth-dependent indentation.
    static void hierarchy(const Node* node, std::size_t depth);
    // Recursively print preorder names without indentation.
    static void preorder(const Node* node, bool& first);
};
