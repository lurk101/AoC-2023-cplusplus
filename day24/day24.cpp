const auto title = "--- Day 24: Never Tell Me The Odds ---";

#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <stdfloat>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

struct position {
    float128_t x, y, z;
    bool operator==(const position& p) const { return x == p.x && y == p.y && z == p.z; }
};

struct stone {
    position pos, vel;
    bool operator==(const stone& s) const { return pos == s.pos && vel == s.vel; }
};

static vector<stone> stones;

static int part1() {
    constexpr float128_t min_area = 200000000000000;
    constexpr float128_t max_area = 400000000000000;
    int sum = 0;
    for (int i = 0; i < stones.size() - 1; i++) {
        for (int j = i + 1; j < stones.size(); j++) {
            auto& si = stones[i];
            auto& sj = stones[j];
            float128_t det = (si.vel.x * sj.vel.y - si.vel.y * sj.vel.x);
            if (det == 0)
                continue;
            float128_t u = (sj.vel.x * (si.pos.y - sj.pos.y) + sj.vel.y * (sj.pos.x - si.pos.x)) /
                           det,
                       t = (si.vel.x * (si.pos.y - sj.pos.y) + si.vel.y * (sj.pos.x - si.pos.x)) /
                           det;
            if (u < 0 || t < 0)
                continue;
            float128_t x = sj.pos.x + sj.vel.x * t, y = sj.pos.y + sj.vel.y * t;
            if (x >= min_area && x <= max_area && y >= min_area && y <= max_area) sum++;
        }
    }
    return sum;
}

static int64_t part2() {
    int stone0_ix = 0, stone1_ix = 1;
    while (stones[stone0_ix].vel == stones[stone1_ix].vel) stone1_ix++;
    int stone2_ix = stone1_ix + 1;
    while (stones[stone0_ix].vel == stones[stone2_ix].vel ||
           stones[stone1_ix].vel == stones[stone2_ix].vel)
        stone2_ix++;
    float128_t vx0 = stones[stone0_ix].vel.x, vy0 = stones[stone0_ix].vel.y,
               vz0 = stones[stone0_ix].vel.z, vx1 = stones[stone1_ix].vel.x,
               vy1 = stones[stone1_ix].vel.y, vz1 = stones[stone1_ix].vel.z,
               vx2 = stones[stone2_ix].vel.x, vy2 = stones[stone2_ix].vel.y,
               vz2 = stones[stone2_ix].vel.z;
    float128_t abvx = vx0 - vx1, abvy = vy0 - vy1, abvz = vz0 - vz1, acvx = vx0 - vx2,
               acvy = vy0 - vy2, acvz = vz0 - vz2;
    float128_t x0 = stones[stone0_ix].pos.x, x1 = stones[stone1_ix].pos.x,
               x2 = stones[stone2_ix].pos.x, y0 = stones[stone0_ix].pos.y,
               y1 = stones[stone1_ix].pos.y, y2 = stones[stone2_ix].pos.y,
               z0 = stones[stone0_ix].pos.z, z1 = stones[stone1_ix].pos.z,
               z2 = stones[stone2_ix].pos.z;
    float128_t abx = x0 - x1, aby = y0 - y1, abz = z0 - z1, acx = x0 - x2, acy = y0 - y2,
               acz = z0 - z2;
    float128_t h0 = (y1 * vx1 - x1 * vy1) - (y0 * vx0 - x0 * vy0),
               h1 = (y2 * vx2 - x2 * vy2) - (y0 * vx0 - x0 * vy0),
               h2 = (x1 * vz1 - z1 * vx1) - (x0 * vz0 - z0 * vx0),
               h3 = (x2 * vz2 - z2 * vx2) - (x0 * vz0 - z0 * vx0),
               h4 = (z1 * vy1 - y1 * vz1) - (z0 * vy0 - y0 * vz0),
               h5 = (z2 * vy2 - y2 * vz2) - (z0 * vy0 - y0 * vz0);
    float128_t pxx = acvx * abz - abvx * acz, pyy = acvy * abx - abvy * acx,
               pzz = acvz * aby - abvz * acy, pxz = abvx * acx - acvx * abx,
               pzy = abvz * acz - acvz * abz, pyx = abvy * acy - acvy * aby;
    float128_t pxc = abvx * h3 - acvx * h2, pyc = abvy * h1 - acvy * h0,
               pzc = abvz * h5 - acvz * h4;
    float128_t pxd = acvx * abvz - abvx * acvz, pyd = acvy * abvx - abvy * acvx,
               pzd = acvz * abvy - abvz * acvy;
    float128_t qz0 = (abvy / pxd) * pxz, qx0 = (abvy / pxd) * pxx - (abvx / pyd) * pyx - aby,
               qy0 = abx - (abvx / pyd) * pyy;
    float128_t r0 = h0 - (abvy / pxd) * pxc + (abvx / pyd) * pyc;
    float128_t qy1 = (abvx / pzd) * pzy, qz1 = (abvx / pzd) * pzz - (abvz / pxd) * pxz - abx,
               qx1 = abz - (abvz / pxd) * pxx;
    float128_t r1 = h2 - (abvx / pzd) * pzc + (abvz / pxd) * pxc;
    float128_t qx2 = (abvz / pyd) * pyx, qy2 = (abvz / pyd) * pyy - (abvy / pzd) * pzy - abz,
               qz2 = aby - (abvy / pzd) * pzz;
    float128_t r2 = h4 - (abvz / pyd) * pyc + (abvy / pzd) * pzc;
    float128_t qz = ((qx1 * qy0 - qx0 * qy1) * (qx2 * r0 - qx0 * r2) -
                     (qx2 * qy0 - qx0 * qy2) * (qx1 * r0 - qx0 * r1)) /
                    ((qx2 * qy0 - qx0 * qy2) * (qx0 * qz1 - qx1 * qz0) -
                     (qx1 * qy0 - qx0 * qy1) * (qx0 * qz2 - qx2 * qz0)),
               qy =
                   ((qx0 * qz1 - qx1 * qz0) * qz + (qx1 * r0 - qx0 * r1)) / (qx1 * qy0 - qx0 * qy1),
               qx = (r0 - qy0 * qy - qz0 * qz) / qx0;
    float128_t px = (pxz * qz + pxx * qx + pxc) / pxd, py = (pyx * qx + pyy * qy + pyc) / pyd,
               pz = (pzy * qy + pzz * qz + pzc) / pzd;
    return round(px) + round(py) + round(pz);
}

int main() {
    auto start = high_resolution_clock::now();
    ifstream f("day24.txt");
    string line;
    stone s;
    char c;
    while (f >> s.pos.x) {
        f >> c >> s.pos.y >> c >> s.pos.z >> c >> s.vel.x >> c >> s.vel.y >> c >> s.vel.z;
        stones.push_back(s);
    }
    cout << title << endl
         << "part 1 - " << part1() << endl
         << "part 2 - " << part2() << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0
         << " ms." << endl;
}
