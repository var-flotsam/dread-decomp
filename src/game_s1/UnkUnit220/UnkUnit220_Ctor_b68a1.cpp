// tools/tu_map.py unit 220 (no class hint): a constructor over the base built by fn_710019e76c
// (fn_710019a2b8): sets the vtable unk_7101a1e188, an identity 4x4 matrix at +0x28 (as in unit 196), a few words, and
// a zeroed 0x40-byte block from fn_7100083ba0 (flags 0x32) at +0x78. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/UnkTU226.h"
#include "types.h"

extern void* g_unk_7101a1e188[];

namespace {

struct UnkMatrix44_b68a1 {
    f32 m[16];

    UnkMatrix44_b68a1() {
        __builtin_memset(m, 0, sizeof(m));
        m[15] = 1.0f;
        m[10] = 1.0f;
        m[5] = 1.0f;
        m[0] = 1.0f;
    }
};

struct UnkBlock40_b68a1 {
    u64 words[8];
};

}  // namespace

struct UnkObject220_b68a1 {
    void* vtable;
    u8 base_0x8[0x18];
    u64 field_0x20;
    UnkMatrix44_b68a1 m;  // 0x28
    u64 field_0x68;  // 0 in the low word, 1 in the high one
    f32 field_0x70;
    u32 pad_0x74;
    UnkBlock40_b68a1* block;  // 0x78
    u32 field_0x80;
};
static_assert(__builtin_offsetof(UnkObject220_b68a1, field_0x80) == 0x80, "layout");

void fn_710019a2b8(UnkObject220_b68a1* self) {
    fn_710019e76c(self);
    self->vtable = reinterpret_cast<char*>(g_unk_7101a1e188) + 0x10;
    self->field_0x20 = 0;
    new (&self->m) UnkMatrix44_b68a1();
    self->field_0x68 = 0x100000000;
    self->field_0x70 = 1.0f;
    self->block = nullptr;
    self->field_0x80 = 4;
    self->block = new (fn_7100083ba0(sizeof(UnkBlock40_b68a1), 0x32)) UnkBlock40_b68a1();
}
