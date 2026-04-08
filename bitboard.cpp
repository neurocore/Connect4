#include "bitboard.h"
#include "square.h"

using namespace std;

namespace Connect4 {

ostream & operator << (ostream & os, const BitBoard & bb)
{
#ifdef _DEBUG
  for (int y = 0; y < 8; ++y)
  {
    os << "  ";
    for (int x = 0; x < 8; ++x)
    {
      SQ sq = to_sq(y, x);
      char ch = (bb.val & bit(sq)) ? 'o' : '.';
      os << ch << ' ';
    }
    os << endl;
  }
  os << endl;
#endif
  return os;
}

void print64(u64 bb)
{
#ifdef _DEBUG
  cout << BitBoard{bb} << endl;
#endif
}

}
