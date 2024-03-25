constexpr auto title = "--- Day 10: Pipe Maze ---";

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../runall.h"

using namespace std;
using namespace chrono;

void day10(struct result& r) {
    auto strt = high_resolution_clock::now();
    enum directions { d_down, d_right, d_up, d_left };
    ifstream f("day10/day10.txt");
    string line;
    uint32_t start_x, start_y, x, y = 0;
    vector<string> S, X;
    while (getline(f, line)) {
        string s;
        x = 0;
        for (auto c : line) {
            if (c == 'S') {
                start_x = x;
                start_y = y;
            }
            x++;
            s += '.';
        }
        y++;
        S.push_back(line);
        X.push_back(s);
    }
    uint32_t part1 = 0, part2 = 0, steps = 0;
    directions direction = d_down;
    x = start_x;
    y = start_y;
    X[y][x] = 'S';
    if (x < S[0].size() - 1 && (S[y][x + 1] == '-' || S[y][x + 1] == 'J' || S[y][x + 1] == '7'))
        direction = d_right;
    else if (y > 0 && (S[y - 1][x] == '|' || S[y - 1][x] == '7' || S[y - 1][x] == 'F'))
        direction = d_up;
    else if (x > 0 && (S[y][x - 1] == '-' || S[y][x - 1] == 'F' || S[y][x - 1] == 'L'))
        direction = d_left;
    char c = '.';
    while (c != 'S') {
        c = '.';
        switch (direction) {
        case d_down:
            c = S[y + 1][x];
            if (c == 'J')
                direction = d_left;
            else if (c == 'L')
                direction = d_right;
            y++;
            break;
        case d_right:
            c = S[y][x + 1];
            if (c == 'J')
                direction = d_up;
            else if (c == '7')
                direction = d_down;
            x++;
            break;
        case d_up:
            c = S[y - 1][x];
            if (c == 'F')
                direction = d_right;
            else if (c == '7')
                direction = d_left;
            y--;
            break;
        case d_left:
            c = S[y][x - 1];
            if (c == 'F')
                direction = d_down;
            else if (c == 'L')
                direction = d_up;
            x--;
            break;
        }
        X[y][x] = c;
        steps++;
    }
    part1 = steps / 2;
    for (uint32_t i = 0; i < X.size(); i++) {
        bool inside = false;
        char online = '.';
        for (uint32_t j = 0; j < X[0].size(); j++) {
            char c = X[i][j];
            if (c == '|' || c == 'J' || c == 'L' || c == 'F' || c == '7') {
                switch (c) {
                case '|':
                    inside = !inside;
                    break;
                case 'F':
                    online = 'F';
                    break;
                case 'L':
                    online = 'L';
                    break;
                case '7':
                    if (online == 'L') inside = !inside;
                    break;
                case 'J':
                    if (online == 'F') inside = !inside;
                    break;
                }
            } else if (c == '.')
                if (inside) part2++;
        }
    }
    stringstream ss;
    ss << part1;
    r.p1 = ss.str();
    ss.str("");
    ss << part2;
    r.p2 = ss.str();
    r.t = duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1000.0;
    if (coutflag)
        cout << title << endl
             << "Part 1  - " << r.p1 << endl
             << "Part 2  - " << r.p2 << endl
             << "Elapsed - " << r.t << " ms." << endl;
}
