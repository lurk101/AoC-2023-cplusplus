constexpr auto title = "--- Day 7: Camel Cards ---";

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;
using namespace chrono;

enum strengths {
    high_card,
    one_pair,
    two_pair,
    three_of_a_kind,
    full_house,
    four_of_a_kind,
    five_of_a_kind
};

struct hand {
    string h;
    uint32_t bid;
    strengths strength1, strength2;
};

static vector<hand> hands;

static bool contains(vector<int> vec, int target) {
    return find(vec.begin(), vec.end(), target) != vec.end();
}

static strengths strength_part1(string hand) {
    unordered_map<char, int> counts_map;
    for (auto c : hand) ++counts_map[c];
    vector<int> counts;
    for (auto p : counts_map) counts.push_back(p.second);
    if (contains(counts, 5))
        return five_of_a_kind;
    else if (contains(counts, 4))
        return four_of_a_kind;
    else if (contains(counts, 3)) {
        if (contains(counts, 2))
            return full_house;
        else
            return three_of_a_kind;
    } else if (count(counts.begin(), counts.end(), 2) == 2)
        return two_pair;
    else if (contains(counts, 2))
        return one_pair;
    else
        return high_card;
}

static unordered_map<char, int> strength{{'2', 1},  {'3', 2},  {'4', 3}, {'5', 4}, {'6', 5},
                                         {'7', 6},  {'8', 7},  {'9', 8}, {'T', 9}, {'J', 10},
                                         {'Q', 11}, {'K', 12}, {'A', 13}};

static bool compare_part1(const hand& a, const hand& b) {
    if (a.strength1 != b.strength1)
        return a.strength1 < b.strength1;
    for (int i = 0; i < 5; ++i)
        if (a.h[i] != b.h[i]) return strength[a.h[i]] < strength[b.h[i]];
    return false;
}

static strengths strength_part2(string hand) {
    unordered_set<char> s;
    for (auto c : hand)
        s.insert(c);
    s.insert('K');
    strengths max_strength = high_card;
    for (auto c : s) {
        string candidate = hand;
        replace(candidate.begin(), candidate.end(), 'J', c);
        max_strength = max(max_strength, strength_part1(candidate));
    }
    return max_strength;
}

static unordered_map<char, int> wildcard_strength{
    {'J', 1}, {'2', 2}, {'3', 3},  {'4', 4},  {'5', 5},  {'6', 6}, {'7', 7},
    {'8', 8}, {'9', 9}, {'T', 10}, {'Q', 11}, {'K', 12}, {'A', 13}};

static bool compare_part2(const hand& a, const hand& b) {
    if (a.strength2 != b.strength2)
        return a.strength2 < b.strength2;
    else
        for (int i = 0; i < 5; ++i)
            if (a.h[i] != b.h[i]) return wildcard_strength[a.h[i]] < wildcard_strength[b.h[i]];
    return false;
}

static uint32_t part(int part_n) {
    sort(hands.begin(), hands.end(), part_n == 1 ? compare_part1 : compare_part2);
    uint32_t sum = 0;
    for (int i = 0; i < hands.size(); ++i) sum += (i + 1) * hands[i].bid;
    return sum;
}

int main() {
    auto start = high_resolution_clock::now();
    ifstream f("day07.txt");
    string line;
    while (getline(f, line)) {
        stringstream ss(line);
        string hand;
        ss >> hand;
        string s;
        ss >> s;
        uint32_t bid = stoll(s);
        hands.push_back({hand, bid, strength_part1(hand), strength_part2(hand)});
    }
    cout << title << endl
         << "Part 1  - " << part(1) << endl
         << "Part 2  - " << part(2) << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0
         << " ms." << endl;
}
