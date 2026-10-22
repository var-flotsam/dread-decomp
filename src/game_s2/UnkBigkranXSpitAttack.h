#pragma once

// CBigkranXSpitAttack (reflection: size 0x128, a CAttack subclass): its default constructor,
// defined twice (fn_71003f13fc and fn_71003f3b30, e.g. the reflected descriptor's in-place copy
// of it). Written with explicit vtable stores and offset-named members, like the destructors
// in game_5821e4/CAttackDtor.h; the base constructor (fn_710036069c) is called out of line.

#include "base/UnkVector.h"
#include "types.h"

extern void* g_unk_7101a31348[];  // the class's vtable
extern void* g_unk_7101a22d48[];  // the embedded listener's vtable
void fn_710036069c(void* attack);  // CAttack's constructor

struct UnkSpitAttackListener {
    void* vtable;
    u64 field_0x8;
};

struct UnkBigkranXSpitAttack {
    void* vtable;
    u8 base_0x8[0x20];
    u64 field_0x28;
    u32 field_0x30;
    u8 pad_0x34[4];
    UnkSpitAttackListener field_0x38;
    u64 field_0x48[2];
    f32 field_0x58;
    f32 field_0x5c[9];
    u8 pad_0x80[8];
    u64 field_0x88[2];
    s32 field_0x98;
    f32 field_0x9c[12];
    u8 pad_0xcc[0x10];
    f32 field_0xdc[3];
    u64 field_0xe8[3];
    base::UnkVector<void*> field_0x100;
};
static_assert(sizeof(UnkBigkranXSpitAttack) == 0x128, "UnkBigkranXSpitAttack layout");

__attribute__((always_inline)) inline void UnkBigkranXSpitAttackInit(UnkBigkranXSpitAttack* self) {
    fn_710036069c(self);
    self->vtable = g_unk_7101a31348 + 2;
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
    for (int i = 0; i < 3; ++i)
        self->field_0xdc[i] = -1.0f;
    self->field_0xe8[0] = 0;
    self->field_0xe8[1] = 0;
    self->field_0xe8[2] = 0;
    new (&self->field_0x100) base::UnkVector<void*>(0xf);
}
