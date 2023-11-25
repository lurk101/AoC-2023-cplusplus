auto const title = "--- Day 23: A Long Walk ---";

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;
using namespace chrono;

static vector<string> lines;
static int rows, columns;

using grid_t = vector<vector<int>>;
using visited_t = vector<vector<bool>>;

static grid_t grid;
static visited_t visited;

static void visit1(int r, int c, int steps) {
    if (lines[r][c] == '#') return;
    if (visited[r][c]) return;
    visited[r][c] = true;
    grid[r][c] = max(grid[r][c], steps);
    if ((c > 0) && (lines[r][c] == '.') || (lines[r][c] == '<')) visit1(r, c - 1, steps + 1);
    if (((c + 1) < columns) && (lines[r][c] == '.') || (lines[r][c] == '>'))
        visit1(r, c + 1, steps + 1);
    if ((r > 0) && (lines[r][c] == '.') || (lines[r][c] == '^')) visit1(r - 1, c, steps + 1);
    if (((r + 1) < rows) && (lines[r][c] == '.') || (lines[r][c] == 'v'))
        visit1(r + 1, c, steps + 1);
    visited[r][c] = false;
}

static int part1() {
    grid = grid_t(rows, vector<int>(columns, 0));
    visited = visited_t(rows, vector<bool>(columns, false));
    visit1(0, 1, 0);
    return grid[rows - 1][columns - 2];
}

using position = pair<int, int>;

struct pos_hash {
    size_t operator()(const position& p) const { return (size_t(p.second) << 32) | p.first; }
};

using nodes_t = unordered_set<position, pos_hash>;
using longest_paths_t = unordered_map<position, int, pos_hash>;
using edges_t = unordered_map<position, longest_paths_t, pos_hash>;
using visited2_t = unordered_map<position, bool, pos_hash>;

static nodes_t nodes;
static edges_t edges;
static longest_paths_t longest_paths;
static visited2_t visited2;

static void visit2(position node, int path) {
    if (visited2[node]) return;
    visited2[node] = true;
    longest_paths[node] = max(longest_paths[node], path);
    for (auto [node2, length] : edges[node]) visit2(node2, path + length);
    visited2[node] = false;
}

static tuple<int, int, int> run_length(int r, int c, char dir) {
    nodes_t visited;
    auto is_visited = [&](int r, int c) { return visited.find({r, c}) != visited.end(); };
    visited.insert({r, c});
    switch (dir) {
    case '<':
        c -= 1;
        break;
    case '>':
        c += 1;
        break;
    case '^':
        r -= 1;
        break;
    case 'v':
        r += 1;
        break;
    }
    int length = 0;
    if (lines[r][c] == '#') return make_tuple(r, c, length);
    while (nodes.find({r, c}) == nodes.end()) {
        ++length;
        visited.insert({r, c});
        if ((c > 0) && (lines[r][c - 1] != '#') && !is_visited(r, c - 1))
            c -= 1;
        else if ((r > 0) && (lines[r - 1][c] != '#') && !is_visited(r - 1, c))
            r -= 1;
        else if (((c + 1) < columns) && (lines[r][c + 1] != '#') && !is_visited(r, c + 1))
            c += 1;
        else if (((r + 1) < rows) && (lines[r + 1][c] != '#') && !is_visited(r + 1, c))
            r += 1;
    }
    return make_tuple(r, c, length + 1);
}

static int part2() {
    nodes.insert({0, 1});
    nodes.insert({rows - 1, columns - 2});
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < columns; c++) {
            if (lines[r][c] == '#') continue;
            int exits = 0;
            if (c > 0) exits += (lines[r][c - 1] != '#');
            if ((c + 1) < columns) exits += (lines[r][c + 1] != '#');
            if (r > 0) exits += (lines[r - 1][c] != '#');
            if ((r + 1) < rows) exits += (lines[r + 1][c] != '#');
            if (exits > 2) nodes.insert({r, c});
        }
    for (auto [r, c] : nodes) {
        if (c > 0) {
            auto [r2, c2, len] = run_length(r, c, '<');
            if (len > 1) edges[{r, c}][{r2, c2}] = len;
        }
        if ((c + 1) < columns) {
            auto [r2, c2, len] = run_length(r, c, '>');
            if (len > 1) edges[{r, c}][{r2, c2}] = len;
        }
        if (r > 0) {
            auto [r2, c2, len] = run_length(r, c, '^');
            if (len > 1) edges[{r, c}][{r2, c2}] = len;
        }
        if ((r + 1) < rows) {
            auto [r2, c2, len] = run_length(r, c, 'v');
            if (len > 1) edges[{r, c}][{r2, c2}] = len;
        }
    }
    visit2(position{0, 1}, 0);
    return longest_paths[position{rows - 1, columns - 2}];
}

int main() {
    auto start = high_resolution_clock::now();
    ifstream f("day23.txt");
    string line;
    while (f >> line) lines.push_back(line);
    rows = lines.size();
    columns = lines[0].size();
    cout << title << endl
         << "Part 1 - " << part1() << endl
         << "Part 2 - " << part2() << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0
         << " ms." << endl;
}
