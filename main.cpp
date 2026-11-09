#include "CampusGraph.h"
#include "FacilityDatabase.h"
#include "DepartmentTree.h"
#include "StringUtils.h"
#include <iostream>
#include <string>


static void printMenu() {
    std::cout << "\n===== Smart Campus Navigation and Facility Analytics =====\n"
              << "1. View all facilities\n"
              << "2. Search facility by ID\n"
              << "3. View campus graph\n"
              << "4. Display graph counts and degrees\n"
              << "5. Find reachable buildings (BFS)\n"
              << "6. Add department\n"
              << "7. Search department\n"
              << "8. Display department hierarchy\n"
              << "9. Run preorder traversal\n"
              << "0. Exit\n";
}

static bool readLine(const std::string& prompt, std::string& out) {
    std::cout << prompt;
    std::string line;
    if (!std::getline(std::cin, line)) return false;
    out = util::trim(line);
    return true;
}


static bool promptName(const std::string& prompt, std::string& out) {
    if (!readLine(prompt, out)) return false;
    if (out.empty()) { std::cout << "Error: name cannot be blank.\n"; return false; }
    return true;
}


static int parseChoice(const std::string& s) {
    if (s.size() != 1 || s[0] < '0' || s[0] > '9') return -1;
    return s[0] - '0';
}

// Join directory and filename.
static std::string joinPath(const std::string& dir, const std::string& file) {
    if (dir.empty() || dir[dir.size() - 1] == '/') return dir + file;
    return dir + "/" + file;
}

int main(int argc, char* argv[]) {
    std::string dir = (argc > 1) ? argv[1] : "data";
    CampusGraph graph;
    FacilityDatabase facilities;
    DepartmentTree departments;

 
    graph.loadBuildings(joinPath(dir, "buildings.txt"), "buildings.txt");
    graph.loadConnections(joinPath(dir, "connections.txt"), "connections.txt");
    facilities.loadFromFile(joinPath(dir, "facilities.txt"), "facilities.txt", graph);
    departments.loadFromFile(joinPath(dir, "departments.txt"), "departments.txt");

    std::string input;
    while (true) {
        printMenu();
        if (!readLine("Enter choice: ", input)) break;  // end of input -> exit
        int choice = parseChoice(input);
        std::string name, parent, err;
        switch (choice) {
            case 1: facilities.displayAll(); break;
            case 2:
                if (promptName("Enter facility ID: ", name)) facilities.searchById(name);
                break;
            case 3: graph.displayGraph(); break;
            case 4: graph.displayCountsAndDegrees(); break;
            case 5:
                if (promptName("Enter starting building: ", name)) graph.bfsReachable(name);
                break;
            case 6:
                if (!promptName("Enter new department name: ", name)) break;
                if (departments.isEmpty()) {
                    std::cout << "Tree is empty; '" << name << "' will be the root.\n";
                    parent = "";
                } else if (!promptName("Enter parent department name: ", parent)) break;
                if (departments.addDepartment(name, parent, err))
                    std::cout << "Added department '" << name << "'.\n";
                else
                    std::cout << "Error: " << err << ". Tree unchanged.\n";
                break;
            case 7:
                if (departments.isEmpty()) { std::cout << "No departments are loaded.\n"; break; }
                if (promptName("Enter department name: ", name)) departments.searchAndDisplay(name);
                break;
            case 8: departments.displayHierarchy(); break;
            case 9: departments.displayPreorder(); break;
            case 0: std::cout << "Goodbye.\n"; return 0;
            default:
                std::cout << "Error: invalid choice '" << input << "'. Enter a number from 0 to 9.\n";
        }
    }
    return 0;
}
