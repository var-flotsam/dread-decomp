// Unit of 0x7100faced0 (tools/tu_map.py, no name hint): sets or clears one bit of the mask at +0x178 when it
// differs. Names and layouts are placeholders.
#include "types.h"

struct UnkBitMaskHolder_b47a3 {
    u8 unk_0x0[0x178];
    u32 mask;
};

void fn_7100faced0(UnkBitMaskHolder_b47a3* self, u32 bit, bool set) {
    u32 mask = self->mask;
    u32 m = 1u << bit;
    if (((mask & m) != 0) != set)
        self->mask = !set ? (mask & ~m) : (mask | m);
}
