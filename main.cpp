#include <iostream>
#include "search.h"

using namespace std;
using namespace Connect4;

int main()
{
  cout << "Dumb MCTS-based connect4 engine\n";

  Search S;
  S.perft(9);

  return 0;
}
