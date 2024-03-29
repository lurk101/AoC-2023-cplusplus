auto constexpr title = "--- Day 23: A Long Walk ---";

#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <vector>

#include "../runall.h"

using namespace std;
using namespace chrono;

static vector<vector<char>> grid;

struct point_t {
    int x, y;
    point_t(int px, int py) {
        x = px;
        y = py;
    }
    point_t operator+(const point_t& other) const { return point_t(x + other.x, y + other.y); }
    bool operator<(const point_t& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
    bool operator==(const point_t& other) const { return y == other.y && x == other.x; }
    static point_t get_dir(const point_t& p) {
        switch (char c = grid[p.y][p.x]) {
        case '^':
            return point_t(0, -1);
        case 'v':
            return point_t(0, 1);
        case '>':
            return point_t(1, 0);
        case '<':
            return point_t(-1, 0);
        }
        abort();
    }
};

static uint64_t bit(int n) { return 1ULL << n; }

static int ctz(uint64_t v) {
    uint32_t v32 = v;
    if (v32) return __builtin_ctz(v32);
    v32 = v >> 32;
    if (v32 == 0) return 64;
    return 32 + __builtin_ctz(v32);
}

static int popcount(uint64_t v) { return __builtin_popcount(v) + __builtin_popcount(v >> 32); }

static struct {
    int start, end, extra;
    vector<uint64_t> directed, undirected;
    vector<vector<int>> weight;
} input;

static void load_input(string fn) {
    ifstream f(fn);
    string line;
    while (f >> line) {
        grid.push_back({});
        for (auto c : line) grid.back().push_back(c);
    }
    int height = int(grid.size()), width = int(grid[0].size());
    grid[0][1] = '#';
    grid[height - 1][width - 2] = '#';
    point_t start(1, 1);
    point_t end(height - 2, width - 2);
    grid[start.y][start.x] = 'P';
    grid[end.y][end.x] = 'P';
    map<point_t, int> poi;
    poi[start] = 0;
    poi[end] = 1;
    const point_t ortho[] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            if (grid[y][x] != '#') {
                int count = 0;
                point_t pos(x, y);
                for (auto& o : ortho) {
                    point_t neighbour = pos + o;
                    if (grid[neighbour.y][neighbour.x] != '#') count++;
                }
                if (count > 2) {
                    grid[y][x] = 'P';
                    poi[pos] = int(poi.size());
                }
            }
        }
    }
    struct todo_t {
        point_t position;
        int cost;
        bool forward;
    };
    queue<todo_t> todo;
    vector<uint64_t> directed = vector<uint64_t>(64, 0), undirected = vector<uint64_t>(64, 0);
    vector<vector<int>> weight = vector<vector<int>>(64, vector<int>(64, 0));
    for (auto& [start, from] : poi) {
        todo.push({start, 0, true});
        grid[start.y][start.x] = '#';
        while (!todo.empty()) {
            auto [position, cost, forward] = todo.front();
            todo.pop();
            for (auto& o : ortho) {
                auto next = position + o;
                switch (grid[next.y][next.x]) {
                case '#':
                    break;
                case 'P': {
                    auto to = poi[next];
                    if (forward)
                        directed[from] |= bit(to);
                    else
                        directed[to] |= bit(from);
                    undirected[from] |= bit(to);
                    undirected[to] |= bit(from);
                    weight[from][to] = weight[to][from] = cost + 1;
                    break;
                }
                case '.':
                    todo.push({next, cost + 1, forward});
                    grid[next.y][next.x] = '#';
                    break;
                default:
                    todo.push({next, cost + 1, forward && (o == point_t::get_dir(next))});
                    grid[next.y][next.x] = '#';
                    break;
                }
            }
        }
    }
    int start0 = ctz(undirected[0]);
    int end1 = ctz(undirected[1]);
    int extra = 2 + weight[0][start0] + weight[1][end1];
    uint64_t mask = 0;
    for (int i = 0; i < int(undirected.size()); i++)
        if (popcount(undirected[i]) < 4) mask |= bit(i);
    for (int i = 0; i < int(undirected.size()); i++)
        if (popcount(undirected[i]) < 4) undirected[i] = (undirected[i] & ~mask) | directed[i];
    input = {start0, end1, extra, directed, undirected, weight};
}

static int part1() {
    vector<int> cost(64, 0);
    queue<int> todo;
    todo.push(input.start);
    while (!todo.empty()) {
        int from = todo.front();
        todo.pop();
        uint64_t nodes = input.directed[from];
        while (nodes) {
            int to = ctz(nodes);
            uint64_t mask = bit(to);
            nodes ^= mask;
            cost[to] = max(cost[to], cost[from] + input.weight[from][to]);
            todo.push(to);
        }
    }
    return cost[input.end] + input.extra;
}

static int dfs(int from, uint64_t seen) {
    if (from == input.end) return 0;
    uint64_t nodes = input.undirected[from] & ~seen;
    int result = 0;
    while (nodes > 0) {
        int to = ctz(nodes);
        uint64_t mask = bit(to);
        nodes ^= mask;
        result = max(result, input.weight[from][to] + dfs(to, seen | mask));
    }
    return result;
}

static int part2() { return dfs(input.start, bit(input.start)) + input.extra; }

void day23(struct result& r) {
    auto start = high_resolution_clock::now();
    load_input("day23/day23.txt");
    stringstream ss;
    ss << part1();
    r.p1 = ss.str();
    ss.str("");
    ss << part2();
    r.p2 = ss.str();
    r.t = duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0;
    if (coutflag)
        cout << title << endl
             << "Part 1  - " << r.p1 << endl
             << "Part 2  - " << r.p2 << endl
             << "Elapsed - " << r.t << " ms." << endl;
    grid.clear();
}
