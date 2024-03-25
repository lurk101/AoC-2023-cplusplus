constexpr auto title = "--- Day 11: Cosmic Expansion ---";

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "../runall.h"

using namespace std;
using namespace chrono;

static vector<string> lines;

static int count_row(int row) {
    int count = 0;
    for (char c : lines[row])
        if (c == '#')
            count++;
    return count;
}

static int count_col(int col) {
    int count = 0;
    for (string& row : lines)
        if (row[col] == '#')
            count++;
    return count;
}

int64_t part(uint32_t delta) {
    vector<pair<int, int>> rows, columns;
    int y_inc = 0;
    for (int y = 0; y < lines.size(); y++) {
        int row_n = count_row(y);
        if (row_n == 0)
            y_inc += delta;
        else
            rows.push_back(make_pair(y + y_inc, row_n));
    }
    int x_inc = 0;
    for (int x = 0; x < lines[0].size(); x++) {
        int col_n = count_col(x);
        if (col_n == 0)
            x_inc += delta;
        else
            columns.push_back(make_pair(x + x_inc, col_n));
    }
    int64_t dist = 0;
    for (int y = 0; y < rows.size() - 1; y++)
        for (int y2 = y + 1; y2 < rows.size(); y2++)
            dist += int64_t(rows[y2].first - rows[y].first) * rows[y].second * rows[y2].second;
    for (int x = 0; x < columns.size() - 1; x++)
        for (int x2 = x + 1; x2 < columns.size(); x2++)
            dist += int64_t(columns[x2].first - columns[x].first) * columns[x].second *
                    columns[x2].second;
    return dist;
}

void day11(struct result& r) {
    auto start = high_resolution_clock::now();
    ifstream f("day11/day11.txt");
    string line;
    while (getline(f, line))
        lines.push_back(line);
    stringstream ss;
    ss << part(1);
    r.p1 = ss.str();
    ss.str("");
    ss << part(999999);
    r.p2 = ss.str();
    r.t = duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0;
    cout << title << endl
         << "Part 1  - " << r.p1 << endl
         << "Part 2  - " << r.p2 << endl
         << "Elapsed - " << r.t << " ms." << endl;
}
