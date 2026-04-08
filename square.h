#pragma once
#include <cassert>
#include "types.h"

namespace Connect4 {

// SEE: https://github.com/denkspuren/BitboardC4/blob/master/BitboardDesign.md
// 
//  7   6 13 20 27 34 41 48   55 62     Additional row
//    +---------------------+ 
//  6 | 5 12 19 26 33 40 47 | 54 61     top row
//  5 | 4 11 18 25 32 39 46 | 53 60
//  4 | 3 10 17 24 31 38 45 | 52 59
//  3 | 2  9 16 23 30 37 44 | 51 58
//  2 | 1  8 15 22 29 36 43 | 50 57
//  1 | 0  7 14 21 28 35 42 | 49 56 63  bottom row
//    +---------------------+
//      A  B  C  D  E  F  G   H  +
//                            ^^^^^- two columns reserved
       

enum SQ : u8 // standard 7x6
{
  A1, A2, A3, A4, A5, A6, _1,
  B1, B2, B3, B4, B5, B6, _2,
  C1, C2, C3, C4, C5, C6, _3,
  D1, D2, D3, D4, D5, D6, _4,
  E1, E2, E3, E4, E5, E6, _5,
  F1, F2, F3, F4, F5, F6, _6,
  G1, G2, G3, G4, G5, G6, _7, SQ_N
};

INLINE SQ to_sq(int r, int f) { return static_cast<SQ>(r + 7 * f); }
INLINE int rank(SQ sq) { return sq % 7; }
INLINE int file(SQ sq) { return sq / 7; }

static_assert(C2 == to_sq(1, 2));
static_assert(F4 == to_sq(3, 5));

INLINE SQ operator + (SQ a, int i) { return static_cast<SQ>(+a + i); }
INLINE SQ operator - (SQ a, int i) { return static_cast<SQ>(+a - i); }

INLINE SQ & operator ++ (SQ & a) { a = static_cast<SQ>(a + 1); return a; }
INLINE SQ & operator -- (SQ & a) { a = static_cast<SQ>(a - 1); return a; }

}
