auto const title = "--- Day 21: Step Counter ---";

#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <sstream>

#include "../runall.h"

using namespace std;
using namespace chrono;

using position = array<int, 2>;

struct hash_pos {
    size_t operator()(const position& p) const { return p[0] | (size_t(p[1]) << 32); }
};

static unordered_set<position, hash_pos> start_positions, visited;
static vector<string> grid;

static void update_pos() {
    unordered_set<position, hash_pos> next_positions;
    for (const auto& pos : start_positions) {
        if (pos[0] > 0 && grid[pos[0] - 1][pos[1]] != '#')
            next_positions.insert({pos[0] - 1, pos[1]});
        if (pos[0] < grid.size() - 1 && grid[pos[0] + 1][pos[1]] != '#')
            next_positions.insert({pos[0] + 1, pos[1]});
        if (pos[1] > 0 && grid[pos[0]][pos[1] - 1] != '#')
            next_positions.insert({pos[0], pos[1] - 1});
        if (pos[1] < grid[pos[0]].size() - 1 && grid[pos[0]][pos[1] + 1] != '#')
            next_positions.insert({pos[0], pos[1] + 1});
    }
    start_positions = next_positions;
}

static void search() {
    unordered_set<position, hash_pos> next_pos;
    int height = grid.size();
    int width = grid[0].size();
    for (const auto& pos : start_positions) {
        array<position, 4> moves{{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}};
        for (const auto& mov : moves) {
            position check_pos{(((pos[0] + mov[0]) % height) + height) % height,
                               (((pos[1] + mov[1]) % width) + width) % width};
            if (grid[check_pos[0]][check_pos[1]] != '#' &&
                visited.find({pos[0] + mov[0], pos[1] + mov[1]}) == visited.end()) {
                next_pos.insert({pos[0] + mov[0], pos[1] + mov[1]});
                visited.insert({pos[0] + mov[0], pos[1] + mov[1]});
            }
        }
    }
    start_positions = next_pos;
}

void day21(struct result& r) {
    auto start = high_resolution_clock::now();
    ifstream f("day21/day21.txt");
    position start_pos;
    string line;
    int row = 0;
    while (getline(f, line)) {
        grid.push_back(line);
        size_t col = line.find('S');
        if (col != string::npos) {
            start_pos[0] = row;
            start_pos[1] = col;
            grid[row][col] = '.';
        }
        ++row;
    }
    start_positions.insert(start_pos);
    int steps = 64;
    while (steps--) update_pos();
    auto part1 = start_positions.size();
    start_positions.clear();
    start_positions.insert(start_pos);
    visited.insert(start_pos);
    unordered_map<position, int, hash_pos> offs;
    offs[start_pos] = 0;
    steps = 26501365;
    vector<int> parms, lengths;
    lengths.push_back(1);
    for (int i = 0; i < steps; ++i) {
        search();
        lengths.push_back(start_positions.size());
        if (i % grid.size() == steps % grid.size()) {
            int len = 0;
            for (int j = 0; j < lengths.size() - 1; ++j)
                if (j % 2 == i % 2) len += lengths[j];
            parms.push_back(len);
        }
        if (parms.size() == 3) break;
    }
    int64_t p1 = parms[0], p2 = parms[1] - parms[0], p3 = parms[2] - parms[1],
            p = steps / grid.size(), part2 = p1 + p2 * p + (p * (p - 1) / 2) * (p3 - p2);
    stringstream ss;
    ss << part1;
    r.p1 = ss.str();
    ss.str("");
    ss << part2;
    r.p2 = ss.str();
    r.t = duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0;
    cout << title << endl
         << "Part 1  - " << r.p1 << endl
         << "Part 2  - " << r.p2 << endl
         << "Elapsed - " << r.t << " ms." << endl;
}
