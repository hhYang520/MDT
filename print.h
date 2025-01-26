#pragma once
#include "define.h"
#include <fstream>
#include <iostream>

using namespace std;

void print_ans_to_file(vector<vector<sedge>> &ans, string &filename) {
  ofstream fout;
  fout.open(filename, ios::app);

  if (ans.size() == 0)
    fout << "There is no qualified community!" << endl;

  for (auto &vec: ans) {
    fout << "edge_size: " << vec.size();
    set<int> node;
    for (auto &eg: vec) {
      node.insert(eg.first);
      node.insert(eg.second);
    }
    fout << " node_size: " << node.size() << endl;
    fout << "node: ";
    for (auto &id: node)
      fout << id << " ";
    fout << endl << "edge: ";
    for (auto &eg: vec) {
      fout << "(" << eg.first << "," << eg.second << ") ";
    }
    fout << endl;
  }
  fout.close();
}

void print_ans_to_file(vector<int> &ans, string &filename, const int &q, const int &delta, const int &maxk) {
  ofstream fout;
  fout.open(filename, ios::app);
  if (!fout) {
    cerr << "result file is error!" << endl;
    return;
  }

  int nodes[vertices.size()];
  for (auto &v: vertices) {
    nodes[v.second] = v.first;
  }

  fout << "query node: " << nodes[q] << " delta: " << delta << " maxk: " << maxk << endl;

  if (ans.empty()) {
    fout << "There is no qualified community!" << endl;
    fout.close();
    return;
  }

  sort(ans.begin(), ans.end());
  set<int> ansNode;
  vector<int> sources;
  sources.resize(ans.size());
  int uid = 0, id = 0;
  while (uid < offset.size() && id < ans.size()) {
    int eid = ans[id];
    if (eid < offset[uid]) {
      sources[id] = uid;
      ansNode.insert(uid);
      ansNode.insert(targets[eid]);
      id++;
    } else {
      uid++;
    }
  }

  fout << "edge_size: " << ans.size() << ", node_size: " << ansNode.size() << endl;

  for (int i = 0; i < ans.size(); i++) {
    fout << nodes[sources[i]] << " " << nodes[targets[ans[i]]] << endl;
  }
  fout.close();
}

void print_ans_to_file(vector<int> &ans, string &filename, const int &delta, const int &maxk) {
  ofstream fout;
  fout.open(filename, ios::app);
  if (!fout) {
    cerr << "result file is error!" << endl;
    return;
  }

  int nodes[vertices.size()];
  for (auto &v: vertices) {
    nodes[v.second] = v.first;
  }

  fout << " delta: " << delta << " maxk: " << maxk << endl;

  if (ans.empty()) {
    fout << "There is no qualified community!" << endl;
    fout.close();
    return;
  }

  sort(ans.begin(), ans.end());
  set<int> ansNode;
  vector<int> sources;
  sources.resize(ans.size());
  int uid = 0, id = 0;
  while (uid < offset.size() && id < ans.size()) {
    int eid = ans[id];
    if (eid < offset[uid]) {
      sources[id] = uid;
      ansNode.insert(uid);
      ansNode.insert(targets[eid]);
      id++;
    } else {
      uid++;
    }
  }

  fout << "edge_size: " << ans.size() << ", node_size: " << ansNode.size() << endl;

  for (int i = 0; i < ans.size(); i++) {
    fout << nodes[sources[i]] << " " << nodes[targets[ans[i]]] << endl;
  }
  fout.close();
}

void print_index_to_file(const string &eFile, const string &triFile) {
  ofstream wEdge(eFile, ios::out | ios::trunc);
  ofstream wTri(triFile, ios::out | ios::trunc);
  if (!wEdge || !wTri) {
    cerr << "index file is error!" << endl;
    return;
  }

  int eid = 0;
  for (auto &e: sEdges) {
    wEdge << eid << " ";
    for (auto &kd: e.deltaTruss) {
      wEdge << kd << " ";
    }
    wEdge << endl;
    eid++;
  }
  wEdge.close();

  for (auto &k: deltaMaxK) {
    wTri << k << " ";
  }
  wTri << endl;

  int triId = 0;
  for (auto &tri: sTriangles) {
    wTri << triId << " " << tri.minDelta << endl;
    triId++;
  }
  wTri.close();
}

void print_index_to_file(const string &eFile) {
  ofstream wEdge(eFile, ios::out | ios::trunc);
  if (!wEdge) {
    cerr << "index file is error!" << endl;
    return;
  }

  int eid = 0;
  for (auto &e: sEdges) {
    wEdge << eid << " ";
    for (auto &kd: e.deltaTruss) {
      wEdge << kd << " ";
    }
    wEdge << endl;
    eid++;
  }
  wEdge.close();
}
