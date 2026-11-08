// Purpose: Coordinate four loaders and expose the exact Milestone 1 menu.
// Author: Project 2 team starter; confirm individual authors before submission.
#include "CampusGraph.h"
#include "DepartmentTree.h"
#include "FacilityDatabase.h"
#include "Utils.h"
#include <iostream>
#include <string>

// Print exact assignment menu numbering while also recognizing 0 as Exit per UI-7.
static void printMenu() {
    std::cout << "\n--- Smart Campus: Milestone 1 ---\n"
              << "1. View all facilities\n"
              << "2. Search facility by ID\n"
              << "3. View campus graph\n"
              << "4. Display graph counts and degrees\n"
              << "5. Find reachable buildings (BFS)\n"
              << "6. Add department\n"
              << "7. Search department\n"
              << "8. Display department hierarchy\n"
              << "9. Run preorder traversal\n"
              << "10. Exit\nChoice: ";
}
// Read full line to allow spaces and safely recover from unexpected input.
static bool getNonblankName(const std::string& prompt, std::string& name) {
    std::cout << prompt;
    if (!std::getline(std::cin, name)) return false;
    name = trim(name);
    if (name.empty()) std::cout << "Error: name cannot be blank.\n";
    return !name.empty();
}
// Menu parsing accepts digits only (with outer whitespace); bad text is rejected.
static int parseChoice(const std::string& line) {
    std::string s = trim(line);
    if (s.empty()) return -1;
    for (char ch : s) if (ch < '0' || ch > '9') return -1;
    if (s.size() > 2) return -1;
    int choice = std::stoi(s);
    return (choice >= 0 && choice <= 10) ? choice : -1;
}
// Main performs coordination only; data structures own validation and operations.
int main(int argc, char* argv[]) {
    if (argc > 2) { std::cerr << "Usage: " << argv[0] << " [data-directory]\n"; return 1; }
    const std::string directory = argc == 2 ? argv[1] : "data";
    CampusGraph campus;
    FacilityDatabase facilities;
    DepartmentTree departments;
    printSummary("buildings.txt", campus.loadBuildings(dataPath(directory, "buildings.txt")));
    printSummary("connections.txt", campus.loadConnections(dataPath(directory, "connections.txt")));
    printSummary("facilities.txt", facilities.load(dataPath(directory, "facilities.txt"), campus));
    facilities.printHashDiagnostics();
    printSummary("departments.txt", departments.load(dataPath(directory, "departments.txt")));
    while (true) {
        printMenu();
        std::string input;
        if (!std::getline(std::cin, input)) { std::cout << "\nExiting.\n"; break; }
        const int choice = parseChoice(input);
        if (choice == 0 || choice == 10) { std::cout << "Exiting.\n"; break; }
        std::string name, parent, reason;
        switch (choice) {
            case 1: facilities.display(); break;
            case 2:
                if (getNonblankName("Facility ID: ", name)) facilities.searchID(name);
                break;
            case 3: campus.display(); break;
            case 4: campus.statistics(); break;
            case 5:
                if (getNonblankName("Starting building: ", name)) campus.bfs(name);
                break;
            case 6:
                if (!getNonblankName("New department name: ", name)) break;
                if (!departments.empty() && !getNonblankName("Parent department: ", parent)) break;
                if (departments.add(name, parent, reason)) std::cout << "Added department: " << name << '\n';
                else std::cout << "Error: " << reason << '\n';
                break;
            case 7:
                if (getNonblankName("Department name: ", name)) departments.search(name);
                break;
            case 8: departments.displayHierarchy(); break;
            case 9: departments.printPreorder(); break;
            default: std::cout << "Error: invalid menu choice.\n"; break;
        }
    }
    return 0;
}
