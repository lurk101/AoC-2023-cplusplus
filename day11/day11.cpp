constexpr auto title = "--- Day 11: Cosmic Expansion ---";

#include <chrono>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static vector<string> lines;
static vector<bool> empty_columns;

static bool empty_row(string row) {
    for (char c : row)
        if (c != '.') return false;
    return true;
}

static bool empty_col(vector<string>& lines, int col) {
    for (string& row : lines)
        if (row[col] != '.') return false;
    return true;
}

static int manhattan(pair<int, int>& a, pair<int, int>& b) {
    return abs(a.first - b.first) + abs(a.second - b.second);
}

int64_t part(uint32_t delta) {
    vector<pair<int, int>> galaxy;
    uint32_t y_inc = 0;
    for (int y = 0; y < lines.size(); y++) {
        if (empty_row(lines[y])) y_inc += delta;
        int x_inc = 0;
        for (int x = 0; x < lines[0].size(); x++) {
            if (empty_columns[x]) x_inc += delta;
            if (lines[y][x] == '#') galaxy.push_back(make_pair(y + y_inc, x + x_inc));
        }
    }
    int64_t sum = 0;
    for (int i = 0; i < galaxy.size() - 1; i++)
        for (int j = i + 1; j < galaxy.size(); j++) sum += manhattan(galaxy[i], galaxy[j]);
    return sum;
}

int main() {
    auto start = high_resolution_clock::now();
    ifstream f("day11.txt");
    string line;
    while (getline(f, line)) lines.push_back(line);
    for (int x = 0; x < lines[0].size(); x++)
        empty_columns.push_back(empty_col(lines, x));
    cout << title << endl
         << "Part 1  - " << part(1) << endl
         << "Part 2  - " << part(999999) << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0
         << " ms." << endl;
}
