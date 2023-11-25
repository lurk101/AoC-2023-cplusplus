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
    int loss, x, y, d;
    bool operator<(const step_t& s) const { return s.loss < loss; }
};

struct visit_t {
    int x, y, d;
};

static bool in_grid(pair<int, int> pos) {
    return pos.first >= 0 && pos.first < grid[0].size() && pos.second >= 0 &&
           pos.second < grid.size();
}

static int solve(int mindist, int maxdist) {
    priority_queue<step_t> q;
    vector<vector<uint8_t>> visited(grid.size(), vector<uint8_t>(grid[0].size(), 0));
    vector<vector<vector<int>>> losses(
        grid.size(),
        vector<vector<int>>(grid[0].size(), vector<int>(4, numeric_limits<int>::max())));
    q.push({0, 0, 0, -1});
    while (!q.empty()) { // Dijkstra's algorithm
        auto [loss, x, y, d] = q.top();
        q.pop();
        if (x == grid[0].size() - 1 && y == grid.size() - 1) return loss;
        if (visited[x][y] & (1 << d)) continue;
        visited[x][y] |= (1 << d);
        for (int dir = 0; dir < 4; dir++) {
            int path_loss = 0;
            if (dir == d || (dir + 2) % 4 == d) continue;
            for (int dist = 1; dist <= maxdist; dist++) {
                int xx = x + dirs[dir].first * dist, yy = y + dirs[dir].second * dist;
                if (in_grid({xx, yy})) {
                    path_loss += grid[yy][xx];
                    if (dist < mindist) continue;
                    int nc = loss + path_loss, cc = losses[xx][yy][dir];
                    if (cc < nc) continue;
                    losses[xx][yy][dir] = nc;
                    q.push({nc, xx, yy, dir});
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

