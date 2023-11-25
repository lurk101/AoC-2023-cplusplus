constexpr auto title = "--- Day 2: Cube Conundrum ---";

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

struct set {
    int red, green, blue;
};

struct game {
    int id;
    vector<set> sets;
};

static vector<string> split(const string& s, char delim) {
    vector<string> result;
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) result.push_back(item);
    return result;
}

int main() {
    auto start = high_resolution_clock::now();
    ifstream f("day02.txt");
    string line;
    int part1(0), part2(0);
    while (getline(f, line)) {
        game g;
        vector<string> parts = split(line, ':');
        string t;
        stringstream ss(parts[0]);
        ss >> t >> g.id;
        parts = split(parts[1], ';');
        for (auto& s : parts) {
            set st({0, 0, 0});
            vector<string> colors = split(s, ',');
            for (auto& c : colors) {
                stringstream cl(c);
                int n;
                string color;
                cl >> n >> color;
                if (color == "red")
                    st.red = n;
                else if (color == "green")
                    st.green = n;
                else
                    st.blue = n;
            }
            g.sets.push_back(st);
        }
        const int red_max(12), green_max(13), blue_max(14);
        for (auto& s : g.sets)
            if (s.red > red_max || s.green > green_max || s.blue > blue_max) goto nope;
        part1 += g.id;
    nope:
        int red_min(0), green_min(0), blue_min(0);
        for (auto& s : g.sets) {
            if (s.red > red_min) red_min = s.red;
            if (s.green > green_min) green_min = s.green;
            if (s.blue > blue_min) blue_min = s.blue;
        }
        part2 += red_min * green_min * blue_min;
    }
    cout << title << endl
         << "Part 1  - " << part1 << endl
         << "Part 2  - " << part2 << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0
         << " ms." << endl;
}
