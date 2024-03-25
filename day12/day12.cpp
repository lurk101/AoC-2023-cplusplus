constexpr auto title = "--- Day 12: Hot Springs ---";

#include <sched.h>
#include <unistd.h>

#include <atomic>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "../ctpl.h"

#include "../runall.h"

using namespace std;
using namespace chrono;
using namespace ctpl;

static vector<string> split(const string& s, char delim) {
    vector<string> v;
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) v.push_back(item);
    return v;
}

static vector<int> split_numbers(string& s) {
    vector<int> v;
    stringstream ss(s);
    int n;
    char c;
    while (ss >> n) {
        v.push_back(n);
        ss >> c;
    }
    return v;
}

struct key {
    int si, ci, cc, dot;
    bool operator==(const key& a) const {
        return si == a.si && ci == a.ci && cc == a.cc && dot == a.dot;
    }
    struct hash {
        size_t operator()(const key& k) const {
            return ((size_t(k.si) << 32) | k.ci) ^ ((size_t(k.cc) << 32) | k.dot);
        }
    };
};

static void solve(int id, atomic<uint64_t>* cnt, string s, vector<int> c) {
    unordered_map<key, uint64_t, key::hash> states({{{0, 0, 0, 0}, 1}}), new_states;
    uint64_t count = 0;
    while (states.size()) {
        for (auto& cs : states) {
            auto num = cs.second;
            auto si = cs.first.si, ci = cs.first.ci, cc = cs.first.cc, dot = cs.first.dot;
            if (si == s.size()) {
                if (ci == c.size()) count += num;
                continue;
            }
            if ((s[si] == '#' || s[si] == '?') && ci < c.size() && dot == 0) {
                if (s[si] == '?' && cc == 0) new_states[{si + 1, ci, 0, 0}] += num;
                cc++;
                if (cc == c[ci]) {
                    ci++;
                    cc = 0;
                    dot = 1;
                }
                new_states[{si + 1, ci, cc, dot}] += num;
            } else if ((s[si] == '.' || s[si] == '?') && cc == 0)
                new_states[{si + 1, ci, 0, 0}] += num;
        }
        states.swap(new_states);
        new_states.clear();
    }
    *cnt += count;
}

void day12(struct result& r) {
    auto start = high_resolution_clock::now();
    ifstream f("day12/day12.txt");
    cpu_set_t cpus;
    sched_getaffinity(0, sizeof(cpus), &cpus);
    thread_pool pool(CPU_COUNT(&cpus));
    atomic<uint64_t> p1 = 0, p2 = 0;
    string l;
    while (getline(f, l)) {
        auto ll = split(l, ' ');
        string s = ll[0];
        auto v = split_numbers(ll[1]);
        pool.push(solve, &p1, s, v);
        string p2s = s;
        auto p2v = v;
        for (int j = 0; j < 4; j++) {
            p2s += '?';
            p2s += s;
            p2v.insert(p2v.end(), v.begin(), v.end());
        }
        pool.push(solve, &p2, p2s, p2v);
    }
    pool.stop(true);
    stringstream ss;
    ss << p1;
    r.p1 = ss.str();
    ss.str("");
    ss << p2;
    r.p2 = ss.str();
    r.t = duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0;
    cout << title << endl
         << "Part 1  - " << r.p1 << endl
         << "Part 2  - " << r.p2 << endl
         << "Elapsed - " << r.t << " ms." << endl;
}
