constexpr auto title = "--- Day 13: Point of Incidence ---";

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "../runall.h"

using namespace std;
using namespace chrono;

static vector<string> transpose(vector<string> grid) {
    vector<string> v(grid[0].size());
    for (int i = 0; i < grid[0].size(); i++)
        for (int j = 0; j < grid.size(); j++) v[i] += grid[j][i];
    return v;
}

static int part1(const vector<string>& grid) {
    int count = 0;
    for (int i = 0; i < grid.size() - 1; i++) {
        int j = 0, reflection = 0;
        while (i - j >= 0 && i + j + 1 < grid.size() && grid[i - j] == grid[i + j + 1]) {
            reflection++;
            j++;
        }
        if (i - j + 1 == 0 || i + j + 1 == grid.size()) count = max(count, reflection + i - j + 1);
    }
    return count;
}

static bool fix_smudge(const string& line1, const string& line2) {
    bool updated = false;
    for (int i = 0; i < line1.size(); i++)
        if (line1[i] != line2[i]) {
            if (updated)
                return false;
            updated = true;
        }
    return true;
}

static int part2(const vector<string>& grid) {
    int count = 0;
    for (int i = 0; i < grid.size() - 1; i++) {
        int j = 0, reflection = 0;
        bool updated = false;
        while (i - j >= 0 && i + j + 1 < grid.size()) {
            if (grid[i - j] == grid[i + j + 1])
                reflection++;
            else if (!updated && fix_smudge(grid[i - j], grid[i + j + 1])) {
                updated = true;
                reflection++;
            } else
                break;
            j++;
        }
        if ((i - j + 1 == 0 || i + j + 1 == grid.size()) && updated)
            count = max(count, reflection + i - j + 1);
    }
    return count;
}

void day13(struct result& r) {
    auto start = high_resolution_clock::now();
    ifstream inf("day13/day13.txt");
    string line;
    uint32_t p1 = 0, p2 = 0;
    while (getline(inf, line)) {
        vector<string> grid;
        while (line.size() != 0) {
            grid.push_back(line);
            getline(inf, line);
        }
        uint32_t p = part1(grid) * 100;
        if (p == 0) p = part1(transpose(grid));
        p1 += p;
        p = part2(grid) * 100;
        if (p == 0) p = part2(transpose(grid));
        p2 += p;
    }
    stringstream ss;
    ss << p1;
    r.p1 = ss.str();
    ss.str("");
    ss << p2;
    r.p2 = ss.str();
    r.t = duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0;
    cout << title << endl
         << "Part 1  - " << r.p1 << endl
         << "Part 2  - " << r.p2 << endl
         << "Elapsed - " << r.t << " ms." << endl;
}
