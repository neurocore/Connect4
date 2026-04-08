#pragma once
#include <vector>
#include "square.h"
#include "types.h"

using std::vector;

namespace Connect4 {

struct Board
{
  int stm;
  int h[7];
  u64 bb[2];

  Board() { init(); }
  void init();
  bool is_win(int opp = 1);
  void print();

  void make(int move);
  void unmake(int move);
  vector<int> generate();
};

}
