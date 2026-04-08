#include "search.h"
#include "utils.h"
#include "timer.h"

namespace Connect4 {

Search::Search()
{
  N = new Node[1000];
  C = new Child[1000 * 100];
}

u64 Search::divide(int depth)
{
  u64 count = 0ull;

  log("-- Divide {}\n", depth);
  B.print();

  auto start = Clock::now();

  Moves moves;
  B.generate(moves);

  for (int i = 0; i < moves.size(); i++)
  {
    B.make(moves[i]);

    log("{}", (int)moves[i]);

    u64 cnt = B.is_win() ? 1 : perft_inner(depth - 1);
    count += cnt;

    log(" - {}\n", cnt);

    B.unmake(moves[i]);
  }

  i64 time = elapsed(start);
  double knps = static_cast<double>(count) / (time + 1);

  log("\nCount: {}\n", count);
  log("Time: {} ms\n", time);
  log("Speed: {:.2f} knps\n\n", knps);

  return count;
}

u64 Search::perft(int depth)
{
  log("-- Perft {}\n", depth);
  B.print();

  auto start = Clock::now();

  u64 total = 0ull;
  for (int d = 1; d <= depth; ++d)
  {
    u64 count = perft_inner(d);
    total += count;

    log("{} ", count);
  }

  i64 time = elapsed(start);
  double knps = static_cast<double>(total) / (time + 1);

  log("\n\nTotal: {}\n", total);
  log("Time: {} ms\n", time);
  log("Speed: {:.2f} knps\n\n", knps);

  return total;
}

u64 Search::perft_inner(int depth)
{
  if (depth <= 0) return 1;

  u64 count = 0ull;

  Moves moves;
  B.generate(moves);

  if (depth == 1) return moves.size();

  for (int i = 0; i < moves.size(); i++)
  {
    B.make(moves[i]);
    count += B.is_win() ? 1 : perft_inner(depth - 1);
    B.unmake(moves[i]);
  }
  return count;
}

}
