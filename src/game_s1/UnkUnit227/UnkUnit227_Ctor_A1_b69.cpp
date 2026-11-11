// tools/tu_map.py unit 227 (no class hint): a constructor over the base built by fn_710019e76c
// (fn_710019fa64): sets the vtable unk_7101a1e2f8, clears a 0xc4-byte block (one unit float in it), then a run of
// scales and limits (4, 1e-6, 500000, 10, two 0x800 counts), two identity 4x4 matrices (+0x138, +0x178; zeroed,
// then the diagonal set from the last element) and a few flags. Names and layouts are placeholders.
#include <new>

#include "game/UnkTU226.h"
#include "types.h"

extern void* g_unk_7101a1e2f8[];

struct UnkIdentity44_227_b69a1 {
    f32 m[16];

    UnkIdentity44_227_b69a1() {
        __builtin_memset(m, 0, sizeof(m));
        m[15] = 1.0f;
        m[10] = 1.0f;
        m[5] = 1.0f;
        m[0] = 1.0f;
    }
};

struct UnkObject227_b69a1 {
    void* vtable;
    u8 base_0x8[0x18];
    u64 field_0x20[14];
    u32 field_0x90;
    f32 field_0x94;
    u64 field_0x98[9];
    u32 field_0xe0;
    f32 field_0xe4;
    f32 field_0xe8;
    f32 field_0xec;
    u64 field_0xf0;
    bool field_0xf8;
    u32 field_0xfc;
    u32 field_0x100;
    u32 field_0x104;
    u32 field_0x108;
    f32 field_0x10c;
    f32 field_0x110;
    f32 field_0x114;
    f32 field_0x118;
    f32 field_0x11c;
    u32 field_0x120;
    u32 field_0x124;
    u32 field_0x128;
    u32 field_0x12c;
    u32 field_0x130;
    u32 field_0x134;
    UnkIdentity44_227_b69a1 m0;  // 0x138
    UnkIdentity44_227_b69a1 m1;  // 0x178
    bool field_0x1b8;
    bool field_0x1b9;
    bool field_0x1ba;
    bool field_0x1bb;
    bool field_0x1bc;
    bool field_0x1bd;
    bool field_0x1be;
};
static_assert(__builtin_offsetof(UnkObject227_b69a1, m0) == 0x138, "layout");
static_assert(__builtin_offsetof(UnkObject227_b69a1, field_0x1be) == 0x1be, "layout");

void fn_710019fa64(UnkObject227_b69a1* self) {
    fn_710019e76c(self);
    self->vtable = reinterpret_cast<char*>(g_unk_7101a1e2f8) + 0x10;
    for (u64& w : self->field_0x20)
        w = 0;
    self->field_0x90 = 0;
    self->field_0x94 = 1.0f;
    for (u64& w : self->field_0x98)
        w = 0;
    self->field_0xe0 = 0;
    self->field_0xe4 = 4.0f;
    self->field_0xe8 = 1.0f;
    self->field_0xec = 1.0f;
    self->field_0xf0 = 0;
    self->field_0xf8 = false;
    self->field_0xfc = 0;
    self->field_0x100 = 0;
    self->field_0x104 = 0;
    self->field_0x108 = 0;
    self->field_0x10c = 1e-6f;
    self->field_0x110 = 1.0f;
    self->field_0x114 = 1.0f;
    self->field_0x118 = 500000.0f;
    self->field_0x11c = 10.0f;
    self->field_0x120 = 0x800;
    self->field_0x124 = 0x800;
    self->field_0x130 = 0;
    self->field_0x134 = 0;
    new (&self->m0) UnkIdentity44_227_b69a1();
    new (&self->m1) UnkIdentity44_227_b69a1();
    self->field_0x128 = 0;
    self->field_0x12c = 0;
    self->field_0x1b8 = true;
    self->field_0x1b9 = false;
    self->field_0x1ba = false;
    self->field_0x1bb = true;
    self->field_0x1bc = false;
    self->field_0x1bd = true;
    self->field_0x1be = false;
}
