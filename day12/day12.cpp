constexpr auto title = "--- Day 12: Hot Springs ---";

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
using namespace chrono;

static vector<string> split(const string& s, char delim) {
    vector<string> v;
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) v.push_back(item);
    return v;
}

static vector<uint32_t> split_numbers(string& s) {
    vector<uint32_t> v;
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
    uint32_t si, ci, cc, dot;
    bool operator==(const key& a) const {
        return si == a.si && ci == a.ci && cc == a.cc && dot == a.dot;
    }
    struct hash {
        size_t operator()(const key& k) const {
            return ((size_t(k.si) << 32) | k.ci) ^ ((size_t(k.cc) << 32) | k.dot);
        }
    };
};

static uint64_t solve(string s, vector<uint32_t> c) {
    unordered_map<key, uint64_t, key::hash> states({{{0, 0, 0, 0}, 1}}), new_states;
    uint64_t count = 0;
    while (states.size()) {
        for (auto& cs : states) {
            auto num = cs.second;
            auto si = cs.first.si;
            auto ci = cs.first.ci;
            auto cc = cs.first.cc;
            auto dot = cs.first.dot;
            if (si == s.size()) {
                if (ci == c.size()) count += num;
                continue;
            }
            if ((s[si] == '#' || s[si] == '?') && ci < c.size() && dot == 0) {
                if (s[si] == '?' && cc == 0) new_states[{si + 1, ci, cc, dot}] += num;
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
    return count;
}

int main() {
    auto start = high_resolution_clock::now();
    ifstream f("day12.txt");
    uint64_t p1 = 0, p2 = 0;
    string l;
    while (getline(f, l)) {
        auto ll = split(l, ' ');
        string s = ll[0];
        auto v = split_numbers(ll[1]);
        p1 += solve(s, v);
        string p2s = s;
        auto p2v = v;
        for (uint32_t j = 0; j < 4; j++) {
            p2s += '?';
            p2s += s;
            p2v.insert(p2v.end(), v.begin(), v.end());
        }
        p2 += solve(p2s, p2v);
    }
    cout << title << endl
         << "Part 1  - " << p1 << endl
         << "Part 2  - " << p2 << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start)
                    .count() /
                1000.0
         << " ms." << endl;
}
