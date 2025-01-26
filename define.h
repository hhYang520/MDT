#pragma once
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define NDEBUG // must precede cassert to disable assert.
#include <cassert>

using namespace std;

using ui = int;
#define pb push_back

map<int, int> vertices;

vector<vector<int>> singleTime;
vector<int> deltaMaxK;

vector<int> offset;
vector<int> targets;

vector<vector<int>> adjEdges;

int tMin, tMax;

class sEdge {
 public:
  vector<int> triangles;
  vector<int> deltaTruss;
  int support;

  sEdge() {
    support = 0;
  }
};
vector<sEdge> sEdges;

class sTriangle {
 public:
  int e1, e2, e3;
  int minDelta;

  sTriangle(int a, int b, int c) {
    e1 = a;
    e2 = b;
    e3 = c;
    minDelta = -1;
  }
};
vector<sTriangle> sTriangles;

class subGraph {
 public:
  unordered_map<int, int> wEdges;
  unordered_map<int, int> triangles;
};