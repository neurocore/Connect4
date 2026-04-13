#include <span>
#include "search.h"
#include "utils.h"
#include "timer.h"

using namespace std;

namespace Connect4 {

Search::Search() : gen(random_device{}()), dist(0, 1)
{
  N = new Node[nodes_max];
}

Search::~Search()
{
  delete[] N;
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

// --------------------------------------------
//   Utilities
// --------------------------------------------

Idx Search::allocate_nodes(int cnt)
{
  Idx ptr = nodes_ptr;
  nodes_ptr += cnt;
  if (nodes_ptr > nodes_max) return Null;
  return ptr;
}

void Search::print_stats() const
{
  const Idx start = N[Root].child_start_idx;
  const Idx end = start + N[Root].children_count;

  for (Idx i = start; i < end; i++)
  {
    log("{} - {:.5f} / {}\n",
      (int)N[i].move, N[i].value / N[i].visits, N[i].visits);
  }
}

// --------------------------------------------
//   Trivial checks
// --------------------------------------------

WDL Search::get_wdl() const
{
  if (B.is_draw()) return Draw;
  if (B.is_win(1)) return Lose;
  if (B.is_win(0)) return Win;
  return NonTerminal;
}

bool Search::is_terminal(Idx node) const
{
  if (N[node].terminal) return true;

  const WDL wdl = get_wdl();

  N[node].terminal = wdl;
  return wdl;
}

bool Search::is_expanded(Idx node) const
{
  return N[node].children_count > 0;
}

float Search::get_uct(Idx node) const
{
  const int visits = N[node].visits;
  const Idx parent = N[node].parent_idx;
  assert(visits > 0);

  float parent_visits = parent ? N[parent].visits : 0.f;
  float winrate = N[node].value / N[node].visits;

  if (B.stm) winrate = -winrate;

  float uncertainty = std::log(parent_visits) / visits;
  return winrate + Utc_C * sqrt(uncertainty);
}

// --------------------------------------------
//   Working with nodes
// --------------------------------------------

Idx Search::get_child(Idx node, Move move)
{
  const Idx start = N[node].child_start_idx;
  const Idx end = start + N[node].children_count;

  for (Idx i = start; i < end; i++)
  {
    if (N[i].move == move) return i;
  }
  return Null;
}

Idx Search::expand(Idx node)
{
  Moves moves;
  B.generate(moves);
  const int size = moves.size();

  N[node].child_start_idx = allocate_nodes(size);
  N[node].children_count = size;

  if (!N[node].child_start_idx) return Null;

  const Idx start = N[node].child_start_idx;

  for (Idx i = start; i < start + size; i++)
  {
    N[i].parent_idx = node;
    N[i].move = moves[i - start];
    N[i].prior_prob = 0.f; //
    N[i].node_value = 0.f; //
  }
  B.make(N[start].move);
  //B.print();
  return start;
}

// --------------------------------------------
//   Parts of MCTS algorithm
// --------------------------------------------

Move Search::get_move()
{
  B.print();
  if (is_terminal(Root)) return None;

  auto start = Clock::now();

  while (nodes_ptr < nodes_max
  &&     elapsed(start) < 3'000)
  {
    mcts();
  }

  log("Done\n");
  Idx best = get_most_visited(Root);
  return N[best].move;
}

void Search::mcts()
{
  B = B0; // set at root
  const Idx curr = tree_policy(Root);
  if (!curr) return;

  const float reward = rollout(curr);
  backprop(curr, reward);
}

Idx Search::tree_policy(Idx node)
{
  while (!is_terminal(node))
  {
    if (!is_expanded(node)) return expand(node);
    node = get_best_child(node);
  }
  return node;
}

Idx Search::get_best_child(Idx node)
{
  int best = 0;
  float best_uct = -1e8;

  const Idx start = N[node].child_start_idx;
  const Idx end = start + N[node].children_count;

  for (int i = start; i < end; i++)
  {
    const float uct = get_uct(i);

    if (uct > best_uct)
    {
      best_uct = uct;
      best = i;
    }
  }
  B.make(N[best].move);
  //B.print();
  return best;
}

Idx Search::get_most_visited(Idx node)
{
  int best = 0;
  int best_visits = 0;

  const Idx start = N[node].child_start_idx;
  const Idx end = start + N[node].children_count;

  for (int i = start; i < end; i++)
  {
    if (N[i].visits > best_visits)
    {
      best_visits = N[i].visits;
      best = i;
    }
  }
  return best;
}

float Search::rollout(Idx node)
{
  WDL wdl;
  while ((wdl = get_wdl()) == NonTerminal)
  {
    Moves moves;
    B.generate(moves);

    int best = (int)(dist(gen) * moves.size());
    B.make(moves[best]);
  }

  return B.stm ? wdl_value[wdl] : -wdl_value[wdl];
}

void Search::backprop(Idx node, float reward)
{
  while (node)
  {
    N[node].value += reward;
    N[node].visits++;

    node = N[node].parent_idx;
    reward = -reward;
  }
}

}
