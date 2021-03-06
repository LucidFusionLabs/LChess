/*
 * $Id: magic.h 1172 2014-05-11 18:16:55Z justin $
 * Copyright (C) 2009 Lucid Fusion Labs

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LFL_CHESS_MAGIC_H__
#define LFL_CHESS_MAGIC_H__
namespace LFL {
namespace Chess {

static int rook_magic_number_bits[] = {
  12, 11, 11, 11, 11, 11, 11, 12,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  12, 11, 11, 11, 11, 11, 11, 12
};

static int bishop_magic_number_bits[] = {
  6, 5, 5, 5, 5, 5, 5, 6,
  5, 5, 5, 5, 5, 5, 5, 5,
  5, 5, 7, 7, 7, 7, 5, 5,
  5, 5, 7, 9, 9, 7, 5, 5,
  5, 5, 7, 9, 9, 7, 5, 5,
  5, 5, 7, 7, 7, 7, 5, 5,
  5, 5, 5, 5, 5, 5, 5, 5,
  6, 5, 5, 5, 5, 5, 5, 6
};

static BitBoard rook_magic_number[] = {
  0xa180022080400230LL, 0x40100040022000LL,   0x80088020001002LL,   0x80080280841000LL,   0x4200042010460008LL, 0x4800a0003040080LL,  0x400110082041008LL, 0x8000a041000880LL,
  0x10138001a080c010LL, 0x804008200480LL,     0x10011012000c0LL,    0x22004128102200LL,   0x200081201200cLL,    0x202a001048460004LL, 0x81000100420004LL,  0x4000800380004500LL,
  0x208002904001LL,     0x90004040026008LL,   0x208808010002001LL,  0x2002020020704940LL, 0x8048010008110005LL, 0x6820808004002200LL, 0xa80040008023011LL, 0xb1460000811044LL,
  0x4204400080008ea0LL, 0xb002400180200184LL, 0x2020200080100380LL, 0x10080080100080LL,   0x2204080080800400LL, 0xa40080360080LL,     0x2040604002810b1LL, 0x8c218600004104LL,
  0x8180004000402000LL, 0x488c402000401001LL, 0x4018a00080801004LL, 0x1230002105001008LL, 0x8904800800800400LL, 0x42000c42003810LL,   0x8408110400b012LL,  0x18086182000401LL,
  0x2240088020c28000LL, 0x1001201040c004LL,   0xa02008010420020LL,  0x10003009010060LL,   0x4008008008014LL,    0x80020004008080LL,   0x282020001008080LL, 0x50000181204a0004LL,
  0x102042111804200LL,  0x40002010004001c0LL, 0x19220045508200LL,   0x20030010060a900LL,  0x8018028040080LL,    0x88240002008080LL,   0x10301802830400LL,  0x332a4081140200LL,
  0x8080010a601241LL,   0x1008010400021LL,    0x4082001007241LL,    0x211009001200509LL,  0x8015001002441801LL, 0x801000804000603LL,  0xc0900220024a401LL, 0x1000200608243LL
};

static BitBoard bishop_magic_number[] = {
  0x2910054208004104LL, 0x2100630a7020180LL,  0x5822022042000000LL, 0x2ca804a100200020LL, 0x204042200000900LL,  0x2002121024000002LL, 0x80404104202000e8LL, 0x812a020205010840LL,
  0x8005181184080048LL, 0x1001c20208010101LL, 0x1001080204002100LL, 0x1810080489021800LL, 0x62040420010a00LL,   0x5028043004300020LL, 0xc0080a4402605002LL, 0x8a00a0104220200LL,
  0x940000410821212LL,  0x1808024a280210LL,   0x40c0422080a0598LL,  0x4228020082004050LL, 0x200800400e00100LL,  0x20b001230021040LL,  0x90a0201900c00LL,    0x4940120a0a0108LL,
  0x20208050a42180LL,   0x1004804b280200LL,   0x2048020024040010LL, 0x102c04004010200LL,  0x20408204c002010LL,  0x2411100020080c1LL,  0x102a008084042100LL, 0x941030000a09846LL,
  0x244100800400200LL,  0x4000901010080696LL, 0x280404180020LL,     0x800042008240100LL,  0x220008400088020LL,  0x4020182000904c9LL,  0x23010400020600LL,   0x41040020110302LL,
  0x412101004020818LL,  0x8022080a09404208LL, 0x1401210240484800LL, 0x22244208010080LL,   0x1105040104000210LL, 0x2040088800c40081LL, 0x8184810252000400LL, 0x4004610041002200LL,
  0x40201a444400810LL,  0x4611010802020008LL, 0x80000b0401040402LL, 0x20004821880a00LL,   0x8200002022440100LL, 0x9431801010068LL,    0x1040c20806108040LL, 0x804901403022a40LL,
  0x2400202602104000LL, 0x208520209440204LL,  0x40c000022013020LL,  0x2000104000420600LL, 0x400000260142410LL,  0x800633408100500LL,  0x2404080a1410LL,     0x138200122002900LL    
};

static BitBoard rook_occupancy_mask[] = {
  0x101010101017eLL,    0x202020202027cLL,    0x404040404047aLL,    0x8080808080876LL,    0x1010101010106eLL,   0x2020202020205eLL,   0x4040404040403eLL,   0x8080808080807eLL,
  0x1010101017e00LL,    0x2020202027c00LL,    0x4040404047a00LL,    0x8080808087600LL,    0x10101010106e00LL,   0x20202020205e00LL,   0x40404040403e00LL,   0x80808080807e00LL,
  0x10101017e0100LL,    0x20202027c0200LL,    0x40404047a0400LL,    0x8080808760800LL,    0x101010106e1000LL,   0x202020205e2000LL,   0x404040403e4000LL,   0x808080807e8000LL,
  0x101017e010100LL,    0x202027c020200LL,    0x404047a040400LL,    0x8080876080800LL,    0x1010106e101000LL,   0x2020205e202000LL,   0x4040403e404000LL,   0x8080807e808000LL,
  0x1017e01010100LL,    0x2027c02020200LL,    0x4047a04040400LL,    0x8087608080800LL,    0x10106e10101000LL,   0x20205e20202000LL,   0x40403e40404000LL,   0x80807e80808000LL,
  0x17e0101010100LL,    0x27c0202020200LL,    0x47a0404040400LL,    0x8760808080800LL,    0x106e1010101000LL,   0x205e2020202000LL,   0x403e4040404000LL,   0x807e8080808000LL,
  0x7e010101010100LL,   0x7c020202020200LL,   0x7a040404040400LL,   0x76080808080800LL,   0x6e101010101000LL,   0x5e202020202000LL,   0x3e404040404000LL,   0x7e808080808000LL,
  0x7e01010101010100LL, 0x7c02020202020200LL, 0x7a04040404040400LL, 0x7608080808080800LL, 0x6e10101010101000LL, 0x5e20202020202000LL, 0x3e40404040404000LL, 0x7e80808080808000LL 
};

static BitBoard bishop_occupancy_mask[] = {
  0x40201008040200LL, 0x402010080400LL,   0x4020100a00LL,     0x40221400LL,       0x2442800LL,        0x204085000LL,      0x20408102000LL,    0x2040810204000LL,
  0x20100804020000LL, 0x40201008040000LL, 0x4020100a0000LL,   0x4022140000LL,     0x244280000LL,      0x20408500000LL,    0x2040810200000LL,  0x4081020400000LL,
  0x10080402000200LL, 0x20100804000400LL, 0x4020100a000a00LL, 0x402214001400LL,   0x24428002800LL,    0x2040850005000LL,  0x4081020002000LL,  0x8102040004000LL,
  0x8040200020400LL,  0x10080400040800LL, 0x20100a000a1000LL, 0x40221400142200LL, 0x2442800284400LL,  0x4085000500800LL,  0x8102000201000LL,  0x10204000402000LL,
  0x4020002040800LL,  0x8040004081000LL,  0x100a000a102000LL, 0x22140014224000LL, 0x44280028440200LL, 0x8500050080400LL,  0x10200020100800LL, 0x20400040201000LL,
  0x2000204081000LL,  0x4000408102000LL,  0xa000a10204000LL,  0x14001422400000LL, 0x28002844020000LL, 0x50005008040200LL, 0x20002010080400LL, 0x40004020100800LL,
  0x20408102000LL,    0x40810204000LL,    0xa1020400000LL,    0x142240000000LL,   0x284402000000LL,   0x500804020000LL,   0x201008040200LL,   0x402010080400LL,
  0x2040810204000LL,  0x4081020400000LL,  0xa102040000000LL,  0x14224000000000LL, 0x28440200000000LL, 0x50080402000000LL, 0x20100804020000LL, 0x40201008040200LL     
};

static BitBoard knight_occupancy_mask[] = {
  0x20400LL,           0x50800LL,           0xa1100LL,            0x142200LL,           0x284400LL,           0x508800LL,           0xa01000LL,           0x402000LL,
  0x2040004LL,         0x5080008LL,         0xa110011LL,          0x14220022LL,         0x28440044LL,         0x50880088LL,         0xa0100010LL,         0x40200020LL,
  0x204000402LL,       0x508000805LL,       0xa1100110aLL,        0x1422002214LL,       0x2844004428LL,       0x5088008850LL,       0xa0100010a0LL,       0x4020002040LL,
  0x20400040200LL,     0x50800080500LL,     0xa1100110a00LL,      0x142200221400LL,     0x284400442800LL,     0x508800885000LL,     0xa0100010a000LL,     0x402000204000LL,
  0x2040004020000LL,   0x5080008050000LL,   0xa1100110a0000LL,    0x14220022140000LL,   0x28440044280000LL,   0x50880088500000LL,   0xa0100010a00000LL,   0x40200020400000LL,
  0x204000402000000LL, 0x508000805000000LL, 0xa1100110a000000LL,  0x1422002214000000LL, 0x2844004428000000LL, 0x5088008850000000LL, 0xa0100010a0000000LL, 0x4020002040000000LL,
  0x400040200000000LL, 0x800080500000000LL, 0x1100110a00000000LL, 0x2200221400000000LL, 0x4400442800000000LL, 0x8800885000000000LL, 0x100010a000000000LL, 0x2000204000000000LL,
  0x4020000000000LL,   0x8050000000000LL,   0x110a0000000000LL,   0x22140000000000LL,   0x44280000000000LL,   0x88500000000000LL,   0x10a00000000000LL,   0x20400000000000LL
};

static BitBoard king_occupancy_mask[] = {
  0x302LL,             0x705LL,             0xe0aLL,             0x1c14LL,             0x3828LL,             0x7050LL,             0xe0a0LL,             0xc040LL,
  0x30203LL,           0x70507LL,           0xe0a0eLL,           0x1c141cLL,           0x382838LL,           0x705070LL,           0xe0a0e0LL,           0xc040c0LL,
  0x3020300LL,         0x7050700LL,         0xe0a0e00LL,         0x1c141c00LL,         0x38283800LL,         0x70507000LL,         0xe0a0e000LL,         0xc040c000LL,
  0x302030000LL,       0x705070000LL,       0xe0a0e0000LL,       0x1c141c0000LL,       0x3828380000LL,       0x7050700000LL,       0xe0a0e00000LL,       0xc040c00000LL,
  0x30203000000LL,     0x70507000000LL,     0xe0a0e000000LL,     0x1c141c000000LL,     0x382838000000LL,     0x705070000000LL,     0xe0a0e0000000LL,     0xc040c0000000LL,
  0x3020300000000LL,   0x7050700000000LL,   0xe0a0e00000000LL,   0x1c141c00000000LL,   0x38283800000000LL,   0x70507000000000LL,   0xe0a0e000000000LL,   0xc040c000000000LL,
  0x302030000000000LL, 0x705070000000000LL, 0xe0a0e0000000000LL, 0x1c141c0000000000LL, 0x3828380000000000LL, 0x7050700000000000LL, 0xe0a0e00000000000LL, 0xc040c00000000000LL,
  0x203000000000000LL, 0x507000000000000LL, 0xa0e000000000000LL, 0x141c000000000000LL, 0x2838000000000000LL, 0x5070000000000000LL, 0xa0e0000000000000LL, 0x40c0000000000000LL,
};

static BitBoard white_pawn_occupancy_mask[] = {
  0x100LL,             0x200LL,             0x400LL,             0x800LL,             0x1000LL,             0x2000LL,             0x4000LL,             0x8000LL,
  0x1010000LL,         0x2020000LL,         0x4040000LL,         0x8080000LL,         0x10100000LL,         0x20200000LL,         0x40400000LL,         0x80800000LL,
  0x1000000LL,         0x2000000LL,         0x4000000LL,         0x8000000LL,         0x10000000LL,         0x20000000LL,         0x40000000LL,         0x80000000LL,
  0x100000000LL,       0x200000000LL,       0x400000000LL,       0x800000000LL,       0x1000000000LL,       0x2000000000LL,       0x4000000000LL,       0x8000000000LL,
  0x10000000000LL,     0x20000000000LL,     0x40000000000LL,     0x80000000000LL,     0x100000000000LL,     0x200000000000LL,     0x400000000000LL,     0x800000000000LL,
  0x1000000000000LL,   0x2000000000000LL,   0x4000000000000LL,   0x8000000000000LL,   0x10000000000000LL,   0x20000000000000LL,   0x40000000000000LL,   0x80000000000000LL,
  0x100000000000000LL, 0x200000000000000LL, 0x400000000000000LL, 0x800000000000000LL, 0x1000000000000000LL, 0x2000000000000000LL, 0x4000000000000000LL, 0x8000000000000000LL,
  0x0LL,               0x0LL,               0x0LL,               0x0LL,               0x0LL,                0x0LL,                0x0LL,                0x0LL,
};

static BitBoard white_pawn_attack_mask[] = {
  0x200LL,             0x500LL,             0xa00LL,             0x1400LL,             0x2800LL,             0x5000LL,             0xa000LL,             0x4000LL,
  0x20000LL,           0x50000LL,           0xa0000LL,           0x140000LL,           0x280000LL,           0x500000LL,           0xa00000LL,           0x400000LL,
  0x2000000LL,         0x5000000LL,         0xa000000LL,         0x14000000LL,         0x28000000LL,         0x50000000LL,         0xa0000000LL,         0x40000000LL,
  0x200000000LL,       0x500000000LL,       0xa00000000LL,       0x1400000000LL,       0x2800000000LL,       0x5000000000LL,       0xa000000000LL,       0x4000000000LL,
  0x20000000000LL,     0x50000000000LL,     0xa0000000000LL,     0x140000000000LL,     0x280000000000LL,     0x500000000000LL,     0xa00000000000LL,     0x400000000000LL,
  0x2000000000000LL,   0x5000000000000LL,   0xa000000000000LL,   0x14000000000000LL,   0x28000000000000LL,   0x50000000000000LL,   0xa0000000000000LL,   0x40000000000000LL,
  0x200000000000000LL, 0x500000000000000LL, 0xa00000000000000LL, 0x1400000000000000LL, 0x2800000000000000LL, 0x5000000000000000LL, 0xa000000000000000LL, 0x4000000000000000LL,
  0x0LL,               0x0LL,               0x0LL,               0x0LL,                0x0LL,                0x0LL,                0x0LL,                0x0LL 
};

static BitBoard black_pawn_occupancy_mask[] = {
  0x0LL,             0x0LL,             0x0LL,             0x0LL,             0x0LL,              0x0LL,              0x0LL,              0x0LL,
  0x1LL,             0x2LL,             0x4LL,             0x8LL,             0x10LL,             0x20LL,             0x40LL,             0x80LL,
  0x100LL,           0x200LL,           0x400LL,           0x800LL,           0x1000LL,           0x2000LL,           0x4000LL,           0x8000LL,
  0x10000LL,         0x20000LL,         0x40000LL,         0x80000LL,         0x100000LL,         0x200000LL,         0x400000LL,         0x800000LL,
  0x1000000LL,       0x2000000LL,       0x4000000LL,       0x8000000LL,       0x10000000LL,       0x20000000LL,       0x40000000LL,       0x80000000LL,
  0x100000000LL,     0x200000000LL,     0x400000000LL,     0x800000000LL,     0x1000000000LL,     0x2000000000LL,     0x4000000000LL,     0x8000000000LL,
  0x10100000000LL,   0x20200000000LL,   0x40400000000LL,   0x80800000000LL,   0x101000000000LL,   0x202000000000LL,   0x404000000000LL,   0x808000000000LL,
  0x1000000000000LL, 0x2000000000000LL, 0x4000000000000LL, 0x8000000000000LL, 0x10000000000000LL, 0x20000000000000LL, 0x40000000000000LL, 0x80000000000000LL
};

static BitBoard black_pawn_attack_mask[] = {
  0x0LL,             0x0LL,             0x0LL,             0x0LL,              0x0LL,              0x0LL,              0x0LL,              0x0LL,
  0x2LL,             0x5LL,             0xaLL,             0x14LL,             0x28LL,             0x50LL,             0xa0LL,             0x40LL,
  0x200LL,           0x500LL,           0xa00LL,           0x1400LL,           0x2800LL,           0x5000LL,           0xa000LL,           0x4000LL,
  0x20000LL,         0x50000LL,         0xa0000LL,         0x140000LL,         0x280000LL,         0x500000LL,         0xa00000LL,         0x400000LL,
  0x2000000LL,       0x5000000LL,       0xa000000LL,       0x14000000LL,       0x28000000LL,       0x50000000LL,       0xa0000000LL,       0x40000000LL,
  0x200000000LL,     0x500000000LL,     0xa00000000LL,     0x1400000000LL,     0x2800000000LL,     0x5000000000LL,     0xa000000000LL,     0x4000000000LL,
  0x20000000000LL,   0x50000000000LL,   0xa0000000000LL,   0x140000000000LL,   0x280000000000LL,   0x500000000000LL,   0xa00000000000LL,   0x400000000000LL,
  0x2000000000000LL, 0x5000000000000LL, 0xa000000000000LL, 0x14000000000000LL, 0x28000000000000LL, 0x50000000000000LL, 0xa0000000000000LL, 0x40000000000000LL
};

struct MagicMoves {
  vector<BitBoard> rook_magic_moves[64], bishop_magic_moves[64];
  MagicMoves() {
    for (int i=0; i<64; i++) {
      vector<BitBoard> occupancy_variation;
      GetRookOccupancyVariations(i, &occupancy_variation);
      SetupRookMagicMoves(i, occupancy_variation, &rook_magic_moves[i]);
    }
    for (int i=0; i<64; i++) {
      vector<BitBoard> occupancy_variation;
      GetBishopOccupancyVariations(i, &occupancy_variation);
      SetupBishopMagicMoves(i, occupancy_variation, &bishop_magic_moves[i]);
    }
  }

  BitBoard RookMoves(int p, BitBoard blockers, BitBoard friendly) {
    CHECK_RANGE(p, 0, 64);
    int magic_index = MagicHash(p, blockers, rook_magic_number, rook_magic_number_bits);
    CHECK_RANGE(magic_index, 0, rook_magic_moves[p].size());
    return rook_magic_moves[p][magic_index] & ~friendly;
  }

  BitBoard BishopMoves(int p, BitBoard blockers, BitBoard friendly) {
    CHECK_RANGE(p, 0, 64);
    int magic_index = MagicHash(p, blockers, bishop_magic_number, bishop_magic_number_bits);
    CHECK_RANGE(magic_index, 0, bishop_magic_moves[p].size());
    return bishop_magic_moves[p][magic_index] & ~friendly;
  }

  static int MagicHash(BitBoard occupancy, BitBoard magic_number, int magic_number_bits) {
    return int((occupancy * magic_number) >> (64 - magic_number_bits));
  } 

  static int MagicHash(int p, BitBoard occupancy, const BitBoard *magic_number, const int *magic_number_bits) {
    return MagicHash(occupancy, magic_number[p], magic_number_bits[p]);
  }

  static void GetRookOccupancyVariations(int p, vector<BitBoard> *occupancy_variation, vector<BitBoard> *attack_set=0) {
    BitBoard mask = rook_occupancy_mask[p];
    int bit_count=Bit::Count(mask), variation_count=1<<bit_count, mask_bit_indices[65], i_bit_indices[65], j;
    Bit::Indices(mask, mask_bit_indices);

    for (int i=0; i<variation_count; i++) {
      BitBoard occupancy=0, attack=0;
      Bit::Indices(i, i_bit_indices);
      for (j=0; i_bit_indices[j] != -1; j++) occupancy |= (1LL << mask_bit_indices[i_bit_indices[j]]);

      for (j=p+8; j<=55                    && (occupancy & (1LL<<j)) == 0; j+=8) {}; if (j>=0 && j<=63) attack |= (1LL<<j);
      for (j=p-8; j>=8                     && (occupancy & (1LL<<j)) == 0; j-=8) {}; if (j>=0 && j<=63) attack |= (1LL<<j);
      for (j=p+1; j%8!=7 && j%8!=0         && (occupancy & (1LL<<j)) == 0; j++)  {}; if (j>=0 && j<=63) attack |= (1LL<<j);
      for (j=p-1; j%8!=7 && j%8!=0 && j>=0 && (occupancy & (1LL<<j)) == 0; j--)  {}; if (j>=0 && j<=63) attack |= (1LL<<j);

      occupancy_variation->push_back(occupancy);
      if (attack_set) attack_set->push_back(attack);
    }
  }

  static void GetBishopOccupancyVariations(int p, vector<BitBoard> *occupancy_variation, vector<BitBoard> *attack_set=0) {
    BitBoard mask = bishop_occupancy_mask[p];
    int bit_count=Bit::Count(mask), variation_count=1<<bit_count, mask_bit_indices[65], i_bit_indices[65], j;
    Bit::Indices(mask, mask_bit_indices);

    for (int i=0; i<variation_count; i++) {
      BitBoard occupancy=0, attack=0;
      Bit::Indices(i, i_bit_indices);
      for (j=0; i_bit_indices[j] != -1; j++) occupancy |= (1LL << mask_bit_indices[i_bit_indices[j]]);

      for (j=p+9; j%8!=7 && j%8!=0 && j<=55 && (occupancy & (1LL<<j)) == 0; j+=9) {}; if (j>=0 && j<=63) attack |= (1LL<<j);
      for (j=p-9; j%8!=7 && j%8!=0 && j>= 8 && (occupancy & (1LL<<j)) == 0; j-=9) {}; if (j>=0 && j<=63) attack |= (1LL<<j);
      for (j=p+7; j%8!=7 && j%8!=0 && j<=55 && (occupancy & (1LL<<j)) == 0; j+=7) {}; if (j>=0 && j<=63) attack |= (1LL<<j);
      for (j=p-7; j%8!=7 && j%8!=0 && j>= 8 && (occupancy & (1LL<<j)) == 0; j-=7) {}; if (j>=0 && j<=63) attack |= (1LL<<j);

      occupancy_variation->push_back(occupancy);
      if (attack_set) attack_set->push_back(attack);
    }
  }

  static void SetupRookMagicMoves(int p, const vector<BitBoard> &occupancy_variation, vector<BitBoard> *magic_moves) {
    magic_moves->resize(1 << rook_magic_number_bits[p]);
    for (int i=0, j; i<occupancy_variation.size(); i++) {
      int magic_index = MagicHash(p, occupancy_variation[i], rook_magic_number, rook_magic_number_bits);
      BitBoard valid_moves = 0;

      for (j=p+8; j<=63;         j+=8) { valid_moves |= (1LL<<j); if ((occupancy_variation[i] & (1LL<<j)) != 0) break; }
      for (j=p-8; j>= 0;         j-=8) { valid_moves |= (1LL<<j); if ((occupancy_variation[i] & (1LL<<j)) != 0) break; }
      for (j=p+1; j%8!=0;         j++) { valid_moves |= (1LL<<j); if ((occupancy_variation[i] & (1LL<<j)) != 0) break; }
      for (j=p-1; j%8!=7 && j>=0; j--) { valid_moves |= (1LL<<j); if ((occupancy_variation[i] & (1LL<<j)) != 0) break; }

      CHECK_RANGE(magic_index, 0, magic_moves->size());
      (*magic_moves)[magic_index] = valid_moves;
    }
  }

  static void SetupBishopMagicMoves(int p, const vector<BitBoard> &occupancy_variation, vector<BitBoard> *magic_moves) {
    magic_moves->resize(1 << bishop_magic_number_bits[p]);
    for (int i=0, j; i<occupancy_variation.size(); i++) {
      int magic_index = MagicHash(p, occupancy_variation[i], bishop_magic_number, bishop_magic_number_bits);
      BitBoard valid_moves = 0;

      for (j=p+9; j%8!=0 && j<=63; j+=9) { valid_moves |= (1LL<<j); if ((occupancy_variation[i] & (1LL<<j)) != 0) break; }
      for (j=p-9; j%8!=7 && j>= 0; j-=9) { valid_moves |= (1LL<<j); if ((occupancy_variation[i] & (1LL<<j)) != 0) break; }
      for (j=p+7; j%8!=7 && j<=63; j+=7) { valid_moves |= (1LL<<j); if ((occupancy_variation[i] & (1LL<<j)) != 0) break; }
      for (j=p-7; j%8!=0 && j>= 0; j-=7) { valid_moves |= (1LL<<j); if ((occupancy_variation[i] & (1LL<<j)) != 0) break; }

      CHECK_RANGE(magic_index, 0, magic_moves->size());
      (*magic_moves)[magic_index] = valid_moves;
    }
  }
};

}; // namespace Chess
}; // namespace LFL
#endif // LFL_CHESS_MAGIC_H__
