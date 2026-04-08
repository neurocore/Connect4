#pragma once
#include "types.h"

namespace Connect4 {

struct Moves
{
  int cnt;
  Move arr[7];

  void clear() { cnt = 0; }
  void push(Move move) { arr[cnt++] = move; }
  int  size() const { return cnt; }
  Move operator[] (int i) const { return arr[i]; }
};

}
