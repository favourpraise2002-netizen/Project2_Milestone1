#!/usr/bin/env python3
"""Executable smoke tests for Milestone 1, using real input and observed output."""
from pathlib import Path
import subprocess
root = Path(__file__).resolve().parents[1]
exe = root / 'campus'

def run(folder, actions='10\n'):
    result = subprocess.run([str(exe), folder], input=actions, text=True,
                            capture_output=True, cwd=root, timeout=5)
    return result.returncode, result.stdout + result.stderr

tests = [
 ('T01', 'Sample load summaries', 'data', '10\n',
  ['buildings.txt: 7 accepted, 0 rejected', 'connections.txt: 6 accepted, 0 rejected',
   'facilities.txt: 10 accepted, 0 rejected', 'departments.txt: 7 accepted, 0 rejected']),
 ('T02', 'Facility searches: found, lowercase, invalid and missing', 'data',
  '2\nf004\n2\nF004\n2\n004\n2\nf999\n10\n',
  ['Found: F004 | Computer Lab A', 'Invalid ID format', 'Not found: no facility with ID F999']),
 ('T03', 'Graph display, degrees, isolated building, BFS from library', 'data',
  '3\n4\n5\n Library \n5\nObservatory\n5\nMissing Hall\n10\n',
  ['Observatory: (no connections)', 'Total buildings: 7', 'Total connections: 6',
   'Engineering Building - degree 1', 'Reachable: 6 of 7 buildings',
   'Reachable: 1 of 7 buildings', 'unknown building: Missing Hall']),
 ('T04', 'Tree root / child / missing and preorder / add', 'data',
  '7\nUniversity\n7\nComputer Science\n7\nMissing\n6\nNew Department\nUniversity\n8\n9\n10\n',
  ['Parent: None', 'Children: 2', 'Depth: 2', 'Department not found: Missing',
   'Added department: New Department', '    New Department', 'University ->']),
 ('T05', 'Collision and duplicate ID: 2 accepted, 1 rejected, 1 collision',
  'testdata/collision', '1\n2\nf001\n2\nF014\n10\n',
  ['facilities.txt: 2 accepted, 1 rejected', 'Hash capacity: 17, entries: 2, collisions: 1',
   'duplicate facility ID: F001', 'F001 | First Room', 'F014 | Second Room']),
 ('T06', 'Malformed lines, CRLF, reversed edges, unknown refs, duplicate case',
  'testdata/validation', '1\n3\n8\n10\n',
  ['buildings.txt: 3 accepted, 2 rejected',
   'connections.txt: 2 accepted, 5 rejected',
   'facilities.txt: 3 accepted, 5 rejected',
   'departments.txt: 3 accepted, 4 rejected',
   'duplicate or reversed connection', 'unknown building: Missing Building',
   'F004 | Lab 1 | Library', 'F005 | Room #5 | STUDENT UNION']),
 ('T07', 'Empty files, empty tree root addition without parent prompt',
  'testdata/empty', '3\n4\n5\nLibrary\n8\n9\n6\nNew Root Name\n7\nnew root name\n10\n',
  ['No buildings available.', 'Total buildings: 0',
   'Department tree is empty.', 'Added department: New Root Name', 'Parent: None', 'Depth: 0']),
 ('T08', 'Missing file continues without crash', 'testdata/missing', '10\n',
  ['ERROR cannot open testdata/missing/connections.txt',
   'connections.txt: 0 accepted, 0 rejected', 'facilities.txt: 1 accepted, 0 rejected']),
 ('T09', 'Bad menu input/blank names recovers and returns to menu', 'data',
  '\nabc\n-1\n999\n2\n \n5\n \n6\n \n0\n',
  ['Error: invalid menu choice.', 'Error: name cannot be blank.', 'Exiting.']),
 ('T10', 'Tree rejects repeated names and unknown parents in interactive additions', 'data',
  '6\nPhysics\nUniversity\n6\nNew Program\nUnknown Parent\n6\nNew Program\nCollege of Science\n7\nnew program\n10\n',
  ['duplicate department name: Physics', 'unknown parent department: Unknown Parent',
   'Added department: New Program', 'Parent: College of Science']),
 ('T11', 'Root-only tree reports child count and preorder', 'testdata/collision',
  '7\nUniversity\n8\n9\n10\n',
  ['Name: University', 'Parent: None', 'Children: 0', 'Depth: 0', 'Choice: University']),
]
results = []
for number, desc, folder, actions, expected in tests:
    code, output = run(folder, actions)
    missing = [x for x in expected if x not in output]
    ok = code == 0 and not missing
    results.append((number, desc, folder, actions, expected, output, ok, missing))
    print(number, 'PASS' if ok else 'FAIL', desc)
    if missing: print('  Missing expected:', missing)
report = ['PROJECT 2 MILESTONE 1 - TEST RESULTS',
          'Execution environment: local Linux container, g++ C++17; CELL not yet verified.',
          'Command: make && python3 tests/run_tests.py',
          'Do not mark CELL testing completed until separately performed on CELL.', '']
for number, desc, folder, actions, expected, output, ok, missing in results:
    extracted = [x for x in expected if x in output]
    report.extend([f'{number}: {desc}', f'Input directory: {folder}',
                   f'Input menu lines (\\n-separated): {actions.rstrip()!r}',
                   'Expected: ' + '; '.join(expected),
                   'Actual (confirmed excerpts): ' + ('; '.join(extracted) or '(none matched)'),
                   f'Result: {"PASS" if ok else "FAIL"} (exit code {0 if ok else "check output"})', ''])
(root/'test_results.txt').write_text('\n'.join(report))
print(f'Passed {sum(row[-2] for row in results)}/{len(results)} smoke tests')
raise SystemExit(0 if all(row[-2] for row in results) else 1)
