#pragma once
#include "define.h"
#include "print.h"

using namespace std;

void insert_edges(const int &t,
                  subGraph &sub_graph,
                  const int &delta) {
  for (auto &eid : singleTime[t]) {
    if (sub_graph.wEdges.find(eid) == sub_graph.wEdges.end()) {
      sub_graph.wEdges[eid] = 1;

      for (int i = 0; i < sEdges[eid].triangles.size(); i += 3) {
        int tri = sEdges[eid].triangles[i];
        int e2 = sEdges[eid].triangles[i + 1], e3 = sEdges[eid].triangles[i + 2];
        if (sub_graph.wEdges.find(e2) != sub_graph.wEdges.end()
            && sub_graph.wEdges.find(e3) != sub_graph.wEdges.end()) {
          sub_graph.triangles[tri] = 0;
        }
      }
    } else {
      sub_graph.wEdges[eid]++;
    }
  }
}

void compute_support(vector<subGraph> &subGraphs, unordered_map<int, int> &allTri, const int &delta) {
  int subMax = (int) subGraphs.size();
  unordered_map<int, int> tempTri;
  unordered_map<int, int> t0Tri;
  for (auto &tri : allTri) {
    tempTri[tri.first] = tri.second;
    tri.second = 0;
  }

  for (int t = 0; t < subMax; ++t) {
    subGraph &subG = subGraphs[t];
    if (delta != 0 && subG.wEdges.empty()) continue;
    insert_edges(t + delta, subG, delta);
    for (auto &tri : subG.triangles) {
      sTriangle &sTri = sTriangles[tri.first];
      int w = subG.wEdges[sTri.e1] * subG.wEdges[sTri.e2] * subG.wEdges[sTri.e3];
      int incre = (w - tri.second);
      if (allTri.find(tri.first) == allTri.end()) {
        allTri[tri.first] = 0;
        sTriangles[tri.first].minDelta = delta;
      }
      allTri[tri.first] += incre;
      if (t == 0) t0Tri[tri.first] = incre;
      tri.second = w;
    }
  }

  for (auto &tri : allTri) {
    if (tri.second == 0) continue;
    sTriangle &sTri = sTriangles[tri.first];
    int e1 = sTri.e1, e2 = sTri.e2, e3 = sTri.e3;
    int incre = tri.second - tempTri[tri.first];
    sEdges[e1].support += incre;
    sEdges[e2].support += incre;
    sEdges[e3].support += incre;
    tri.second -= t0Tri[tri.first];
  }
}

void decompose(unordered_map<int, int> &allTri) {
  for (auto &tri : allTri) {
    sEdge &e1 = sEdges[sTriangles[tri.first].e1];
    sEdge &e2 = sEdges[sTriangles[tri.first].e2];
    sEdge &e3 = sEdges[sTriangles[tri.first].e3];
    int minSup = e1.support;
    if (minSup > e2.support) minSup = e2.support;
    if (minSup > e3.support) minSup = e3.support;

    if (e1.deltaTruss.back() < minSup) {
      e1.deltaTruss.back() = minSup;
    }
    if (e2.deltaTruss.back() < minSup) {
      e2.deltaTruss.back() = minSup;
    }
    if (e3.deltaTruss.back() < minSup) {
      e3.deltaTruss.back() = minSup;
    }
  }
}

void build_index_vec(int &interval) {

  vector<subGraph> subGraphs;
  subGraphs.resize(interval);
  deltaMaxK.resize(interval);
  unordered_map<int, int> allTri;

  for (int d = 0; d < interval; d++) {
    for (auto &e : sEdges) {
      e.deltaTruss.push_back(d);
      e.deltaTruss.push_back(0);
    }
    deltaMaxK[d] = 0;

    compute_support(subGraphs, allTri, d);

    decompose(allTri);

    for (auto &e : sEdges) {
      int last = e.deltaTruss.back();
      if (last == 0) {
        e.deltaTruss.pop_back();
        e.deltaTruss.pop_back();
        continue;
      }
      if (last > deltaMaxK[d]) deltaMaxK[d] = last;
      if (e.deltaTruss.size() > 3) {
        int secondLast = e.deltaTruss[e.deltaTruss.size() - 3];
        if (last == secondLast) {
          e.deltaTruss.pop_back();
          e.deltaTruss.pop_back();
        } else {
          e.deltaTruss.back() = last;
        }
      }
    }

    subGraphs.pop_back();
  }
}