#define title "--- Day 07:  ---"
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;
using namespace chrono;

int main() {
    auto start = high_resolution_clock::now();
    ifstream f("day07.txt");
    string line;
    while (getline(f, line)) {
    }
    cout << title << endl
         << "Part 1 - " << 0 << endl
         << "Part 2 - " << 0 << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0
         << " ms." << endl;
}
