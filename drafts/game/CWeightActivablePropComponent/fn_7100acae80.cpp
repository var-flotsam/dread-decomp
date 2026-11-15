// CWeightActivablePropComponent's unit: a constructor; base part, vtable, several zeroed
// members and sub-objects, the list at +0x200, then its own vf_0x18.
// Names and layouts are placeholders.
#include "game/CAbilityGhostAura.h"
#include "game/CComponent.h"
#include "types.h"

struct UnkWeightList;
void fn_7100acac2c(UnkWeightList* self);

extern void* g_unk_7101a971b8[];
extern void* g_unk_7101a973c8[];
extern void* g_unk_7101a97478[];

struct UnkWeightActivable {
    void** vtable;
    u8 pad_0x8[0x38];
    void* field_0x40;
    u8 pad_0x48[8];
    void** field_0x50;
    u64 field_0x58;
    u32 field_0x60;
    u8 pad_0x64[4];
    u64 field_0x68;
    u64 field_0x70;
    u32 field_0x78;
    u32 field_0x7c;
    u64 field_0x80;
    u64 field_0x88;
    u64 field_0x90;
    u64 field_0x98;
    u64 field_0xa0;
    u64 field_0xa8;
    u64 field_0xb0;
    u64 field_0xb8;
    void** field_0xc0;
    u64 field_0xc8;
    u32 field_0xd0;
    u8 pad_0xd4[4];
    u64 field_0xd8;
    u64 field_0xe0;
    u32 field_0xe8;
    u32 field_0xec;
    u64 field_0xf0;
    u64 field_0xf8;
    u64 field_0x100;
    u64 field_0x108;
    u64 field_0x110;
    u64 field_0x118;
    u64 field_0x120;
    u64 field_0x128;
    void** field_0x130;
    u64 field_0x138;
    u64 field_0x140;
    u64 field_0x148;
    u32 field_0x150;
    u8 pad_0x154[0x1d4 - 0x154];
    u32 field_0x1d4;
    u16 field_0x1d8;
    u8 pad_0x1da[6];
    void** field_0x1e0;
    u64 field_0x1e8;
    u64 field_0x1f0;
    u64 field_0x1f8;
    u8 list[8];  // 0x200
};

void fn_7100acae80(UnkWeightActivable* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->field_0x60 = 1;
    self->vtable = g_unk_7101a971b8 + 2;
    self->field_0x68 = 0;
    self->field_0x70 = 0;
    self->field_0x40 = g_unk_7101a971b8 + 0x34;
    self->field_0x88 = 0;
    self->field_0x90 = 0;
    self->field_0x98 = 0;
    self->field_0xa0 = 0;
    self->field_0xd0 = 2;
    self->field_0xa8 = 0;
    self->field_0xb0 = 0;
    self->field_0xb8 = 0;
    self->field_0xc0 = g_unk_7101a973c8 + 2;
    self->field_0xc8 = 0;
    self->field_0x50 = g_unk_7101a973c8 + 2;
    self->field_0x58 = 0;
    self->field_0x118 = 0;
    self->field_0x120 = 0;
    self->field_0x108 = 0;
    self->field_0x110 = 0;
    self->field_0x78 = 2;
    self->field_0x7c = 1;
    self->field_0x80 = 0;
    self->field_0xd8 = 0;
    self->field_0xe0 = 0;
    self->field_0xf8 = 0;
    self->field_0x100 = 0;
    self->field_0xe8 = 2;
    self->field_0xec = 1;
    self->field_0xf0 = 0;
    self->field_0x150 = 0;
    self->field_0x138 = 0;
    self->field_0x1d8 = 0;
    self->field_0x128 = 0;
    self->field_0x130 = g_unk_7101a97478 + 2;
    self->field_0x1d4 = 3;
    self->field_0x140 = 0;
    self->field_0x148 = 0;
    self->field_0x1e0 = g_unk_7101a22d48 + 2;
    self->field_0x1e8 = 0;
    self->field_0x1f0 = 0;
    self->field_0x1f8 = 0;
    fn_7100acac2c(reinterpret_cast<UnkWeightList*>(self->list));
    reinterpret_cast<void (*)(UnkWeightActivable*)>(self->vtable[3])(self);
}
