constexpr auto title = "--- Day 17: Clumsy Crucible ---";

#include <chrono>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static vector<vector<int8_t>> grid;
static const pair<int, int> dirs[4] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

struct step_t {
    int cost, y, x, d;
    bool operator<(const step_t& s) const { return s.cost < cost; }
};

static bool in_grid(pair<int, int> pos) {
    return pos.first >= 0 && pos.first < grid.size() && pos.second >= 0 &&
           pos.second < grid[0].size();
}

static int solve(int mindist, int maxdist) {
    priority_queue<step_t> q;
    vector<vector<uint8_t>> visited(grid.size(), vector<uint8_t>(grid[0].size(), 0));
    vector<vector<vector<int>>> costs(
        grid.size(),
        vector<vector<int>>(grid[0].size(), vector<int>(4, numeric_limits<int>::max())));
    q.push({0, 0, 0, -1});
    while (!q.empty()) { // Dijkstra's algorithm
        auto [cost, y, x, d] = q.top();
        q.pop();
        if (x == grid[0].size() - 1 && y == grid.size() - 1) return cost;
        if (visited[y][x] & (1 << d)) continue;
        visited[y][x] |= (1 << d);
        for (int dir = 0; dir < 4; dir++) {
            int path_cost = 0;
            if (dir == d || (dir + 2) % 4 == d) continue;
            for (int dist = 1; dist <= maxdist; dist++) {
                int xx = x + dirs[dir].first * dist, yy = y + dirs[dir].second * dist;
                if (in_grid({yy, xx})) {
                    path_cost += grid[yy][xx];
                    if (dist < mindist) continue;
                    int nc = cost + path_cost, cc = costs[yy][xx][dir];
                    if (cc < nc) continue;
                    costs[yy][xx][dir] = nc;
                    q.push({nc, yy, xx, dir});
                }
            }
        }
    }
    abort();
}

int main() {
    auto start = high_resolution_clock::now();
    ifstream f("day17.txt");
    string line;
    while (getline(f, line)) {
        grid.push_back({});
        for (auto c : line) grid.back().push_back(c - '0');
    }
    cout << title << endl
         << "Part 1 -  " << solve(1, 3) << endl
         << "Part 2 -  " << solve(4, 10) << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0
         << " ms." << endl;
}

