#pragma once
#include "board.h"

namespace Connect4 {

// Tips:
//
// 1. Put children in P(s, a)-decreasing order for better pickup in PUCT
// 2. Use separate FIFO-container for nodes recycling

struct alignas(64) Node
{
  u32 visits;
  float value;

  float prior_prob; // P(s, a)
  float node_value; // V(s) from NN

  u32 parent_idx;
  u32 child_start_idx; // indexing in separate
  u32 children_count;  //  table of children

  u64 hash_key; // for nodes recycling in TT

  u8 is_terminal;
  u32 children_expanded;
};

struct Child
{
  u32 is_open;
  u32 node_idx;
};

struct Search
{
  Board B;
  Node * N; // Actual records
  Child * C; // References to child nodes

  Search();
  u64 divide(int depth);
  u64 perft(int depth);
  u64 perft_inner(int depth);
};

}
