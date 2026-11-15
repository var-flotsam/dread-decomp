// CSluggerSpitAttack unit: the default constructor (CAttack base, vtable, listener, zeroed members).
// Names and layouts are placeholders.
#include "game/CAbilityGhostAura.h"
#include "types.h"

void fn_710036069c(void* attack);  // CAttack's constructor
extern void* g_unk_7101a64db0[];   // CSluggerSpitAttack's vtable

struct UnkSluggerListener {
    void* vtable;
    u64 field_0x8;
};

struct UnkSluggerSpitAttack {
    void* vtable;
    u8 base_0x8[0x20];
    u64 field_0x28;
    u32 field_0x30;
    u8 pad_0x34[4];
    UnkSluggerListener field_0x38;
    u64 field_0x48[2];
    f32 field_0x58;
    f32 field_0x5c[9];
    u8 pad_0x80[8];
    u64 field_0x88[2];
    s32 field_0x98;
    f32 field_0x9c[12];
    u8 pad_0xcc[0x14];
    u64 field_0xe0;
};

void fn_71006c083c(UnkSluggerSpitAttack* self) {
    fn_710036069c(self);
    self->vtable = g_unk_7101a64db0 + 2;
    self->field_0x28 = 0;
    self->field_0x30 = 0;
    self->field_0x38.vtable = g_unk_7101a22d48 + 2;
    self->field_0x38.field_0x8 = 0;
    self->field_0x48[0] = 0;
    self->field_0x48[1] = 0;
    self->field_0x58 = -1.0f;
    for (int i = 0; i < 9; ++i)
        self->field_0x5c[i] = 0;
    self->field_0x88[0] = 0;
    self->field_0x88[1] = 0;
    self->field_0x98 = -1;
    for (int i = 0; i < 12; ++i)
        self->field_0x9c[i] = 0;
    self->field_0xe0 = 0;
}
