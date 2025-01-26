#pragma once
#include "LinearHeap.h"
#include "temp_truss.h"
#include "triangle_count.h"
#include <ctime>

unordered_set<int> scan_subgraph(tgraph &G, tgraph &Gs, vector<int> &vec, int maxnode) {
  unordered_set<int> nid_edge;

  if (Gs.nodes.empty())
    Gs.edge_num = 0;

  if (Gs.nodes.empty() || maxnode > Gs.maxid) {
    Gs.nodes.resize(maxnode + 1);
    Gs.vedge_set.resize(maxnode + 1);
    Gs.maxid = maxnode;
    //cout << "maxid: " << maxnode << endl;
  }

  int cur_eid = Gs.edge_num;

  for (auto &eid: vec) {
    int id1 = G.eid_touv[eid].first, id2 = G.eid_touv[eid].second;
    Gs.nodes[id1].TE[id2].TS = G.nodes[id1].TE[id2].TS;
    Gs.nodes[id2].TE[id1].TS = G.nodes[id2].TE[id1].TS;
    if (id1 > id2)
      swap(id1, id2);
    Gs.vedge_set[id1][id2] = cur_eid;
    sedge eg{id1, id2};
    Gs.eid_touv.emplace_back(eg);
    nid_edge.insert(cur_eid);
    cur_eid++;
  }
  Gs.edge_num = cur_eid;

  Gs.eedge_sup.resize(cur_eid, -1);
  Gs.etriangle.resize(cur_eid);
  return nid_edge;
}

vector<int> shrink_prune(tgraph &Gs, int q, int maxsup, int delta, int &ansk, int midk) {
  int q_edge = 0;
  q_edge = Gs.nodes[q].TE.size();

  vector<bool> edge_del(Gs.edge_num, false);
  ListLinearHeap heap(Gs.edge_num, maxsup);

  int *Ids = new int[Gs.edge_num];
  int *Keys = new int[Gs.edge_num];
  int i_ids = 0;

  for (int i = 0; i < Gs.edge_num; i++) {
    Ids[i_ids] = i;
    Keys[i_ids] = Gs.eedge_sup[i];
    i_ids++;

  }

  heap.init(Ids, Keys);
  vector<int> recover_edge;
  while (!heap.empty() && q_edge >= 2) {
    recover_edge.clear();
    int eid_min, sup_min;
    heap.get_min(eid_min, sup_min);
    ansk = sup_min;

    while (!heap.empty() && q_edge >= 2) {
      int eid_i, supl;
      heap.get_min(eid_i, supl);

      if (supl <= sup_min) {
        edge_del[eid_i] = true;
        heap.pop_min(eid_i, supl);
        recover_edge.push_back(eid_i);

        int id1 = Gs.eid_touv[eid_i].first, id2 = Gs.eid_touv[eid_i].second;
        if (q == id1 || q == id2)
          q_edge--;

        for (auto &
              id3_ifo: Gs.etriangle[eid_i]) {
          int id3 = id3_ifo.first, dec = id3_ifo.second;
          if (dec == 0)
            continue;

          int eid13 = id1 < id3 ? Gs.vedge_set[id1][id3] : Gs.vedge_set[id3][id1];
          int eid23 = id2 < id3 ? Gs.vedge_set[id2][id3] : Gs.vedge_set[id3][id2];

          if (!edge_del[eid13] && !edge_del[eid23]) {
            if (heap.get_key(eid13) > sup_min)
              heap.decrement(eid13, dec);
            if (heap.get_key(eid23) > sup_min)
              heap.decrement(eid23, dec);
          }
        }
      } else
        break;
    }
  }
  if (ansk >= midk && q_edge < 2) {
    while (!heap.empty()) {
      int e, s;
      heap.pop_min(e, s);
      recover_edge.emplace_back(e);
    }
  }
  delete[] Ids;
  delete[] Keys;
  return recover_edge;
}

void check_connect(tgraph &G,
                   tgraph &Gs,
                   vector<vector<sedge>> &solution,
                   vector<int> &ans,
                   int q,
                   int delta,
                   int k,
                   vector<unordered_set<int>> &non_temptri) {
  tgraph Gf;
  scan_subgraph(Gs, Gf, ans, Gs.maxid);
  vector<bool> vis(Gf.edge_num, false);
  vector<unordered_set<int>> trian_vis(Gf.edge_num);

  for (auto &v_ifo: Gf.nodes[q].TE) {
    int u = q, v = v_ifo.first;
    if (u > v)
      swap(u, v);
    int eid = Gf.vedge_set[u][v];
    if (!vis[eid]) {
      queue<int> que;
      que.push(eid);
      vis[eid] = true;
      vector<sedge> comu;
      while (!que.empty()) {
        int eid = que.front();
        que.pop();
        int x = Gf.eid_touv[eid].first, y = Gf.eid_touv[eid].second;
        sedge eg{x, y};
        comu.emplace_back(eg);

        if (Gf.nodes[x].TE.size() > Gf.nodes[y].TE.size())
          swap(x, y);
        for (auto &z_ifo: Gf.nodes[x].TE) {
          int z = z_ifo.first;
          if (x == z || y == z)
            continue;
          if (Gf.nodes[y].TE.find(z) != Gf.nodes[y].TE.end()) {
            int eidxz, eidyz;
            if (x < z && y < z) {
              if (non_temptri[G.vedge_set[min(x, y)][max(x, y)]].find(z) != non_temptri[G.vedge_set[min(x, y)][max(x,
                                                                                                                   y)]].end())            //trian_vis[eid].find(z) != trian_vis[eid].end() ||
                continue;
              eidxz = Gf.vedge_set[x][z];
              eidyz = Gf.vedge_set[y][z];
            } else if (x < y && z < y) {
              eidxz = x < z ? Gf.vedge_set[x][z] : Gf.vedge_set[z][x];
              if (non_temptri[G.vedge_set[min(x, z)][max(x, z)]].find(y)
                  != non_temptri[G.vedge_set[min(x, z)][max(x, z)]].end())
                continue;
              eidyz = Gf.vedge_set[z][y];
            } else if (z < x && y < x) {
              eidyz = y < z ? Gf.vedge_set[y][z]
                            : Gf.vedge_set[z][y];
              if (non_temptri[G.vedge_set[min(z, y)][max(z, y)]].find(x)
                  != non_temptri[G.vedge_set[min(z, y)][max(z, y)]].end())
                continue;
              eidxz = Gf.vedge_set[z][x];
            }

            if (!vis[eidxz]) {
              que.push(eidxz);
              vis[eidxz] = true;
            }

            if (!vis[eidyz]) {
              que.push(eidyz);
              vis[eidyz] = true;
            }
          }
        }
      }
      if (comu.size() >= 3)
        solution.push_back(comu);
    }
  }
}

vector<vector<sedge>> global_search(tgraph &G, int q, int delta) {
  vector<vector<sedge>> solution;
  vector<unordered_set<int>> non_temptri;

  G.eedge_sup.clear();
  G.etriangle.clear();

  G.etriangle.resize(G.edge_num);
  G.eedge_sup.resize(G.edge_num, -1);
  non_temptri.resize(G.edge_num);

  int ansk = 0, maxsup = 0;
  maxsup = count_global_sup(G, delta, non_temptri);
  vector<int> ans1 = shrink_prune(G, q, maxsup, delta, ansk, 0);
  check_connect(G, G, solution, ans1, q, delta, ansk, non_temptri);

  return solution;
}
