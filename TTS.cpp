//
// Created by Yang on 2024/11/14.
//

#include "Graph_IO.h"
#include "index_build.h"
#include "index_search.h"
#include "random"
#include <regex>

using namespace std;

void random_qMDT(const string &filename, const int &vNum, const int &interval) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> qs(0, vNum - 1);
  std::uniform_int_distribution<> deltas(0, interval);

  int q, delta;
  int expCount = 100;
  time_t s_t, e_t;
  double time_taken = 0;
  string file = "./ans/" + filename + "_qMDT";

  for (int i = 1; i <= expCount; ++i) {
    q = qs(gen);
    delta = deltas(gen);
    int maxk = 0;
    s_t = clock();
    vector<int> ansi = index_search(q, delta, i, maxk);
    e_t = clock();
    time_taken += (double(e_t - s_t) / double(CLOCKS_PER_SEC)) * 1000;

    print_ans_to_file(ansi, file, q, delta, maxk);
  }
  cout << "TTS takes time to answer random_qMDT: " << time_taken << "ms"
       << endl;
}

void random_MDT(const string &filename, const int &interval) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> deltas(0, interval);

  int expCount = 100;
  time_t s_t, e_t;
  double time_taken = 0;
  string file = "./ans/" + filename + "_MDT";

  for (int i = 0; i < expCount; ++i) {
    int delta = deltas(gen);
    s_t = clock();
    vector<int> ansi = index_search(delta, i);
    e_t = clock();
    time_taken += (double(e_t - s_t) / double(CLOCKS_PER_SEC)) * 1000;
    print_ans_to_file(ansi, file, delta, deltaMaxK[delta]);
  }
  cout << "TTS takes time to answer random_MDT: " << time_taken << "ms" << endl;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    cerr << "The number of parameters is error!" << endl;
    cout << "Usage: ./TTS file_path filename" << endl;
    return 1;
  }

  // read graph
  string filePath = argv[1], filename = argv[2];
  string file = filePath + filename;
  int vNum = 0, sEdgeNum = 0, tEdgeNum = 0, interval = 0;
  if (!readFile(file, vNum, sEdgeNum, tEdgeNum, interval)) {
    cerr << "The dataset is not exist! Please input again!" << endl;
    return 1;
  }
  int tri_num = findAllTriangles(vNum);

  regex txt_regex("\\.txt");
  filename = regex_replace(filename, txt_regex, "");

  cout << filename << " " << vNum << " " << tEdgeNum << " " << sEdgeNum << " "
       << tri_num << endl;
  cout << "The number of timestamps is " << interval << ", its range is ["
       << tMin << ", " << tMax << "]." << endl;

  string index = "./index/" + filename + "_edge.txt";
  string tri_index = "./index/" + filename + "_tri.txt";

  time_t s_t, e_t;
  double time_taken = 0;
  ifstream indexFile(index);
  if (indexFile) {
    cout << "The index has been built!" << endl;
    deltaMaxK.resize(interval);
    recover_index(index);
    recover_triangle(tri_index);
  } else {
    cout << "It is building index!" << endl;
    s_t = clock();
    build_index_vec(interval);
    e_t = clock();
    print_index_to_file(index, tri_index);
    time_taken += double(e_t - s_t) / double(CLOCKS_PER_SEC);
    cout << "TT-index building time: " << time_taken << " sec" << endl;
  }

  for (auto &e : sEdges) {
    e.support = -1;
  }

  random_qMDT(filename, vNum, interval);

  for (auto &e : sEdges) {
    e.support = -1;
  }

  random_MDT(filename, interval);

  return 0;
}