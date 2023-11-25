#include <chrono>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <map>

using namespace std;
using namespace chrono;

static map<int, string> titles;
static map<int, float> opi5;
static map<int, float> rock5b;
static map<int, float> pi5;
static map<int, float> pi4b;

static void load_map(string fn, map<int, float>& m) {
    ifstream f(fn);
    string line;
    while (getline(f, line)) {
        int n;
        float t;
        stringstream ss(line);
        ss >> n >> t;
        m[n] = t;
    }
    f.close();
}

int main() {
    ifstream f("titles");
    string line;
    while (getline(f, line))
        titles[atoi(line.substr(0, 2).c_str())] = line.substr(3);
    f.close();
    for (auto& t : titles)
        cout << t.first << " " << t.second << endl;
    load_map("opi5", opi5);
    load_map("rock5b", rock5b);
    load_map("pi5", pi5);
    load_map("pi4b", pi4b);
    ofstream o("readme");
    for (int i = 1; i <= 25; i++) {
        if (!titles.contains(i))
            break;
        float least = min(rock5b[i], min(opi5[i], min(pi5[i], pi4b[i])));
        o << "| " << i << " | " << titles[i] << " | ";
        if (rock5b[i] == least)
            o << "**" << rock5b[i] << " ms.** | ";
        else
            o << rock5b[i] << " ms. | ";
        if (opi5[i] == least)
            o << "**" << opi5[i] << " ms.** | ";
        else
            o << opi5[i] << " ms. | ";
        if (pi5[i] == least)
            o << "**" << pi5[i] << " ms.** | ";
        else
            o << pi5[i] << " ms. | ";
        if (pi4b[i] == least)
            o << "**" << pi4b[i] << " ms.** |" << endl;
        else
            o << pi4b[i] << " ms. |" << endl;
    }
    o.close();
}
