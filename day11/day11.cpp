constexpr auto title = "--- Day 11: Cosmic Expansion ---";

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

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
    vector<int> row_pos, row_count, col_pos, col_count;
    int y_inc = 0;
    for (int y = 0; y < lines.size(); y++) {
        int row_n = count_row(y);
        if (row_n == 0)
            y_inc += delta;
        else {
            row_pos.push_back(y + y_inc);
            row_count.push_back(row_n);
        }
    }
    int x_inc = 0;
    for (int x = 0; x < lines[0].size(); x++) {
        int col_n = count_col(x);
        if (col_n == 0)
            x_inc += delta;
        else {
            col_pos.push_back(x + x_inc);
            col_count.push_back(col_n);
        }
    }
    int64_t dist = 0;
    for (int y = 0; y < row_pos.size() - 1; y++)
        for (int y2 = y + 1; y2 < row_pos.size(); y2++)
            dist += int64_t(abs(row_pos[y] - row_pos[y2])) * row_count[y] * row_count[y2];
    for (int x = 0; x < col_pos.size() - 1; x++)
        for (int x2 = x + 1; x2 < col_pos.size(); x2++)
            dist += int64_t(abs(col_pos[x] - col_pos[x2])) * col_count[x] * col_count[x2];
    return dist;
}

int main() {
    auto start = high_resolution_clock::now();
    ifstream f("day11.txt");
    string line;
    while (getline(f, line))
        lines.push_back(line);
    cout << title << endl
         << "Part 1  - " << part(1) << endl
         << "Part 2  - " << part(999999) << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0
         << " ms." << endl;
}
