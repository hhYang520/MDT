//
// Created by Yang on 2024/11/13.
//

#include "global.h"
#include "Graph_IO.h"
#include "print.h"
#include <random>

using namespace std;

int main(int argc, char *argv[]) {
  if (argc != 3) {
    cerr << "The number of parameters is error!" << endl;
    cout << "Usage: ./global file_path filename" << endl;
    return 1;
  }

  //read graph
  string filePath = argv[1], filename = argv[2];
  string file = filePath + filename;
  tgraph G;
  if (!Readin(file, G)) {
    cerr << "The dataset is not exist! Please input again!" << endl;
    return 1;
  }

  int q, delta;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> qs(1, G.maxid);
  std::uniform_int_distribution<> deltas(G.tmin, G.tmax);

  time_t s_t, e_t;
  double time_taken = 0;
  int expCount = 10;
  file = "./ans/global/" + filename;
  ofstream f;
  for (int i = 0; i < expCount; ++i) {
    q = qs(gen);
    delta = deltas(gen);
    f.open(file, ios::app);
    f << "query node: " << q << " delta: " << delta << endl;
    f.close();
    s_t = clock();
    vector<vector<sedge>> ansg = global_search(G, q, delta);
    e_t = clock();
    time_taken += (double(e_t - s_t) / double(CLOCKS_PER_SEC)) * 1000;
    print_ans_to_file(ansg, file);
  }
  cout << "GS takes time: " << time_taken << "ms" << endl;

  return 0;
}
