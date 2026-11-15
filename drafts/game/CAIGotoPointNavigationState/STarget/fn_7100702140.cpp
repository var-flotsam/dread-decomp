// CAIGotoPointNavigationState::STarget unit: constructor of the target object (vtable 0x7101a6bb48), on its base
// (fn_7100703814), with the sub-object vtable at +0xb0, cleared state and the path query at +0x140. Names and
// layouts are placeholders.
#include "game/UnkTU1124.h"
#include "types.h"

struct UnkGotoTargetQuery;
void fn_7100706818(UnkGotoTargetQuery* self, void* owner);
extern void* g_unk_7101a6bb48[];
extern void* g_unk_7101a25b38[];

struct __attribute__((packed)) UnkGotoTargetCounters {
    u64 field_0x0;
    u32 field_0x8;
    u8 field_0xc;
};

struct UnkGotoTargetNew {
    void** vtable;
    u8 pad_0x8[0xa8];
    void** field_0xb0;
    UnkGotoTargetCounters field_0xb8;
    u8 pad_0xc5[0x7];
    bool field_0xcc;
    u8 pad_0xcd[0x3];
    u64 field_0xd0;
    u64 field_0xd8;
    bool field_0xe0;
    u8 pad_0xe1[0x3];
    u32 field_0xe4;
    u8 pad_0xe8[0x4];
    bool field_0xec;
    u8 pad_0xed[0x1];
    bool field_0xee;
    u8 pad_0xef[0x1];
    bool field_0xf0;
    u8 pad_0xf1[0x7];
    u64 field_0xf8;
    u64 field_0x100;
    bool field_0x108;
    u8 pad_0x109[0xf];
    bool field_0x118;
    u8 pad_0x119[0x1];
    bool field_0x11a;
    u8 pad_0x11b[0x5];
    u64 field_0x120;
    u64 field_0x128;
    u32 field_0x130;
    u32 field_0x134;
    bool field_0x138;
    u8 pad_0x139[0x7];
    u8 field_0x140[0x38180 - 0x140];
    bool field_0x38180;
};

void fn_7100702140(UnkGotoTargetNew* self, u32 id) {
    fn_7100703814(reinterpret_cast<UnkUnit1124Object*>(self), id);
    self->vtable = g_unk_7101a6bb48 + 2;
    self->field_0xcc = false;
    self->field_0xb8 = {};
    self->field_0xd0 = 0;
    self->field_0xd8 = 0;
    self->field_0xe0 = false;
    self->field_0xb0 = g_unk_7101a25b38 + 2;
    self->field_0xec = false;
    self->field_0xee = false;
    self->field_0xf0 = false;
    self->field_0x118 = false;
    self->field_0x11a = false;
    self->field_0x130 = 2;
    self->field_0x134 = 1;
    self->field_0x108 = false;
    self->field_0x138 = false;
    self->field_0xe4 = 1;
    self->field_0x120 = 0;
    self->field_0x128 = 0;
    self->field_0xf8 = 0;
    self->field_0x100 = 0;
    fn_7100706818(reinterpret_cast<UnkGotoTargetQuery*>(self->field_0x140), self);
    self->field_0x38180 = false;
}
