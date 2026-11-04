// tools/tu_map.py unit 168 (no class hint): a constructor over the base built by fn_710019e76c
// (fn_7100151684): sets the vtable unk_7101a1d740, an id of -1, two identity 4x4 matrices (+0x2c, +0x6c; zeroed,
// then the diagonal set from the last element), and clears the rest. Names and layouts are placeholders.
#include <new>

#include "types.h"

extern void* g_unk_7101a1d740[];
void fn_710019e76c(void* self);

struct UnkIdentity44_168_b69a1 {
    f32 m[16];

    UnkIdentity44_168_b69a1() {
        __builtin_memset(m, 0, sizeof(m));
        m[15] = 1.0f;
        m[10] = 1.0f;
        m[5] = 1.0f;
        m[0] = 1.0f;
    }
};

struct UnkObject168_b69a1 {
    void* vtable;
    u8 base_0x8[0x20];
    s32 id;                       // 0x28
    UnkIdentity44_168_b69a1 m0;   // 0x2c
    UnkIdentity44_168_b69a1 m1;   // 0x6c
    u8 field_0xac[0x30];
    u8 pad_0xdc[0x14];
    bool field_0xf0;
};
static_assert(__builtin_offsetof(UnkObject168_b69a1, field_0xf0) == 0xf0, "layout");

void fn_7100151684(UnkObject168_b69a1* self) {
    fn_710019e76c(self);
    self->vtable = reinterpret_cast<char*>(g_unk_7101a1d740) + 0x10;
    self->id = -1;
    new (&self->m0) UnkIdentity44_168_b69a1();
    new (&self->m1) UnkIdentity44_168_b69a1();
    self->field_0xf0 = false;
    __builtin_memset(self->field_0xac, 0, sizeof(self->field_0xac));
}
