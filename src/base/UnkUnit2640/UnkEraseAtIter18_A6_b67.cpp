// Unit 2640's unit: reflected collection slot 0x128, erase the 0x18-byte entry an index iterator names through
// fn_7100ffbff0. The index goes through a 64-bit byte offset from the collection and a pointer difference, which keeps
// the original's unfolded exact division. Names are placeholders.
#include "types.h"

struct UnkCollection_U2640x18_A6_b67;

struct UnkEntry_U2640x18_A6_b67 {
    u8 unk_0x0[0x18];
};

void fn_7100ffbff0(UnkCollection_U2640x18_A6_b67* self, u32 index);

void fn_7100ffb69c(UnkCollection_U2640x18_A6_b67* self, const u32* it) {
    UnkEntry_U2640x18_A6_b67* base = reinterpret_cast<UnkEntry_U2640x18_A6_b67*>(self);
    u64 offset = (u64)*it * sizeof(UnkEntry_U2640x18_A6_b67);
    UnkEntry_U2640x18_A6_b67* p = reinterpret_cast<UnkEntry_U2640x18_A6_b67*>(reinterpret_cast<char*>(base) + offset);
    fn_7100ffbff0(self, (u32)(p - base));
}
