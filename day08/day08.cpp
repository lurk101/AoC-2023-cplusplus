constexpr auto title = "--- Day 8: Haunted Wasteland ---";

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

struct direction {
    string left, right;
};

static unordered_map<string, direction> wasteland;
static string directions;

static uint32_t part1(void) {
    uint32_t count = 0, d_index = 0;
    string loc = "AAA";
    while (loc != "ZZZ") {
        count++;
        loc = directions[d_index] == 'L' ? wasteland[loc].left : wasteland[loc].right;
        d_index = (d_index == directions.size() - 1) ? 0 : d_index + 1;
    }
    return count;
}

static vector<int64_t> counts;
static vector<string> locs;

static int64_t part2(void) {
    vector<string> locs;
    for (auto& w : wasteland)
        if (w.first[2] == 'A') locs.push_back(w.first);
    vector<uint32_t> counts;
    for (auto loc : locs) {
        uint32_t count = 0, d_index = 0;
        while (loc[2] != 'Z') {
            count++;
            loc = directions[d_index] == 'L' ? wasteland[loc].left : wasteland[loc].right;
            d_index = (d_index == directions.size() - 1) ? 0 : d_index + 1;
        }
        counts.push_back(count);
    }
    int64_t l = 1;
    for (auto c : counts) l = lcm(l, c);
    return l;
}

int main() {
    auto start = high_resolution_clock::now();
    ifstream f("day08.txt");
    string line;
    getline(f, directions);
    getline(f, line);
    while (getline(f, line)) wasteland[line.substr(0, 3)] = {line.substr(7, 3), line.substr(12, 3)};
    cout << title << endl
         << "Part 1  - " << part1() << endl
         << "Part 2  - " << part2() << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0
         << " ms." << endl;
}
