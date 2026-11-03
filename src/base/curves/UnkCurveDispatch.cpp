// The base::curves unit (tools/tu_map.py unit 34; enums EKeysType, ETimeType, EInterpMode are
// registered by src/game_s1/fn_71000296e0.cpp): dispatches on a curve's 16-bit header through the
// unit's handler table, indexed by bits 0-2, 3-4 and 5 of the header (rows of 2x2 entries; the middle index can reach 3 as in the original). Names are placeholders.
#include "types.h"

#define HIDDEN __attribute__((visibility("hidden")))

typedef void UnkCurveHandler34(const u16* curve);

extern UnkCurveHandler34* g_unk_7101a19760[][2][2] HIDDEN;

void fn_7100027964(const u16* curve) {
    u16 header = *curve;
    g_unk_7101a19760[header & 7][(header >> 3) & 3][(header >> 5) & 1](curve);
}
