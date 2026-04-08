#include <cassert>
#include "bitboard.h"
#include "board.h"
#include "utils.h"

namespace Connect4 {

void Board::init()
{
  stm = 0;
  bb[0] = bb[1] = 0ull;
  for (int i = 0; i < 7; i++)
    h[i] = 0;
}

bool Board::is_win(int opp)
{
  static const int dirs[] = { 1, 7, 6, 8 };
  const int side = stm ^ opp;

  for (int dir : dirs)
  {
    const u64 bb1 = bb[side];
    const u64 bb2 = bb1 & (bb1 >> dir);
    const u64 bb4 = bb2 & (bb2 >> (2 * dir));
    if (bb4) return true;
  }
  return false;
}

void Board::print()
{
  for (int r = 6; r >= 0; r--)
  {
    log("{} | ", r);
    for (int f = 0; f < 7; f++)
    {
      SQ sq = to_sq(r, f);
      if      (bb[0] & bit(sq)) log("o ");
      else if (bb[1] & bit(sq)) log("x ");
      else                      log(". ");
    }
    log("\n");
  }
  log("  +----------------\n");
  log("    A B C D E F G  \n\n");
}

void Board::make(int move)
{
  assert(h[move] < 6);

  bb[stm] |= bit(to_sq(h[move], move));
  h[move]++;
  stm ^= 1;
}

void Board::unmake(int move)
{
  stm ^= 1;
  h[move]--;
  bb[stm] &= ~bit(to_sq(h[move], move));
}

vector<int> Board::generate()
{
  vector<int> moves;
  for (int i = 0; i < 7; i++)
  {
    if (!(Top & bit(h[i])))
    {
      moves.push_back(i);
    }
  }
  return moves;
}

}
