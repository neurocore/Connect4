#pragma once
#include <random>
#include "board.h"

namespace Connect4 {

using Idx = u32;
enum : u32 { Null, Root, Empty };
enum WDL : u8 { NonTerminal, Win, Draw, Lose };

const float wdl_value[] = { 0.f, 1.f, 0.f, -1.f };
const float Utc_C = 1.41;

// Tips:
//
// 1. Put children in P(s, a)-decreasing order for better pickup in PUCT
// 2. Use separate FIFO-container for nodes recycling

struct alignas(64) Node
{
  u32 visits = 5;
  float value = 0.f;

  float prior_prob = 0.f; // P(s, a)
  float node_value = 0.f; // V(s) from NN

  Idx parent_idx = 0;
  Idx child_start_idx = 0; // indexing in separate
  Idx children_count = 0;  //  table of children

  u64 hash_key = 0ull; // for nodes recycling in TT

  Move move = None;
  WDL terminal = NonTerminal;
};

struct Search
{
  Board B, B0;
  Node * N;

  Idx nodes_ptr = Empty;
  Idx nodes_max = 100'000;

  std::minstd_rand gen;
  std::uniform_real_distribution<double> dist;

  Search();
  ~Search();
  u64 divide(int depth);
  u64 perft(int depth);
  u64 perft_inner(int depth);

  Idx allocate_nodes(int cnt);
  void print_stats() const;

  WDL get_wdl() const;

  bool is_terminal(Idx node) const;
  bool is_expanded(Idx node) const;
  float get_uct(Idx node) const;

  Idx get_child(Idx node, Move move);
  Idx expand(Idx node);

  Move get_move();
  void mcts();
  Idx tree_policy(Idx node);
  Idx get_best_child(Idx node);
  Idx get_most_visited(Idx node);
  float rollout(Idx node);
  void backprop(Idx node, float reward);
};

}
