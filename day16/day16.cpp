constexpr auto title = "--- Day 16: The Floor Will Be Lava ---";

#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <thread>
#include <vector>

using namespace std;
using namespace chrono;

enum directions { RIGHT, DOWN, LEFT, UP };

struct beam_t {
    int x, y;
    directions d;
};

static list<beam_t> beams[4];
static vector<string> grid;

static uint32_t energy(list<beam_t>& beams, beam_t start_beam) {
    const vector<uint8_t> visited_row(grid[0].size(), 0);
    vector<vector<uint8_t>> visited(grid.size(), visited_row);
    beams.push_back(start_beam);
    while (beams.size()) {
        beam_t beam = beams.front();
        beams.pop_front();
        if (!(visited[beam.y][beam.x] & (1 << beam.d))) {
            visited[beam.y][beam.x] |= 1 << beam.d;
            switch (grid[beam.y][beam.x]) {
            case '.':
                if (beam.d == RIGHT && beam.x < grid[0].size() - 1)
                    beams.push_back({beam.x + 1, beam.y, beam.d});
                else if (beam.d == LEFT && beam.x > 0)
                    beams.push_back({beam.x - 1, beam.y, beam.d});
                else if (beam.d == DOWN && beam.y < grid.size() - 1)
                    beams.push_back({beam.x, beam.y + 1, beam.d});
                else if (beam.d == UP && beam.y > 0)
                    beams.push_back({beam.x, beam.y - 1, beam.d});
                break;
            case '/':
                if (beam.d == UP && beam.x < grid[0].size() - 1)
                    beams.push_back({beam.x + 1, beam.y, RIGHT});
                else if (beam.d == DOWN && beam.x > 0)
                    beams.push_back({beam.x - 1, beam.y, LEFT});
                else if (beam.d == LEFT && beam.y < grid.size() - 1)
                    beams.push_back({beam.x, beam.y + 1, DOWN});
                else if (beam.d == RIGHT && beam.y > 0)
                    beams.push_back({beam.x, beam.y - 1, UP});
                break;
            case '\\':
                if (beam.d == DOWN && beam.x < grid[0].size() - 1)
                    beams.push_back({beam.x + 1, beam.y, RIGHT});
                else if (beam.d == UP && beam.x > 0)
                    beams.push_back({beam.x - 1, beam.y, LEFT});
                else if (beam.d == RIGHT && beam.y < grid.size() - 1)
                    beams.push_back({beam.x, beam.y + 1, DOWN});
                else if (beam.d == LEFT && beam.y > 0)
                    beams.push_back({beam.x, beam.y - 1, UP});
                break;
            case '-':
                if (beam.d == RIGHT && beam.x < grid[0].size() - 1)
                    beams.push_back({beam.x + 1, beam.y, RIGHT});
                else if (beam.d == LEFT && beam.x > 0)
                    beams.push_back({beam.x - 1, beam.y, LEFT});
                else if (beam.d == DOWN || beam.d == UP) {
                    if (beam.x < grid[0].size() - 1) beams.push_back({beam.x + 1, beam.y, RIGHT});
                    if (beam.x > 0) beams.push_back({beam.x - 1, beam.y, LEFT});
                }
                break;
            case '|':
                if (beam.d == DOWN && beam.y < grid.size() - 1)
                    beams.push_back({beam.x, beam.y + 1, DOWN});
                else if (beam.d == UP && beam.y > 0)
                    beams.push_back({beam.x, beam.y - 1, UP});
                else if (beam.d == RIGHT || beam.d == LEFT) {
                    if (beam.y < grid.size() - 1) beams.push_back({beam.x, beam.y + 1, DOWN});
                    if (beam.y > 0) beams.push_back({beam.x, beam.y - 1, UP});
                }
                break;
            }
        }
    }
    int count = 0;
    for (auto& row : visited)
        for (auto c : row)
            if (c) count++;
    return count;
}

static uint32_t sides[4];

void t(int i) {
    uint32_t p2 = 0;
    switch (i) {
    case 0:
        for (int i = 0; i < grid.size(); i++) p2 = max(energy(beams[0], {0, i, RIGHT}), p2);
        break;
    case 1:
        for (int i = 0; i < grid.size(); i++)
            p2 = max(energy(beams[1], {int(grid.size() - 1), i, LEFT}), p2);
        break;
    case 2:
        for (int i = 0; i < grid[0].size(); i++) p2 = max(energy(beams[2], {i, 0, DOWN}), p2);
        break;
    case 3:
        for (int i = 0; i < grid[0].size(); i++)
            p2 = max(energy(beams[3], {i, int(grid[0].size() - 1), UP}), p2);
        break;
    }
    sides[i] = p2;
}

int main() {
    auto start = high_resolution_clock::now();
    ifstream f("day16.txt");
    string line;
    while (getline(f, line)) grid.push_back(line);
    uint32_t part1 = energy(beams[0], {0, 0, RIGHT}), part2 = 0;
    thread* threads[4];
    for (int i = 0; i < 4; i++) threads[i] = new thread(t, i);
    for (int i = 0; i < 4; i++) {
        threads[i]->join();
        part2 = max(part2, sides[i]);
    }
    cout << title << endl
         << "Part 1  - " << part1 << endl
         << "Part 2  - " << part2 << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0
         << " ms." << endl;
}
