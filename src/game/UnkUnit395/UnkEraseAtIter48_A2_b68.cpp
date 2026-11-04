// Unit 395: reflected collection slot 0x128, erase the 0x48-byte entry an index iterator names through
// fn_71002ce01c (which steps by 0x48). The index goes through a 64-bit byte offset from the collection and a
// pointer difference (build/b67/b67a6.md). Names are placeholders.
#include "types.h"

struct UnkCollection48_A2_b68;

struct UnkEntry48_A2_b68 {
    u8 unk_0x0[0x48];
};

void fn_71002ce01c(UnkCollection48_A2_b68* v, u32 index);

void fn_71002cd278(UnkCollection48_A2_b68* self, const u32* it) {
    UnkEntry48_A2_b68* base = reinterpret_cast<UnkEntry48_A2_b68*>(self);
    u64 offset = (u64)*it * sizeof(UnkEntry48_A2_b68);
    UnkEntry48_A2_b68* p = reinterpret_cast<UnkEntry48_A2_b68*>(reinterpret_cast<char*>(base) + offset);
    fn_71002ce01c(self, (u32)(p - base));
}
