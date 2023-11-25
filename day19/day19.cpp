constexpr auto title = "--- Day 19: Aplenty ---";

#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
using namespace chrono;

constexpr array<char, 4> rule_index{'x', 'm', 'a', 's'};

struct rule_t {
    int index, value;
    bool less;
    string target;
    bool eval(const array<int, 4>& p) const {
        return less ? p[index] < value : p[index] > value;
    }
};

struct flow_t {
    vector<rule_t> rules;
    string all_false;
    void process_rule(const string s) {
        stringstream ss(s);
        string line;
        while (getline(ss, line, ',')) {
            auto p = line.find(':');
            if (p == string::npos) {
                all_false = line;
                break;
            }
            rule_t r;
            r.target = line.substr(p + 1);
            line = line.substr(0, p);
            for (int i = 0; i < rule_index.size(); i++)
                if (rule_index[i] == line[0]) {
                    r.index = i;
                    break;
                }
            r.less = line[1] == '<';
            r.value = stoi(line.substr(2));
            rules.push_back(r);
        }
    }
    const string& next(const array<int, 4>& p) const {
        for (const auto& r : rules)
            if (r.eval(p)) return r.target;
        return all_false;
    }
};

static unordered_map<string, flow_t> flows;

static uint64_t combos(const string& rule_name, array<pair<int64_t, int64_t>, 4> vr) {
    if (rule_name == "R") return 0;
    if (rule_name == "A")
        return accumulate(vr.begin(), vr.end(), 1ll,
                          [](int64_t a, const pair<int64_t, int64_t>& r) {
                              return a * (r.second - r.first + 1);
                          });
    int64_t result = 0;
    const flow_t& w = flows.find(rule_name)->second;
    for (const rule_t& r : w.rules) {
        array<pair<int64_t, int64_t>, 4> split = vr;
        if (r.less) {
            split[r.index].second = r.value - 1;
            vr[r.index].first = r.value;
            result += combos(r.target, split);
        } else {
            split[r.index].first = r.value + 1;
            vr[r.index].second = r.value;
            result += combos(r.target, split);
        }
    }
    return result + combos(w.all_false, vr);
}

int main() {
    auto start = high_resolution_clock::now();
    ifstream f("day19.txt");
    int part1 = 0;
    string line;
    while (getline(f, line)) {
        if (line.empty()) break;
        flow_t w;
        auto p = line.find('{');
        string name = line.substr(0, p);
        w.process_rule(line.substr(p + 1, line.size() - p - 2));
        flows[name] = w;
    }
    while (getline(f, line)) {
        line = line.substr(1, line.size() - 2);
        stringstream ss(line);
        array<int, 4> p = {0, 0, 0, 0};
        for (int& i : p) {
            string s;
            getline(ss, s, ',');
            i = stoi(s.substr(2));
        }
        string name = "in";
        while (name != "A" && name != "R") name = flows.find(name)->second.next(p);
        if (name == "A")
            for (int i : p) part1 += i;
    }
    int64_t part2 = combos(
        "in", {make_pair(1, 4000), make_pair(1, 4000), make_pair(1, 4000), make_pair(1, 4000)});
    cout << title << endl
         << "Part 1  - " << part1 << endl
         << "Part 2  - " << part2 << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0
         << " ms." << endl;
}
