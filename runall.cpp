#include "runall.h"

#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <list>
#include <regex>
#include <string>

using namespace std;

extern void day01(struct result& r);
extern void day02(struct result& r);
extern void day03(struct result& r);
extern void day04(struct result& r);
extern void day05(struct result& r);
extern void day06(struct result& r);
extern void day07(struct result& r);
extern void day08(struct result& r);
extern void day09(struct result& r);
extern void day10(struct result& r);
extern void day11(struct result& r);
extern void day12(struct result& r);
extern void day13(struct result& r);
extern void day14(struct result& r);
extern void day15(struct result& r);
extern void day16(struct result& r);
extern void day17(struct result& r);
extern void day18(struct result& r);
extern void day19(struct result& r);
extern void day20(struct result& r);
extern void day21(struct result& r);
extern void day22(struct result& r);
extern void day23(struct result& r);
extern void day24(struct result& r);
extern void day25(struct result& r);

int coutflag;
ofstream o;
float total = 0;
int test = 0;

static void run(void (*r)(struct result&), string p1, string p2) {
    struct result res;
    coutflag = 1;
    r(res);
    float min_t = res.t;
    int retries = 10;
    coutflag = 0;
    while (retries) {
        r(res);
        if (res.t < min_t) {
            retries = 10;
            min_t = res.t;
        }
        retries--;
    }
    if (res.p1 != p1 || res.p2 != p2) {
        cerr << "Wrong answer!!!" << endl;
        exit(-1);
    }
    cout << "best = " << min_t << endl;
    o << ++test << ' ' << min_t << endl;
    total += min_t;
}

int main(int ac, char** av) {
    if (ac < 2) {
        cout << "missing run name" << endl;
        exit(-1);
    }
    o.open(av[1]);

    run(day01, "55002", "55093");
    run(day02, "2563", "70768");
    run(day03, "527144", "81463996");
    run(day04, "24733", "5422730");
    run(day05, "510109797", "9622622");
    run(day06, "840336", "41382569");
    run(day07, "246912307", "246894760");
    run(day08, "19951", "16342438708751");
    run(day09, "1877825184", "1108");
    run(day10, "6815", "269");
    run(day11, "9509330", "635832237682");
    run(day12, "7694", "5071883216318");
    run(day13, "34993", "29341");
    run(day14, "110677", "90551");
    run(day15, "498538", "286278");
    run(day16, "6514", "8089");
    run(day17, "742", "918");
    run(day18, "50746", "70086216556038");
    run(day19, "376008", "124078207789312");
    run(day20, "703315117", "230402300925361");
    run(day21, "3733", "617729401414635");
    run(day22, "430", "60558");
    run(day23, "2402", "6450");
    run(day24, "27732", "641619849766168");
    run(day25, "603368", "free star");

    o.close();
    cout << "Total " << total << endl;
}
