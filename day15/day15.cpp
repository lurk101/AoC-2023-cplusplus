constexpr auto title = "--- Day 15: Lens Library ---";

#include <chrono>
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

static unsigned char hash_string(string instruction) {
    unsigned char result = 0;
    for (unsigned char c : instruction) result = (result + c) * 17;
    return result;
}

static unsigned int part1(vector<string>& instructions) {
    unsigned int sum = 0;
    for (auto& instruction : instructions) sum += hash_string(instruction);
    return sum;
}

static int part2(vector<string>& instructions) {
    vector<vector<pair<string, int>>> boxes(256);
    for (auto& instruction : instructions)
        if (instruction.find("-") != string::npos) {
            string label = instruction.substr(0, instruction.size() - 1);
            vector<pair<string, int>>& box = boxes[hash_string(label)];
            auto last = remove_if(box.begin(), box.end(), [&label](const pair<string, int>& p) {
                return p.first == label;
            });
            box.erase(last, box.end());
        } else {
            int found = instruction.find('=');
            string label = instruction.substr(0, found);
            int level = stoi(instruction.substr(found + 1));
            vector<pair<string, int>>& box = boxes[hash_string(label)];
            bool replaced = false;
            for (auto& b : box)
                if (b.first == label) {
                    b.second = level;
                    replaced = true;
                }
            if (!replaced) box.push_back({label, level});
        }
    int sum = 0;
    for (int i = 0; i < boxes.size(); i++)
        for (int j = 0; j < boxes[i].size(); j++) sum += (i + 1) * (j + 1) * boxes[i][j].second;
    return sum;
}

void day15(struct result& r) {
    auto start = high_resolution_clock::now();
    ifstream f("day15/day15.txt");
    string line;
    getline(f, line);
    vector<string> instructions = split(line, ',');
    stringstream ss;
    ss << part1(instructions);
    r.p1 = ss.str();
    ss.str("");
    ss << part2(instructions);
    r.p2 = ss.str();
    r.t = duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0;
    if (coutflag)
        cout << title << endl
             << "Part 1  - " << r.p1 << endl
             << "Part 2  - " << r.p2 << endl
             << "Elapsed - " << r.t << " ms." << endl;
}
