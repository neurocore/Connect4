#include <iostream>
#include "search.h"
#include "utils.h"

using namespace std;
using namespace Connect4;

int main()
{
  cout << "Dumb MCTS-based connect4 engine\n";

  Search S;
  //S.perft(10);
  auto move = S.get_move();

  S.print_stats();

  log("\nbest move: {}\n", move);
  log("nodes used: {}\n", S.nodes_ptr);

  return 0;
}
