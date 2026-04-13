#include <cassert>
#include "bitboard.h"
#include "board.h"
#include "utils.h"

namespace Connect4 {

Board::Board(const Board & B)
{
  stm = B.stm;
  bb[0] = B.bb[0];
  bb[1] = B.bb[1];

  for (int i = 0; i < 7; i++)
    h[i] = B.h[i];
}

void Board::init()
{
  static const int heights[] = {0, 7, 14, 21, 28, 35, 42};

  stm = 0;
  bb[0] = bb[1] = 0ull;
  for (int i = 0; i < 7; i++)
    h[i] = heights[i];
}

bool Board::is_win(int opp) const
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

bool Board::is_draw() const
{
  return (bb[0] | bb[1]) == (Full ^ Top);
}

void Board::print() const
{
  for (int r = 6; r >= 0; r--)
  {
    log("{} | ", r + 1);
    for (int f = 0; f < 7; f++)
    {
      SQ sq = to_sq(r, f);
      if      (bb[0] & bit(sq)) log("o ");
      else if (bb[1] & bit(sq)) log("x ");
      else                      log(". ");
    }
    log("\n");
  }
  log("  +---------------\n");
  log("    A B C D E F G  \n\n");
}

void Board::make(int move)
{
  bb[stm] |= bit(h[move]++);
  stm ^= 1;
}

void Board::unmake(int move)
{
  stm ^= 1;
  bb[stm] &= ~bit(--h[move]);
}

void Board::generate(Moves & moves) const
{
  moves.clear();

  for (int i = 0; i < 7; i++)
  {
    if (!(Top & bit(h[i])))
    {
      moves.push(i);
    }
  }
}

}
