constexpr auto title = "--- Day 13: Point of Incidence ---";

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static vector<string> transpose(vector<string> pattern) {
    vector<string> v(pattern[0].size());
    for (int i = 0; i < pattern[0].size(); i++)
        for (int j = 0; j < pattern.size(); j++) v[i] += pattern[j][i];
    return v;
}

static int part1_reflection(const vector<string>& pattern) {
    int count = 0;
    for (int i = 0; i < pattern.size() - 1; i++) {
        int j = 0;
        int reflection{0};
        while (i - j >= 0 && i + j + 1 < pattern.size() && pattern[i - j] == pattern[i + j + 1]) {
            reflection++;
            j++;
        }
        if (i - j + 1 == 0 || i + j + 1 == pattern.size())
            count = max(count, reflection + i - j + 1);
    }
    return count;
}

static bool fix_smudge(string_view line1, string_view line2) {
    bool updated = false;
    for (int i = 0; i < line1.size(); i++)
        if (line1[i] != line2[i]) {
            if (updated)
                return false;
            else
                updated = true;
        }
    return true;
}

static int part2_reflection(const vector<string>& pattern) {
    int count = 0;
    for (int i = 0; i < pattern.size() - 1; i++) {
        int j = 0, reflection = 0;
        bool updated = false;
        while (i - j >= 0 && i + j + 1 < pattern.size()) {
            if (pattern[i - j] == pattern[i + j + 1])
                reflection++;
            else if (!updated && fix_smudge(pattern[i - j], pattern[i + j + 1])) {
                updated = true;
                reflection++;
            } else
                break;
            j++;
        }
        if ((i - j + 1 == 0 || i + j + 1 == pattern.size()) && updated)
            count = max(count, reflection + i - j + 1);
    }
    return count;
}

int main() {
    auto start = high_resolution_clock::now();
    ifstream inf("day13.txt");
    string line;
    uint32_t part1 = 0, part2 = 0;
    while (getline(inf, line)) {
        vector<string> pattern;
        while (line.size() != 0) {
            pattern.push_back(line);
            getline(inf, line);
        }
        uint32_t p = part1_reflection(pattern) * 100;
        if (p == 0) p = part1_reflection(transpose(pattern));
        part1 += p;
        p = part2_reflection(pattern) * 100;
        if (p == 0) p = part2_reflection(transpose(pattern));
        part2 += p;
    }
    cout << title << endl
         << "Part 1  - " << part1 << endl
         << "Part 2  - " << part2 << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0
         << " ms." << endl;
}
