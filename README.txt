PROJECT 2: SMART CAMPUS NAVIGATION AND FACILITY ANALYTICS SYSTEM
Milestone 1 starter, C++17. Group: Favour, Ken, Ademola.

Purpose:
Load four campus data files; display facilities and hash-search facility IDs;
display a dynamic undirected building graph and BFS reachability; manage a
general department tree with search, hierarchy and preorder traversal.
Only M1 features are required here. Final-only features are not implemented.

Build (from Project2/; on a machine with g++):
    make
OR  g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp -o campus

Run (from Project2/):
    ./campus
    ./campus data
    ./campus testdata/collision
    ./campus testdata/validation
    ./campus testdata/empty
    ./campus testdata/missing
On Windows Command Prompt, use campus.exe instead of ./campus.
On Linux/CELL, use ./campus. Test on UNT CELL before submitting.
To delete compiled files: make clean

Directory layout:
    include/            C++ declarations: Utils, HashTable, FacilityDatabase,
                        CampusGraph, DepartmentTree
    src/                C++ implementations + main.cpp
    data/               UNCHANGED instructor-supplied four input files
    testdata/           Our own independent collision/validation/empty/missing cases
    tests/              Local smoke/regression test runner
    README.txt          Build/run instructions
    design_document.txt Data structure design decisions
    test_results.txt    Actual local test observations
    CONTRIBUTIONS.txt   Team ownership; update with actual contribution details
    Makefile            Portable C++17 build rule
    .gitignore          Excludes generated binaries and editor files

Input order: buildings.txt, connections.txt, facilities.txt, departments.txt.
Files are read from the optional data-directory argument (default data/).
Malformed records produce filename, line number and reason without ending load.
Blank lines and comments are ignored. IDs and name lookup are case-insensitive.

Required M1 menu prints options 1 through 10. Both 10 (listed in Section
6.9) and 0 (required by UI-7) exit cleanly.

IMPORTANT: These are locally tested starter files. Compile/run on UNT CELL,
review every module as a team, and confirm actual source authorship, GitHub
history, ZIP contents and teamwork before submitting. Do NOT upload campus
executable; it is gitignored and excluded from the ZIP.
