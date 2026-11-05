// tools/tu_map.py unit 215 (no class hint): a constructor over the base built by fn_710019e76c
// (fn_7100196480): sets the vtable unk_7101a1e078, an embedded 0x5c-byte record at +0x30 (fn_7100025624), two
// small ints (6, 7), an identity 4x4 matrix at +0xbc, a count of 3, then stores the four screen corners moved by
// half a pixel (as in src/engine/UnkScreenCorners.cpp) into four globals. Names and layouts are placeholders.
#include <new>

#include "base/UnkInitRecord_0x5c.h"
#include "types.h"

extern void* g_unk_7101a1e078[];
void fn_710019e76c(void* self);
f32 fn_71000768c0(f32 x);
f32 fn_71000768f0(f32 y);

// The corners as (x, y) pairs (reached through the GOT).
extern f32 unk_7101a1e048[];
extern f32 unk_7101a1e054[];
extern f32 unk_7101a1e060[];
extern f32 unk_7101a1e06c[];

struct UnkIdentity44_215_b69a1 {
    f32 m[16];

    UnkIdentity44_215_b69a1() {
        __builtin_memset(m, 0, sizeof(m));
        m[15] = 1.0f;
        m[10] = 1.0f;
        m[5] = 1.0f;
        m[0] = 1.0f;
    }
};

struct UnkObject215_b69a1 {
    void* vtable;
    u8 base_0x8[0x18];
    u64 field_0x20;
    u64 field_0x28;
    u8 record[0x5c];  // 0x30
    u32 pad_0x8c;
    u64 field_0x90;
    u32 field_0x98;
    u64 field_0xa0;
    u64 field_0xa8;
    u32 field_0xb0;
    u32 field_0xb4;
    u32 field_0xb8;
    UnkIdentity44_215_b69a1 m;  // 0xbc
    u32 field_0xfc;
    u64 field_0x100[6];
    u64 field_0x130;
};
static_assert(__builtin_offsetof(UnkObject215_b69a1, field_0x130) == 0x130, "layout");

void fn_7100196480(UnkObject215_b69a1* self) {
    fn_710019e76c(self);
    self->vtable = reinterpret_cast<char*>(g_unk_7101a1e078) + 0x10;
    fn_7100025624(self->record);
    self->field_0x90 = 0;
    self->field_0x98 = 0;
    self->field_0xa0 = 0;
    self->field_0xa8 = 0;
    self->field_0xb0 = 0;
    self->field_0xb4 = 6;
    self->field_0xb8 = 7;
    new (&self->m) UnkIdentity44_215_b69a1();
    self->field_0xfc = 3;
    self->field_0x20 = 0;
    self->field_0x28 = 0;
    for (u64& w : self->field_0x100)
        w = 0;
    self->field_0x130 = 0;
    unk_7101a1e048[0] = fn_71000768c0(-1.0f);
    unk_7101a1e048[1] = fn_71000768f0(-1.0f);
    unk_7101a1e054[0] = fn_71000768c0(-1.0f);
    unk_7101a1e054[1] = fn_71000768f0(1.0f);
    unk_7101a1e060[0] = fn_71000768c0(1.0f);
    unk_7101a1e060[1] = fn_71000768f0(1.0f);
    unk_7101a1e06c[0] = fn_71000768c0(1.0f);
    unk_7101a1e06c[1] = fn_71000768f0(-1.0f);
}
