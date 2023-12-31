constexpr auto title = "--- Day 14: Parabolic Reflector Dish ---";

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

using namespace std;
using namespace chrono;

static void four_way_roll(vector<string>& g) {
    bool changed = true;
    while (changed) {
        changed = false;
        for (int x = 0; x < g[0].size(); x++)
            for (int y = 0; y < g.size() - 1; y++) {
                auto& curr = g[y][x];
                auto& above = g[y + 1][x];
                if (curr == '.' && above == 'O') {
                    swap(curr, above);
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
                auto& above = g[y][x + 1];
                if (curr == '.' && above == 'O') {
                    swap(curr, above);
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
                auto& above = g[y - 1][x];
                if (curr == '.' && above == 'O') {
                    swap(curr, above);
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
                auto& above = g[y][x - 1];
                if (curr == '.' && above == 'O') {
                    swap(curr, above);
                    changed = true;
                }
            }
    }
}

static string join(const vector<string>& g) {
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
        const auto key = join(g);
        if (grids.find(key) != grids.end()) {
            cycle_start = grids.at(key);
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
                auto& above = g[y + 1][x];
                if (curr == '.' && above == 'O') {
                    swap(curr, above);
                    changed = true;
                }
            }
        i++;
    }
    return total_load(g);
}

int main() {
    auto start = high_resolution_clock::now();
    ifstream inf("day14.txt");
    string line;
    vector<string> grid;
    while (getline(inf, line)) grid.push_back(line);
    cout << title << endl
         << "Part 1  - " << part1(grid) << endl
         << "Part 2  - " << part2(grid) << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0
         << " ms." << endl;
}
