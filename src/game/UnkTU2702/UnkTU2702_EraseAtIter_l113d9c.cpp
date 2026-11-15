// Unit 2702: erases the 0x48-byte entry an index iterator names, by index (fn_7101113d9c).
// Names and layouts are placeholders.
#include "game/UnkCallbackSetVectorFwd.h"
#include "types.h"

void fn_7101114d10(UnkCallbackSetVector* v, u32 index);

struct UnkEntry48_l113d9c {
    u8 unk_0x0[0x48];
};

void fn_7101113d9c(UnkCallbackSetVector* self, const u32* it) {
    UnkEntry48_l113d9c* base = reinterpret_cast<UnkEntry48_l113d9c*>(self);
    u64 offset = (u64)*it * sizeof(UnkEntry48_l113d9c);
    UnkEntry48_l113d9c* p = reinterpret_cast<UnkEntry48_l113d9c*>(reinterpret_cast<char*>(base) + offset);
    fn_7101114d10(self, (u32)(p - base));
}
