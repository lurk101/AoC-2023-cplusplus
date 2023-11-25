const auto title = "--- Day 24: Never Tell Me The Odds ---";

#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

struct position_t {
    double x, y, z;
    bool operator==(const position_t& a) const { return x == a.x && y == a.y && z == a.z; }
};

struct stone_t {
    position_t position, velocity;
    bool operator==(const stone_t& a) const {
        return position == a.position && velocity == a.velocity;
    }
};

static vector<stone_t> stones;

static int part1() {
    const double min_area = 200000000000000;
    const double max_area = 400000000000000;
    int sum = 0;
    for (int i = 0; i < stones.size() - 1; i++) {
        for (int j = i + 1; j < stones.size(); j++) {
            auto& si = stones[i];
            auto& sj = stones[j];
            double det = (si.velocity.x * sj.velocity.y - si.velocity.y * sj.velocity.x);
            if (det == 0)
                continue;
            double u = (sj.velocity.x * (si.position.y - sj.position.y) +
                        sj.velocity.y * (sj.position.x - si.position.x)) /
                       det;
            double t = (si.velocity.x * (si.position.y - sj.position.y) +
                        si.velocity.y * (sj.position.x - si.position.x)) /
                       det;
            if (u < 0 || t < 0)
                continue;
            double x = sj.position.x + sj.velocity.x * t;
            double y = sj.position.y + sj.velocity.y * t;
            if (x >= min_area && x <= max_area && y >= min_area && y <= max_area) sum++;
        }
    }
    return sum;
}

static int64_t part2() {
    int stone0_ix = 0;
    int stone1_ix = 1;
    while (stones[stone0_ix].velocity == stones[stone1_ix].velocity) stone1_ix++;
    int stone2_ix = stone1_ix + 1;
    while (stones[stone0_ix].velocity == stones[stone2_ix].velocity ||
           stones[stone1_ix].velocity == stones[stone2_ix].velocity)
        stone2_ix++;
    double vx0 = stones[stone0_ix].velocity.x;
    double vy0 = stones[stone0_ix].velocity.y;
    double vz0 = stones[stone0_ix].velocity.z;
    double vx1 = stones[stone1_ix].velocity.x;
    double vy1 = stones[stone1_ix].velocity.y;
    double vz1 = stones[stone1_ix].velocity.z;
    double vx2 = stones[stone2_ix].velocity.x;
    double vy2 = stones[stone2_ix].velocity.y;
    double vz2 = stones[stone2_ix].velocity.z;
    double abvx = vx0 - vx1;
    double abvy = vy0 - vy1;
    double abvz = vz0 - vz1;
    double acvx = vx0 - vx2;
    double acvy = vy0 - vy2;
    double acvz = vz0 - vz2;
    double x0 = stones[stone0_ix].position.x;
    double x1 = stones[stone1_ix].position.x;
    double x2 = stones[stone2_ix].position.x;
    double y0 = stones[stone0_ix].position.y;
    double y1 = stones[stone1_ix].position.y;
    double y2 = stones[stone2_ix].position.y;
    double z0 = stones[stone0_ix].position.z;
    double z1 = stones[stone1_ix].position.z;
    double z2 = stones[stone2_ix].position.z;
    double abx = x0 - x1;
    double aby = y0 - y1;
    double abz = z0 - z1;
    double acx = x0 - x2;
    double acy = y0 - y2;
    double acz = z0 - z2;
    double h0 = (y1 * vx1 - x1 * vy1) - (y0 * vx0 - x0 * vy0);
    double h1 = (y2 * vx2 - x2 * vy2) - (y0 * vx0 - x0 * vy0);
    double h2 = (x1 * vz1 - z1 * vx1) - (x0 * vz0 - z0 * vx0);
    double h3 = (x2 * vz2 - z2 * vx2) - (x0 * vz0 - z0 * vx0);
    double h4 = (z1 * vy1 - y1 * vz1) - (z0 * vy0 - y0 * vz0);
    double h5 = (z2 * vy2 - y2 * vz2) - (z0 * vy0 - y0 * vz0);
    double pxx = acvx * abz - abvx * acz;
    double pyy = acvy * abx - abvy * acx;
    double pzz = acvz * aby - abvz * acy;
    double pxz = abvx * acx - acvx * abx;
    double pzy = abvz * acz - acvz * abz;
    double pyx = abvy * acy - acvy * aby;
    double pxc = abvx * h3 - acvx * h2;
    double pyc = abvy * h1 - acvy * h0;
    double pzc = abvz * h5 - acvz * h4;
    double pxd = acvx * abvz - abvx * acvz;
    double pyd = acvy * abvx - abvy * acvx;
    double pzd = acvz * abvy - abvz * acvy;
    double qz0 = (abvy / pxd) * pxz;
    double qx0 = (abvy / pxd) * pxx - (abvx / pyd) * pyx - aby;
    double qy0 = abx - (abvx / pyd) * pyy;
    double r0 = h0 - (abvy / pxd) * pxc + (abvx / pyd) * pyc;
    double qy1 = (abvx / pzd) * pzy;
    double qz1 = (abvx / pzd) * pzz - (abvz / pxd) * pxz - abx;
    double qx1 = abz - (abvz / pxd) * pxx;
    double r1 = h2 - (abvx / pzd) * pzc + (abvz / pxd) * pxc;
    double qx2 = (abvz / pyd) * pyx;
    double qy2 = (abvz / pyd) * pyy - (abvy / pzd) * pzy - abz;
    double qz2 = aby - (abvy / pzd) * pzz;
    double r2 = h4 - (abvz / pyd) * pyc + (abvy / pzd) * pzc;
    double qz = ((qx1 * qy0 - qx0 * qy1) * (qx2 * r0 - qx0 * r2) -
                 (qx2 * qy0 - qx0 * qy2) * (qx1 * r0 - qx0 * r1)) /
                ((qx2 * qy0 - qx0 * qy2) * (qx0 * qz1 - qx1 * qz0) -
                 (qx1 * qy0 - qx0 * qy1) * (qx0 * qz2 - qx2 * qz0));
    double qy = ((qx0 * qz1 - qx1 * qz0) * qz + (qx1 * r0 - qx0 * r1)) / (qx1 * qy0 - qx0 * qy1);
    double qx = (r0 - qy0 * qy - qz0 * qz) / qx0;
    double px = (pxz * qz + pxx * qx + pxc) / pxd;
    double py = (pyx * qx + pyy * qy + pyc) / pyd;
    double pz = (pzy * qy + pzz * qz + pzc) / pzd;
    return round(px) + round(py) + round(pz);
}

int main() {
    auto start = high_resolution_clock::now();
    ifstream f("day24.txt");
    string line;
    stone_t s;
    char c;
    while (f >> s.position.x) {
        f >> c >> s.position.y >> c >> s.position.z;
        f >> c >> s.velocity.x >> c >> s.velocity.y >> c >> s.velocity.z;
        stones.push_back(s);
    }
    cout << title << endl
         << "part 1 - " << part1() << endl
         << "part 2 - " << part2() << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0
         << " ms." << endl;
}
