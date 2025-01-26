#pragma once
#include "define.h"
#include "print.h"

using namespace std;

int get_trussness(const int &eid, const int &delta) {
  int pos = (int) sEdges[eid].deltaTruss.size() - 2;
  if (pos > delta) pos = delta * 2;
  while (pos > 0) {
    if (sEdges[eid].deltaTruss[pos] <= delta) {
      break;
    }
    pos -= 2;
  }
  return sEdges[eid].deltaTruss[pos + 1];
}

vector<int> index_search(int &q, const int &delta, const int &vis, int &maxk) {
  vector<int> ansEdges;
  vector<int> candidates;

  maxk = 0;
  for (auto &e: adjEdges[q]) {
    if (sEdges[e].deltaTruss.empty()) continue;
    int k = get_trussness(e, delta);
    if (maxk < k) {
      maxk = k;
      candidates.clear();
      candidates.push_back(e);
    } else if (maxk == k) {
      candidates.push_back(e);
    }
  }

  if (maxk == 0) return ansEdges;

  while (!candidates.empty()) {
    int eid = candidates.back();
    sEdge &e = sEdges[eid];
    candidates.pop_back();
    if (e.support == vis) continue;
    ansEdges.push_back(eid);
    e.support = vis;

    for (int i = 0; i < e.triangles.size(); i += 3) {
      int tri = e.triangles[i];
      if (sTriangles[tri].minDelta > delta) continue;
      int e1 = e.triangles[i + 1], e2 = e.triangles[i + 2];
      if (get_trussness(e1, delta) >= maxk && sEdges[e1].support != vis) {
        candidates.push_back(e1);
      }
      if (get_trussness(e2, delta) >= maxk && sEdges[e2].support != vis) {
        candidates.push_back(e2);
      }
    }
  }

  return ansEdges;
}

vector<int> index_search(const int &delta, const int &vis) {
  vector<int> candidates;

  int maxk = deltaMaxK[delta];
  for (int e = 0; e < sEdges.size(); e++) {
    if (sEdges[e].deltaTruss.empty())
      continue;
    int k = get_trussness(e, delta);
    if (k == maxk) {
      candidates.push_back(e);
    }
  }

  return candidates;
}

void split_edges(const string &str, char split) {
  istringstream iss(str);
  string token;
  int eid = -1, d = -1;
  while (getline(iss, token, split)) {
    if (eid == -1)
      eid = atoi(token.c_str());
    else if (d == -1) {
      d = atoi(token.c_str());
    } else {
      int k = atoi(token.c_str());
      sEdges[eid].deltaTruss.push_back(d);
      sEdges[eid].deltaTruss.push_back(k);
      d = -1;
    }
  }
}

void recover_index(const string &filename) {
  char split = ' ';
  ifstream infile(filename);
  assert(infile);

  string esky;
  while (getline(infile, esky)) {
    split_edges(esky, split);
  }
}

void split_maxK(const string &str, char split) {
  istringstream iss(str);
  string token;
  int i = 0;
  while (getline(iss, token, split)) {
    int maxk = atoi(token.c_str());
    deltaMaxK[i] = maxk;
    i++;
  }
}

void recover_triangle(const string &filename) {
  ifstream infile(filename);
  assert(infile);

  string str;
  getline(infile, str);
  split_maxK(str, ' ');
  int id, minDelta;
  while (infile >> id >> minDelta) {
    sTriangles[id].minDelta = minDelta;
  }
}