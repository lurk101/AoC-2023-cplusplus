constexpr auto title = "--- Day 3: Gear Ratios ---";

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../runall.h"

using namespace std;
using namespace chrono;

struct part {
    int val, y, x_start, x_end;
};

struct gear {
    int x, y, count, product;
};

static vector<string> engine;
static vector<part> parts;
static vector<gear> gears;

static void update_gear(int x, int y, int v) {
    for (auto& g : gears)
        if (g.x == x && g.y == y) {
            g.count++;
            g.product *= v;
            break;
        }
}

void day03(struct result& r) {
    auto start = high_resolution_clock::now();
    ifstream f("day03/day03.txt");
    string line;
    while (getline(f, line)) engine.push_back(line);
    const int x_max = engine[0].size() - 1;
    const int y_max = engine.size() - 1;
    for (int y = 0; y <= y_max; y++) {
        line = engine[y];
        for (int x = 0; x <= x_max; x++) {
            if (line[x] == '*') {
                gear g({x, y, 0, 1});
                gears.push_back(g);
            }
            if (!isdigit(line[x])) continue;
            part p({0, y, x, 0});
            int x2;
            for (x2 = x; x2 <= x_max; x2++) {
                if (!isdigit(line[x2])) break;
                p.val = p.val * 10 + line[x2] - '0';
            }
            x = x2 - 1;
            p.x_end = x;
            parts.push_back(p);
        }
    }
    int part1 = 0;
    for (auto& p : parts) {
        bool good_part = false;
        if (p.x_start > 0 && engine[p.y][p.x_start - 1] != '.') {
            good_part = true;
            if (engine[p.y][p.x_start - 1] == '*') update_gear(p.x_start - 1, p.y, p.val);
        }
        if (p.x_end < x_max && engine[p.y][p.x_end + 1] != '.') {
            good_part = true;
            if (engine[p.y][p.x_end + 1] == '*') update_gear(p.x_end + 1, p.y, p.val);
        }
        int start = max(0, p.x_start - 1);
        int end = min(x_max, p.x_end + 1);
        if (p.y > 0)
            for (int x = start; x <= end; x++)
                if (engine[p.y - 1][x] != '.') {
                    good_part = true;
                    if (engine[p.y - 1][x] == '*') update_gear(x, p.y - 1, p.val);
                }
        if (p.y < y_max)
            for (int x = start; x <= end; x++)
                if (engine[p.y + 1][x] != '.') {
                    good_part = true;
                    if (engine[p.y + 1][x] == '*') update_gear(x, p.y + 1, p.val);
                }
        if (good_part) part1 += p.val;
    }
    int part2 = 0;
    for (auto& g : gears)
        if (g.count == 2) part2 += g.product;
    stringstream ss;
    ss << part1;
    r.p1 = ss.str();
    ss.str("");
    ss << part2;
    r.p2 = ss.str();
    r.t = duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0;
    if (coutflag)
        cout << title << endl
             << "Part 1  - " << r.p1 << endl
             << "Part 2  - " << r.p2 << endl
             << "Elapsed - " << r.t << " ms." << endl;
    engine.clear();
    parts.clear();
    gears.clear();
}
