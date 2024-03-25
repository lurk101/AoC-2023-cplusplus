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

#include "runall.h"

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

static void run(void (*r)(struct result&), string p1, string p2) {
    struct result res;
    r(res);
    if (res.p1 != p1 || res.p2 != p2) {
        cerr << "Wrong answer!!!" << endl;
        exit(-1);
    }
}

int main(int ac, char** av) {
    run(day01, "55002", "55093");
    run(day02, "2563", "70768");
    run(day03, "527144", "81463996");
    run(day04, "", "");
    run(day05, "", "");
    run(day06, "", "");
    run(day07, "", "");
    run(day08, "", "");
    run(day09, "", "");
    run(day10, "", "");
    run(day11, "", "");
    run(day12, "", "");
    run(day13, "", "");
    run(day14, "", "");
    run(day15, "", "");
    run(day16, "", "");
    run(day17, "", "");
    run(day18, "", "");
    run(day19, "", "");
    run(day20, "", "");
    run(day21, "", "");
    run(day22, "", "");
    run(day23, "", "");
    run(day24, "", "");
    run(day25, "", "");
}
