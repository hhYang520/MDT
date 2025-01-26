#pragma once
#include <time.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <map>
#include <vector>
#include <time.h>
#include <queue>
#include <set>
#include <list>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include <assert.h>

using namespace std;

typedef pair<int, int> sedge;
typedef tuple<int, int, int> striangle;

struct sedge_hash {
  size_t operator()(const sedge &se) const {
    return hash<int>()(se.first) ^ hash<int>()(se.second);
  }
};

struct sedge_equal {
  bool operator()(const sedge &se1, const sedge &se2) const noexcept {
    return se1.first == se2.first && se1.second == se2.second;
  }
};

struct striangle_hash {
  size_t operator()(const striangle &st) const {
    return hash<int>()(get<0>(st)) ^ hash<int>()(get<1>(st)) ^ hash<int>()(get<2>(st));
  }
};

struct striangle_equal {
  bool operator()(const striangle &st1, const striangle &st2) const noexcept {
    return get<0>(st1) == get<0>(st2) && get<1>(st1) == get<1>(st2) && get<2>(st1) == get<2>(st2);
  }
};

class TimeInfo {
 public:
  vector<int> TS;
};

class NodeInfo {
 public:
  map<int, TimeInfo> TE;
  vector<int> neighbors;
  bool isin;
};

class tgraph {
 public:
  vector<NodeInfo> nodes;
  int tmin, tmax, max_interval, edge_num, msup, maxid;

  vector<unordered_map<int, int>> vedge_set;
  vector<sedge> eid_touv;

  vector<unordered_map<int, int>> vedge_sup;
  vector<unordered_map<int, unordered_map<int, int>>> vtriangle;

  vector<int> eedge_sup;
  vector<unordered_map<int, int>> etriangle;
  vector<unordered_set<int>> etuv;

  vector<vector<int>> vskytruss;
  vector<vector<pair<int, int>>> skytruss;
  vector<unordered_map<int, int>> index_train;
  vector<set<pair<int, int>>> tdtri;


  vector<unordered_map<int, int>> vtrian_up;

};

map<pair<int, int>, set<int>> static_triangle;

int pre, pos, mid, counts;
map<pair<int, int>, int> pre_node;
map<pair<int, int>, int> pos_node;

