#pragma once
#include <iostream>
#include "types.h"

namespace Connect4 {

const u64 Bit   = 0x0000000000000001ull;
const u64 Full  = 0xFFFFFFFFFFFFFFFFull;

const u64 Top = 0b1000000'1000000'1000000'1000000'1000000'1000000'1000000ull;

INLINE u64 bit(int sq) { return Bit << static_cast<int>(sq); }
INLINE u64 lsb(u64 bb) { return bb & (0ull - bb); }
INLINE u64 rlsb(u64 bb) { return bb & (bb - Bit); }

INLINE bool only_one(u64 bb) { return bb && !rlsb(bb); }
INLINE bool several(u64 bb)  { return !!rlsb(bb); }

struct BitBoard { u64 val; }; // adapter
std::ostream & operator << (std::ostream & os, const BitBoard & bb);
extern void print64(u64 bb);

}
