#ifndef DEPARTMENT_TREE_H
#define DEPARTMENT_TREE_H
#include <string>
#include <vector>


struct DepartmentNode {
    std::string name;
    DepartmentNode* parent;
    std::vector<DepartmentNode*> children;
    int depth;
    DepartmentNode(const std::string& n, DepartmentNode* p, int d)
        : name(n), parent(p), depth(d) {}
};

class DepartmentTree {
public:
    DepartmentTree();
    ~DepartmentTree();
    DepartmentTree(const DepartmentTree&) = delete;
    DepartmentTree& operator=(const DepartmentTree&) = delete;

    bool addDepartment(const std::string& name, const std::string& parentName,
                       std::string& error);

    void loadFromFile(const std::string& path, const std::string& label);

    bool isEmpty() const { return root == nullptr; }
    const DepartmentNode* find(const std::string& name) const;  

    void searchAndDisplay(const std::string& name) const;  
    void displayHierarchy() const;                         
    std::vector<std::string> preorder() const;             
    void displayPreorder() const;

private:
    DepartmentNode* root;
    std::vector<DepartmentNode*> nodes;  

    static void destroy(DepartmentNode* n);
    static void printIndented(const DepartmentNode* n);
    static void collectPreorder(const DepartmentNode* n, std::vector<std::string>& out);
};
#endif
