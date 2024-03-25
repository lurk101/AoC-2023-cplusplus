constexpr auto title = "--- Day 1: Trebuchet?! ---";

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "../runall.h"

using namespace std;
using namespace chrono;

static int part1(string s) {
    int first(-1), last;
    for (int i = 0; i < s.size(); ++i) {
        if (isdigit(s[i])) {
            if (first < 0) first = s[i] - '0';
            last = s[i] - '0';
        }
    }
    return first * 10 + last;
}

static int part2(string s) {
    struct number {
        string txt;
        int val;
    };
    const number numbers[] = {{"one", 1}, {"two", 2},   {"three", 3}, {"four", 4}, {"five", 5},
                              {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9}, {"1", 1},
                              {"2", 2},   {"3", 3},     {"4", 4},     {"5", 5},    {"6", 6},
                              {"7", 7},   {"8", 8},     {"9", 9}};
    int first, last;
    for (int i = 0; i < s.size(); ++i)
        for (auto& num : numbers)
            if (s.substr(i, num.txt.length()) == num.txt) {
                first = num.val;
                goto found1;
            }
found1:
    for (int i = s.size() - 1; i >= 0; --i)
        for (auto& num : numbers)
            if (s.substr(i, num.txt.length()) == num.txt) {
                last = num.val;
                goto found2;
            }
found2:
    return first * 10 + last;
}

void day01(struct result& r) {
    auto start = high_resolution_clock::now();
    ifstream f("day01/day01.txt");
    string line;
    uint32_t part1_sum = 0, part2_sum = 0;
    while (getline(f, line)) {
        part1_sum += part1(line);
        part2_sum += part2(line);
    }
    stringstream ss;
    ss << part1_sum;
    r.p1 = ss.str();
    ss.str("");
    ss << part2_sum;
    r.p2 = ss.str();
    r.t = duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0;
    if (coutflag)
        cout << title << endl
             << "Part 1  - " << r.p1 << endl
             << "Part 2  - " << r.p2 << endl
             << "Elapsed - " << r.t << " ms." << endl;
}
