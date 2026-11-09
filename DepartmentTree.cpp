#include "DepartmentTree.h"
#include "StringUtils.h"
#include <fstream>
#include <iostream>

DepartmentTree::DepartmentTree() : root(nullptr) {}

DepartmentTree::~DepartmentTree() { destroy(root); }

void DepartmentTree::destroy(DepartmentNode* n) {
    if (!n) return;
    for (size_t i = 0; i < n->children.size(); ++i) destroy(n->children[i]);
    delete n;
}


const DepartmentNode* DepartmentTree::find(const std::string& name) const {
    std::string key = util::trim(name);
    for (size_t i = 0; i < nodes.size(); ++i)
        if (util::equalsIgnoreCase(nodes[i]->name, key)) return nodes[i];
    return nullptr;
}


bool DepartmentTree::addDepartment(const std::string& rawName, const std::string& rawParent,
                                   std::string& error) {
    std::string name = util::trim(rawName);
    std::string parentName = util::trim(rawParent);
    if (name.empty()) { error = "department name is empty"; return false; }
    if (find(name)) { error = "duplicate department name '" + name + "'"; return false; }

    if (root == nullptr) {
        root = new DepartmentNode(name, nullptr, 0);
        nodes.push_back(root);
        return true;
    }
    if (parentName.empty()) { error = "parent name is empty"; return false; }
    const DepartmentNode* p = find(parentName);
    if (!p) { error = "unknown parent '" + parentName + "'"; return false; }

    DepartmentNode* parent = const_cast<DepartmentNode*>(p);
    DepartmentNode* child = new DepartmentNode(name, parent, parent->depth + 1);
    parent->children.push_back(child);
    nodes.push_back(child);
    return true;
}


void DepartmentTree::loadFromFile(const std::string& path, const std::string& label) {
    int accepted = 0, rejected = 0;
    std::ifstream in(path.c_str());
    if (!in.is_open()) {
        std::cout << "ERROR: cannot open " << label << " (" << path
                  << "); treating as empty.\n";
    } else {
        std::string line;
        int lineNo = 0;
        while (std::getline(in, line)) {
            ++lineNo;
            if (util::isBlankOrComment(line)) continue;
            std::vector<std::string> f = util::splitFields(line);
            std::string reason;
            if (f.size() != 2) {
                reason = "expected 2 fields, found " + std::to_string(f.size());
            } else if (f[0].empty() || f[1].empty()) {
                reason = "empty required field";
            } else if (root == nullptr) {
                if (!util::equalsIgnoreCase(f[1], "NONE"))
                    reason = "first department (root) must have parent NONE";
                else
                    addDepartment(f[0], "", reason);  
            } else {
                addDepartment(f[0], f[1], reason);
            }
            if (reason.empty()) ++accepted;
            else {
                ++rejected;
                std::cout << "WARNING: " << label << " line " << lineNo
                          << ": rejected - " << reason << "\n";
            }
        }
    }
    std::cout << label << ": " << accepted << " accepted, " << rejected << " rejected\n";
}


void DepartmentTree::searchAndDisplay(const std::string& name) const {
    const DepartmentNode* n = find(name);
    if (!n) { std::cout << "Department '" << util::trim(name) << "' not found.\n"; return; }
    std::cout << "Department: " << n->name << "\n"
              << "Parent: " << (n->parent ? n->parent->name : "None") << "\n"
              << "Direct children: " << n->children.size() << "\n"
              << "Depth: " << n->depth << "\n";
}


void DepartmentTree::printIndented(const DepartmentNode* n) {
    std::cout << std::string(4 * n->depth, ' ') << n->name << "\n";
    for (size_t i = 0; i < n->children.size(); ++i) printIndented(n->children[i]);
}


void DepartmentTree::displayHierarchy() const {
    if (!root) { std::cout << "No departments are loaded.\n"; return; }
    printIndented(root);
}


void DepartmentTree::collectPreorder(const DepartmentNode* n, std::vector<std::string>& out) {
    out.push_back(n->name);
    for (size_t i = 0; i < n->children.size(); ++i) collectPreorder(n->children[i], out);
}


std::vector<std::string> DepartmentTree::preorder() const {
    std::vector<std::string> out;
    if (root) collectPreorder(root, out);
    return out;
}


void DepartmentTree::displayPreorder() const {
    std::vector<std::string> seq = preorder();
    if (seq.empty()) { std::cout << "No departments are loaded.\n"; return; }
    std::cout << "Preorder: ";
    for (size_t i = 0; i < seq.size(); ++i) std::cout << (i ? " -> " : "") << seq[i];
    std::cout << "\n";
}
