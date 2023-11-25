constexpr auto title = "--- Day 18: Lavaduct Lagoon ---";

#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static vector<pair<int64_t, int64_t>> vertices;
static vector<string> lines;

static vector<string> split(const string& s, char delim) {
    vector<string> result;
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) result.push_back(item);
    return result;
}

static int64_t solve(bool part2) {
    vertices.clear();
    int dir, x = 0, y = 0, steps, i = 0;
    const string dirs = "URDL";
    for (auto& line : lines) {
        auto v = split(line, ' ');
        dir = part2 ? strtoll(v[2].substr(7, 1).c_str(), NULL, 10) : dirs.find(v[0][0]);
        steps =
            part2 ? strtoll(v[2].substr(2, 5).c_str(), NULL, 16) : strtoll(v[1].c_str(), NULL, 10);
        switch (dir) {
        case 0:
            x -= steps;
            break;
        case 1:
            y += steps;
            break;
        case 2:
            x += steps;
            break;
        case 3:
            y -= steps;
            break;
        }
        vertices.push_back(make_pair(x, y));
        i++;
    }
    vertices.push_back(make_pair(vertices[0].first, vertices[0].second));
    int64_t result = 0;
    for(; i > 0; i--) // shoelace formula
    {
        int64_t a = vertices[i].second * vertices[i - 1].first,
                b = vertices[i].first * vertices[i - 1].second,
                c = vertices[i].first - vertices[i - 1].first,
                d = vertices[i].second - vertices[i - 1].second;
        result += abs(c) + abs(d) + b - a;
    }
    return result / 2 + 1;
}

int main() {
    auto start = high_resolution_clock::now();
    ifstream f("day18.txt");
    string line;
    while (getline(f, line)) lines.push_back(line);
    cout << title << endl
         << "Part 1  - " << solve(false) << endl
         << "Part 2  - " << solve(true) << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0
         << " ms." << endl;
}
