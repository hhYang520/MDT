/*
* Modified by Yang on 2025/1/6.
*/
#pragma once
#include "fstream"
#include <cassert>
#include "define.h"
#include <cstdint>
#include <map>
#include <set>
#include <sstream>
#include "temp_truss.h"

using namespace std;

void getVE(tgraph &G, int &V, string filename) {
  int u, v, t, tmin = INT32_MAX, tmax = INT32_MIN;
  ifstream fin;
  fin.open(filename);
  assert(fin);

  while (fin.peek() != EOF) {
    fin >> u >> v >> t;
    V = max(V, max(u, v));
    tmin = min(tmin, t);
    tmax = max(tmax, t);
  }
  G.tmax = tmax;
  G.tmin = tmin;

  fin.close();
}

bool Readin(string filename, tgraph &G) {
  ifstream fin;
  fin.open(filename);
  if (fin.fail()) {
    cout << filename << endl;
    return false;
  }

  int V = 0;
  getVE(G, V, filename);
  G.maxid = V;

  G.nodes.resize(V + 1);
  G.vedge_set.resize(V + 1);
  G.etuv.resize(G.tmax + 1);

  assert(fin);
  int u, v, t;
  int eid = 0;
  while (fin >> u >> v >> t) {
    if (fin.eof())
      break;
    if (u == v)
      continue;
    if (u > v)
      swap(u, v);
    sedge eg({u, v});
    if (G.nodes[u].TE.find(v) == G.nodes[u].TE.end()) {
      G.eid_touv.push_back(eg);
      G.vedge_set[u][v] = eid;
      eid++;
    }
    if (find(G.nodes[v].TE[u].TS.begin(), G.nodes[v].TE[u].TS.end(), t) ==
        G.nodes[v].TE[u].TS.end()) {
      G.nodes[v].TE[u].TS.push_back(t);
      G.nodes[u].TE[v].TS.push_back(t);
      G.etuv[t].insert(G.vedge_set[u][v]);
    }
    G.etuv[t].insert(G.vedge_set[u][v]);
  }
  G.edge_num = eid;
  G.msup = 0;

  cout << "number of vertices: " << V << endl;
  cout << "range of timestamps: [" << G.tmin << "," << G.tmax << "]" << endl;
  cout << "number of timestamps: " << G.tmax - G.tmin + 1 << endl;
  return true;
}

bool readFile(const string &filename, int &v_num, int &s_edges, int &t_edges, int &t_num) {
  fstream file;
  file.open(filename, ios::in);
  assert(file);
  if (!file.is_open()) {
    return false;
  }

  string line;
  map<int, int>::iterator it;
  tMin = INT32_MAX, tMax = INT32_MIN;
  vector<map<int, set<int>>> edges;
  while (getline(file, line)) {
    istringstream iss(line);
    std::vector<std::string> tokens;
    std::string token;

    while (iss >> token) {
      tokens.push_back(token);
    }

    int u, v, t;
    if (tokens.empty()) continue;
    try {
      u = std::stoi(tokens[0]);
      v = std::stoi(tokens[1]);
      t = std::stoi(tokens[2]);
    } catch (const std::exception &e) {
      continue;
    }

    if (u == v) continue;

    it = vertices.find(u);
    if (it == vertices.end()) {
      v_num = (int) vertices.size();
      vertices[u] = v_num;
      u = v_num;
      map<int, set<int>> emptyMap;
      edges.push_back(emptyMap);
    } else {
      u = it->second;
    }

    it = vertices.find(v);
    if (it == vertices.end()) {
      v_num = (int) vertices.size();
      vertices[v] = v_num;
      v = v_num;
      map<int, set<int>> emptyMap;
      edges.push_back(emptyMap);
    } else {
      v = it->second;
    }

    if (v < u) {
      edges[v][u].insert(t);
    } else {
      edges[u][v].insert(t);
    }

    if (t < tMin) tMin = t;
    if (t > tMax) tMax = t;
  }
  file.close();

  v_num += 1;
  offset.resize(v_num);
  t_num = tMax - tMin + 1;
  singleTime.resize(t_num);
  adjEdges.resize(v_num);
  int uid = 0;
  for (auto &eid: edges) {
    for (const auto &item: eid) {
      int vid = item.first;
      targets.push_back(vid);
      adjEdges[uid].push_back(s_edges);
      adjEdges[vid].push_back(s_edges);
      for (const auto &t: item.second) {
        singleTime[t - tMin].push_back(s_edges);
        t_edges++;
      }
      sEdge e;
      sEdges.push_back(e);
      s_edges++;
    }
    offset[uid] = s_edges;
    uid++;
  }

  return true;
}

int findAllTriangles(const int &n) {
  int nTri = 0;
  for (int u = 0; u < n; u++) {
    int vid = 0;
    if (u != 0) vid = offset[u - 1];
    for (; vid < offset[u]; vid++) {
      int v = targets[vid];
      int i = vid + 1, j = offset[v - 1];
      while (i < offset[u] && j < offset[v]) {
        if (targets[i] == targets[j]) {
          int e1 = vid, e2 = i, e3 = j;
          sEdges[e1].triangles.push_back(nTri);
          sEdges[e1].triangles.push_back(e2);
          sEdges[e1].triangles.push_back(e3);
          sEdges[e2].triangles.push_back(nTri);
          sEdges[e2].triangles.push_back(e1);
          sEdges[e2].triangles.push_back(e3);
          sEdges[e3].triangles.push_back(nTri);
          sEdges[e3].triangles.push_back(e1);
          sEdges[e3].triangles.push_back(e2);
          sTriangle tri(e1, e2, e3);
          sTriangles.push_back(tri);
          i++;
          j++;
          nTri++;
        } else if (targets[i] < targets[j]) {
          i++;
        } else {
          j++;
        }
      }
    }
  }
  return nTri;
  return nTri;
}
