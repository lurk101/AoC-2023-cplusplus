constexpr auto title = "--- Day 25: Snowverload ---";

#include <algorithm>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <random>
#include <string>
#include <vector>
#include <sstream>

#include "../runall.h"

using namespace std;
using namespace chrono;

void day25(struct result& r) {
    auto start = high_resolution_clock::now();
    vector<vector<int>> adj;
    int n;

    auto add_edge = [&](int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    };

    auto remove_edge = [&](int u, int v) {
        adj[u].erase(find(adj[u].begin(), adj[u].end(), v));
        adj[v].erase(find(adj[v].begin(), adj[v].end(), u));
    };

    auto count_component = [&](int v) {
        vector<bool> vis(n);
        int ans = 0;

        function<void(int)> dfs = [&](int x) {
            vis[x] = true, ans++;
            for (int u : adj[x])
                if (!vis[u]) dfs(u);
        };

        dfs(v);
        return ans;
    };

    vector<pair<int, int>> edges;

    auto read_graph = [&]() {
        n = 0;
        string u, v;
        ifstream f("day25/day25.txt");
        f >> u;
        u.pop_back();
        map<string, int> vertex_id;
        while (f >> v) {
            if (v.back() == ':') {
                v.pop_back();
                u = v;
                continue;
            }
            if (!vertex_id.count(u)) vertex_id[u] = n++;
            if (!vertex_id.count(v)) vertex_id[v] = n++;
            edges.emplace_back(vertex_id[u], vertex_id[v]);
        }
        n = vertex_id.size();
        adj.resize(n);
        for (auto [u, v] : edges) add_edge(u, v);
    };

    auto min_distances = [&]() {
        vector<vector<int>> dist(n, vector<int>(n, numeric_limits<int>::max()));
        auto bfs = [&](int start) {
            queue<int> q;
            q.push(start);
            dist[start][start] = 0;
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                for (int v : adj[u]) {
                    if (dist[start][v] != numeric_limits<int>::max()) continue;
                    dist[start][v] = dist[start][u] + 1;
                    q.push(v);
                }
            }
        };
        for (int i = 0; i < n; i++) bfs(i);
        return dist;
    };

    read_graph();

    auto dist = min_distances();

    random_device rd;
    mt19937 gen(rd());

    auto can_be_bridge = [&](int u, int v) {
        int failures = 0;
        constexpr int tries = 300, pass = 0.05 * tries;
        for (int i = 0; i < tries; i++) {
            int random_vertex = uniform_int_distribution<>(0, n - 1)(gen);
            if (abs(dist[u][random_vertex] - dist[v][random_vertex]) == 0) failures++;
        }
        return failures <= pass;
    };

    auto solution = [&]() {
        retry:
            for (int i = 0; i < edges.size(); i++) {
                auto [u1, v1] = edges[i];
                if (!can_be_bridge(u1, v1)) continue;
                remove_edge(u1, v1);
                for (int j = i + 1; j < edges.size(); j++) {
                    auto [u2, v2] = edges[j];
                    if (!can_be_bridge(u2, v2)) continue;
                    remove_edge(u2, v2);
                    for (int k = j + 1; k < edges.size(); k++) {
                        auto [u3, v3] = edges[k];
                        if (!can_be_bridge(u3, v3)) continue;
                        remove_edge(u3, v3);
                        int comp = count_component(0);
                        if (comp < n) return comp * (n - comp);
                        add_edge(u3, v3);
                    }
                    add_edge(u2, v2);
                }
                add_edge(u1, v1);
            }
            goto retry;
    };
    stringstream ss;
    ss << solution();
    r.p1 = ss.str();
    ss.str("");
    ss << "free star";
    r.p2 = ss.str();
    r.t = duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0;
    cout << title << endl
         << "Part 1  - " << r.p1 << endl
         << "Part 2  - " << r.p2 << endl
         << "Elapsed - " << r.t << " ms." << endl;
}
