constexpr auto title = "--- Day 6: Wait For It ---";

#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../runall.h"

using namespace std;
using namespace chrono;

static vector<string> split(const string& s, char delim) {
    vector<string> result;
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) result.push_back(item);
    return result;
}

static vector<int> split_numbers(string& s) {
    vector<int> result;
    stringstream ss(s);
    uint32_t n;
    while (ss >> n) result.push_back(n);
    return result;
}

static uint64_t wins(double time, double dist) {
    double c(sqrt(time * time - 4 * dist)), min_dist(floor((time - c) / 2 + 1)),
        max_dist(ceil((time + c) / 2 - 1));
    return max_dist - min_dist + 1;
}

void day06(struct result& r) {
    auto start = high_resolution_clock::now();
    ifstream f("day06/day06.txt");
    string line;
    vector<int> t1, d1;
    uint64_t t2 = 0, d2 = 0;
    getline(f, line);
    string s = split(line, ':')[1];
    t1 = split_numbers(s);
    for (auto c : s)
        if (isdigit(c)) t2 = t2 * 10 + c - '0';
    getline(f, line);
    s = split(line, ':')[1];
    d1 = split_numbers(s);
    for (auto c : s)
        if (isdigit(c)) d2 = d2 * 10 + c - '0';
    int part1 = 1;
    for (int i = 0; i < t1.size(); ++i) part1 *= wins(t1[i], d1[i]);
    int part2 = wins(t2, d2);
    stringstream ss;
    ss << part1;
    r.p1 = ss.str();
    ss.str("");
    ss << part2;
    r.p2 = ss.str();
    r.t = duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0;
    if (coutflag)
        cout << title << endl
             << "Part 1  - " << r.p1 << endl
             << "Part 2  - " << r.p2 << endl
             << "Elapsed - " << r.t << " ms." << endl;
}
