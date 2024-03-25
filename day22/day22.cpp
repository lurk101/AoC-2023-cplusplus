auto const title = "--- Day 22: Sand Slabs ---";

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

#include "../runall.h"

using namespace std;
using namespace chrono;

struct point {
    point(int px = 0, int py = 0, int pz = 0) : x(px), y(py), z(pz) {}
    bool operator<(const point& p) const {
        if (z != p.z) return z < p.z;
        if (x != p.x) return x < p.x;
        return y < p.y;
    }
    int x, y, z;
};

struct slab {
    slab(const point& p1, const point& p2) {
        if (p1 < p2) {
            from = p1;
            to = p2;
        } else {
            from = p2;
            to = p1;
        }
    }
    void fall_by(int by) {
        from.z -= by;
        to.z -= by;
    }
    bool operator<(const slab& s) const {
        if (from < s.from) return true;
        if (s.from < from) return false;
        return to < s.to;
    }
    bool overlaps(const slab& s) const {
        return !(s.to.x < from.x || s.from.x > to.x || s.to.y < from.y || s.from.y > to.y);
    }
    point from, to;
    unordered_set<int> supported_by, supports;
};

static vector<string> split(const string& s, char delim) {
    vector<string> result;
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) result.push_back(item);
    return result;
}

static vector<slab> slabs, stable;

static int part1() {
    vector<int> ok;
    sort(slabs.begin(), slabs.end());
    for (slab s : slabs) {
        int highest = 0;
        unordered_set<int> support;
        for (int i = 0; i < stable.size(); ++i)
            if (stable[i].overlaps(s))
                if (stable[i].to.z > highest) {
                    highest = stable[i].to.z;
                    support = {i};
                } else if (stable[i].to.z == highest)
                    support.insert(i);
        int delta = s.from.z - highest - 1;
        s.fall_by(delta);
        for (int i : support) stable[i].supports.insert(stable.size());
        s.supported_by = support;
        stable.push_back(s);
        ok.push_back(1);
    }
    for (int i = 0; i < stable.size(); ++i)
        if (stable[i].supported_by.size() == 1) ok[*stable[i].supported_by.cbegin()] = 0;
    return accumulate(ok.cbegin(), ok.cend(), 0);
}

static int part2() {
    int p2 = 0;
    for (int i = 0; i < stable.size(); ++i) {
        unordered_set<int> falling = {i}, check = stable[i].supports;
        int not_ok = -1;
        while (true) {
            for (int j : check) {
                int count = 0;
                for (const auto& s : stable[j].supported_by)
                    if (falling.find(s) != falling.end()) count++;
                if (count == stable[j].supported_by.size()) {
                    not_ok = j;
                    break;
                }
            }
            if (not_ok >= 0) {
                check.erase(not_ok);
                falling.insert(not_ok);
                for (int j : stable[not_ok].supports) check.insert(j);
                not_ok = -1;
            } else
                break;
        }
        p2 += falling.size() - 1;
    }
    return p2;
}

void day22(struct result& r) {
    auto start = high_resolution_clock::now();
    ifstream f("day22/day22.txt");
    string line;
    while (getline(f, line)) {
        auto v1 = split(line, '~');
        auto v2 = split(v1[0], ',');
        auto v3 = split(v1[1], ',');
        slabs.emplace_back(point(stoi(v2[0]), stoi(v2[1]), stoi(v2[2])),
                           point(stoi(v3[0]), stoi(v3[1]), stoi(v3[2])));
    }
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
    slabs.clear();
    stable.clear();
}
