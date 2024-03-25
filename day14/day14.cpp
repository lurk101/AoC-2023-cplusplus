constexpr auto title = "--- Day 14: Parabolic Reflector Dish ---";

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

#include "../runall.h"

using namespace std;
using namespace chrono;

static void four_way_roll(vector<string>& g) {
    bool changed = true;
    while (changed) {
        changed = false;
        for (int x = 0; x < g[0].size(); x++)
            for (int y = 0; y < g.size() - 1; y++) {
                auto& curr = g[y][x];
                auto& down = g[y + 1][x];
                if (curr == '.' && down == 'O') {
                    swap(curr, down);
                    changed = true;
                }
            }
    }
    changed = true;
    while (changed) {
        changed = false;
        for (int y = 0; y < g.size(); y++)
            for (int x = 0; x < g[0].size() - 1; x++) {
                auto& curr = g[y][x];
                auto& right = g[y][x + 1];
                if (curr == '.' && right == 'O') {
                    swap(curr, right);
                    changed = true;
                }
            }
    }
    changed = true;
    while (changed) {
        changed = false;
        for (int x = 0; x < g[0].size(); x++)
            for (int y = g.size() - 1; y != 0; y--) {
                auto& curr = g[y][x];
                auto& up = g[y - 1][x];
                if (curr == '.' && up == 'O') {
                    swap(curr, up);
                    changed = true;
                }
            }
    }
    changed = true;
    while (changed) {
        changed = false;
        for (int y = 0; y < g.size(); y++)
            for (int x = g[0].size() - 1; x != 0; x--) {
                auto& curr = g[y][x];
                auto& left = g[y][x - 1];
                if (curr == '.' && left == 'O') {
                    swap(curr, left);
                    changed = true;
                }
            }
    }
}

static string concat(const vector<string>& g) {
    string r;
    for (auto& s : g) r += s;
    return r;
}

static int total_load(const vector<string>& g) {
    int sum = 0;
    for (int i = 0; i < g.size(); i++)
        for (int x = 0; x < g[0].size(); x++)
            if (g[i][x] == 'O') sum += g.size() - i;
    return sum;
}

static int part2(vector<string> g) {
    map<string, int> grids;
    map<int, int> loads;
    int i = 0, cycle_start = 0, cycle_end = 0;
    constexpr int target = 1000000000;
    while (true) {
        four_way_roll(g);
        i++;
        const auto key = concat(g);
        if (grids.find(key) != grids.end()) {
            cycle_start = grids[key];
            cycle_end = i;
            break;
        } else {
            grids[key] = i;
            loads[i] = total_load(g);
        }
    }
    const auto period = cycle_end - cycle_start;
    const auto cycle = target - cycle_start;
    return loads[cycle_start + cycle % period];
}

static int part1(vector<string> g) {
    int i = 1;
    bool changed = true;
    while (changed) {
        changed = false;
        for (int x = 0; x < g[0].size(); x++)
            for (int y = 0; y < g.size() - 1; y++) {
                auto& curr = g[y][x];
                auto& up = g[y + 1][x];
                if (curr == '.' && up == 'O') {
                    swap(curr, up);
                    changed = true;
                }
            }
        i++;
    }
    return total_load(g);
}

void day14(struct result& r) {
    auto start = high_resolution_clock::now();
    ifstream inf("day14/day14.txt");
    string line;
    vector<string> grid;
    while (getline(inf, line)) grid.push_back(line);
    stringstream ss;
    ss << part1(grid);
    r.p1 = ss.str();
    ss.str("");
    ss << part2(grid);
    r.p2 = ss.str();
    r.t = duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0;
    cout << title << endl
         << "Part 1  - " << r.p1 << endl
         << "Part 2  - " << r.p2 << endl
         << "Elapsed - " << r.t << " ms." << endl;
}
