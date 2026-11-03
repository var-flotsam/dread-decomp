// From unit 54 of tools/tu_map.py: whether a 4x4 matrix is bitwise equal to the identity
// g_unk_7101a1a680 (as src/base/spatial/UnkBillboard.cpp declares it). Names are placeholders.
#include "base/UnkMatrix44.h"
#include "types.h"

extern UnkMatrix44 g_unk_7101a1a680;

bool fn_7100076a5c(const UnkMatrix44* m) {
    return __builtin_memcmp(m, &g_unk_7101a1a680, sizeof(UnkMatrix44)) == 0;
}
