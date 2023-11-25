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
    uint32_t k[4];
    bool operator==(const key& a) const {
        return (k[0] == a.k[0]) && (k[1] == a.k[1]) && (k[2] == a.k[2]) && (k[3] == a.k[3]);
    }
    struct hash_key {
        size_t operator()(const key& k) const {
            return ((size_t(k.k[0]) << 32) + k.k[1]) ^ ((size_t(k.k[2]) << 32) + k.k[3]);
        }
    };
};

static uint64_t solve(string s, vector<uint32_t> c) {
    unordered_map<key, uint64_t, key::hash_key> states({{{0, 0, 0, 0}, 1}}), nstates;
    uint64_t count = 0;
    while (states.size()) {
        uint32_t si, ci, cc, dot;
        uint64_t num;
        for (auto& cs : states) {
            num = cs.second;
            si = cs.first.k[0];
            ci = cs.first.k[1];
            cc = cs.first.k[2];
            dot = cs.first.k[3];
            if (si == s.size()) {
                if (ci == c.size()) count += num;
                continue;
            }
            if ((s[si] == '#' || s[si] == '?') && ci < c.size() && dot == 0) {
                if (s[si] == '?' && cc == 0) nstates[{si + 1, ci, cc, dot}] += num;
                cc++;
                if (cc == c[ci]) {
                    ci++;
                    cc = 0;
                    dot = 1;
                }
                nstates[{si + 1, ci, cc, dot}] += num;
            } else if ((s[si] == '.' || s[si] == '?') && cc == 0)
                nstates[{si + 1, ci, 0, 0}] += num;
        }
        states.swap(nstates);
        nstates.clear();
    }
    return count;
}

int main() {
    auto start = high_resolution_clock::now();
    ifstream f("day12.txt");
    vector<string> lines;
    string line;
    while (getline(f, line))
        lines.push_back(line);
    uint64_t p1 = 0, p2 = 0;
    for (auto& l : lines) {
        auto ll = split(l, ' ');
        string s = ll[0];
        auto v = split_numbers(ll[1]);
        p1 += solve(s, v);
        string p2s = s;
        vector<uint32_t> p2v = v;
        for (uint32_t j = 0; j < 4; j++) {
            p2s += '?';
            p2s += s;
            for (int j = 0; j < v.size(); j++) p2v.push_back(v[j]);
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
