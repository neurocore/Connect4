#pragma once
#include "square.h"
#include "moves.h"

namespace Connect4 {

struct Board
{
  int stm, i;
  int h[7];
  u64 bb[2];

  Board() { init(); }
  void init();
  bool is_win(int opp = 1);
  void print();

  void make(Move move);
  void unmake(Move move);
  void generate(Moves & moves);
};

}
