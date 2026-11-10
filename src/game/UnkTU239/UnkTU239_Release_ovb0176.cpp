// Unit 239 (no reflected class): a render target's tracked memory block and its counters. Names and
// layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkRenderTargetSlot;

void fn_7100160590(u64 slot);
void fn_710015f810(UnkRenderTargetSlot* out, u32 format, u32 width, u32 height, u32 a, void* b, void* c, u32 d,
                   u32 e);

struct UnkTrackedBlock_ovb0176 {
    u8 pad_0x0[4];
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u8 pad_0x1c[4];
    u64 field_0x20;  // total
    u64 field_0x28;  // total
    u8 field_0x30[0x10];
};

struct UnkSlot_ovb0176 {
    u64 field_0x0;
    u32 width;
    u32 height;
    bool field_0x10;
};

struct UnkTrackedTarget_ovb0176 {
    u8 live;  // 0x0
    u8 pad_0x1[0xf];
    UnkTrackedBlock_ovb0176* block;  // 0x10
    u32 field_0x18;
    u32 field_0x1c;
    u32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;
    u8 pad_0x2c[4];
    CStrId name;  // 0x30
    UnkSlot_ovb0176 slot;  // 0x38
};
static_assert(__builtin_offsetof(UnkTrackedTarget_ovb0176, slot) == 0x38, "layout");

void fn_71001b3754(UnkTrackedTarget_ovb0176* self) {
    if (self->live) {
        self->field_0x28 = 0;
        self->field_0x18 = 0;
        self->field_0x1c = 0;
        self->field_0x20 = 0;
        self->field_0x24 = 0;
        self->name = g_unk_7101d470e0;
        fn_7100160590(self->slot.field_0x0);
        UnkTrackedBlock_ovb0176* block = self->block;
        if (block) {
            u64 address = reinterpret_cast<u64>(block);
            if (block->field_0x28) {
                u64 total = block->field_0x28 - address;
                __builtin_memcpy(&block->field_0x28, &total, sizeof(total));
                block = self->block;
            }
            if (block->field_0x20)
                block->field_0x20 -= address;
        }
        self->live = 0;
    }
    self->name.Reset();
}
