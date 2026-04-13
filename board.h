#pragma once
#include "square.h"
#include "moves.h"

namespace Connect4 {

struct Board
{
  int stm;
  int h[7];
  u64 bb[2];

  Board() { init(); }
  Board(const Board & B);
  void init();
  bool is_win(int opp = 1) const;
  bool is_draw() const;
  void print() const;

  void make(Move move);
  void unmake(Move move);
  void generate(Moves & moves) const;
};

}
