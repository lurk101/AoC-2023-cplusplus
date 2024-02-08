#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <list>
#include <regex>
#include <string>

#include <unistd.h>

using namespace std;
using namespace filesystem;

string hostname;

float run_once(bool display) {
    FILE* fp = popen("./a.out", "r");
    char out[256];
    float elapsed;
    while (fgets(out, sizeof(out), fp) != NULL) {
        if (!strncmp(out, "Elapsed - ", 10)) elapsed = strtof(out + 10, NULL);
        if (display) cout << out;
    }
    pclose(fp);
    return elapsed;
}

float run_many(void) {
    const int limit = 20;
    int repeats;
    float best;
    for (best = run_once(true), repeats = limit; repeats; --repeats) {
        float e = run_once(false);
        if (e < best) {
            best = e;
            repeats = limit;
        }
    }
    return best;
}

list<float> results;

void run_test(string& path) {
    cout << endl << "Compiling " << path << endl;
    copy_file(path + "/" + path + ".cpp", path + ".cpp", copy_options::overwrite_existing);
    copy_file(path + "/" + path + ".txt", path + ".txt", copy_options::overwrite_existing);
    stringstream ss;
    string tune;
    if (hostname == "opi5" || hostname == "rock5b" || hostname == "pi5")
        tune = "-mtune=cortex-a76";
    else if (hostname == "pi4b")
        tune = "-mtune=cortex-a72";
    ss << "g++ -O3 -std=c++23 " << tune << " " << path << ".cpp -lpthread";
    cout << ss.str() << endl;
    system(ss.str().c_str());
    cout << "Running " << path << endl;
    results.push_back(run_many());
    cout << "Best : " << results.back() << endl;
    system(("rm " + path + ".txt " + path + ".cpp a.out").c_str());
}

int main(int ac, char** av) {
    if (ac < 2) {
        cout << "missing run name" << endl;
        exit(-1);
    }
    char hname[32];
    gethostname(hname, sizeof(hname));
    hostname = hname;

    list<string> paths;
    const string path = ".";
    regex e("./day[0-2][0-9]$");
    if (ac > 2) e = string("./day") + av[2] + '$';
    smatch m;
    for (const auto& entry : directory_iterator(path)) {
        string p(entry.path());
        if (regex_search(p, m, e)) paths.push_back(p.substr(2));
    }
    paths.sort();
    cout << "Running:";
    for (auto& p : paths) cout << ' ' << p;
    cout << endl;
    for (auto& p : paths) run_test(p);
    ofstream o(av[1]);
    int i = 0;
    float total = 0;
    while (results.size()) {
        o << ++i << ' ' << results.front() << endl;
        total += results.front();
        results.pop_front();
    }
    o.close();
    cout << "Total " << total << endl;
}
