#pragma once
#include <iostream>
#include "types.h"

namespace Connect4 {

const u64 Empty = 0x0000000000000000ull;
const u64 Bit   = 0x0000000000000001ull;
const u64 RankH = 0xff00000000000000ull;
const u64 File7 = 0x4040404040404040ull;
const u64 File8 = 0x8080808080808080ull;
const u64 Full  = 0xFFFFFFFFFFFFFFFFull & ~RankH & ~File7 & ~File8;

const u64 Top = 0b1000000'1000000'1000000'1000000'1000000'1000000'1000000ull;

INLINE u64 bit(int sq) { return Bit << static_cast<int>(sq); }
INLINE u64 lsb(u64 bb) { return bb & (Empty - bb); }
INLINE u64 rlsb(u64 bb) { return bb & (bb - Bit); }

INLINE bool only_one(u64 bb) { return bb && !rlsb(bb); }
INLINE bool several(u64 bb)  { return !!rlsb(bb); }

struct BitBoard { u64 val; }; // adapter
std::ostream & operator << (std::ostream & os, const BitBoard & bb);
extern void print64(u64 bb);

}
