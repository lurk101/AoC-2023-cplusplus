const auto title = "--- Day 25: Snowverload ---";

#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static vector<vector<int>> adjacent;
static map<string, int> names;

static auto min_cut() {
    int n = adjacent.size();
    pair<int, vector<int>> best = {numeric_limits<int>::max(), {}};
    vector<vector<int>> co(n);
    for (int i = 0; i < n; i++) co[i] = {i};
    for (int ph = 1; ph < n; ph++) {
        vector<int> w = adjacent[0];
        size_t s = 0, t = 0;
        for (int it = 0; it < n - ph; it++) {
            w[t] = numeric_limits<int>::min();
            auto largest_elm = w.begin();
            auto largest = numeric_limits<int>::min();
            for (auto m = w.begin(); m != w.end(); m++)
                if (*m > largest) {
                    largest_elm = m;
                    largest = *m;
                }
            s = t, t = largest_elm - w.begin();
            for (int i = 0; i < n; i++) w[i] += adjacent[t][i];
        }
        best = min(best, {w[t] - adjacent[t][t], co[t]});
        co[s].insert(co[s].end(), co[t].begin(), co[t].end());
        for (int i = 0; i < n; i++) adjacent[s][i] += adjacent[t][i];
        for (int i = 0; i < n; i++) adjacent[i][s] = adjacent[s][i];
        adjacent[0][t] = numeric_limits<int>::min();
    }
    return best;
}

static int name_index(const string& s) {
    if (names.find(s) != names.cend()) return names[s];
    int index = names.size();
    names[s] = index;
    return index;
}

int main() {
    auto start = high_resolution_clock::now();
    ifstream f("day25.txt");
    vector<pair<int, int>> connections;
    string line;
    while (getline(f, line)) {
        stringstream ss(line);
        string name;
        ss >> name;
        name = name.substr(0, name.size() - 1);
        int parent = name_index(name);
        while (ss >> name) connections.emplace_back(parent, name_index(name));
    }
    adjacent = vector<vector<int>>(names.size(), vector<int>(names.size(), 0));
    for (const auto& p : connections) adjacent[p.first][p.second] = adjacent[p.second][p.first] = 1;
    auto result = min_cut();
    cout << title << endl
         << "part 1  - " << result.second.size() * (names.size() - result.second.size()) << endl
         << "part 2  - free star" << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0
         << " ms." << endl;
}
