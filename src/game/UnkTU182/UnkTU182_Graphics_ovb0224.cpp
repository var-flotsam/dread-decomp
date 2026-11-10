// Unit 182 of tools/tu_map.py (0x71001604e0-0x7100162530; the graphics device layer next to
// src/engine/UnkGfxSlotSetters_b50.cpp): the constructor of a pooled graphics object's first 0xb8 bytes
// and the refresh of the dirty slots of the three slot groups. Names and layouts are placeholders.
#include "game/UnkGraphicsState.h"
#include "types.h"

// A zero whose constructor isn't constexpr, so the statics are guarded.
struct UnkStaticZero_ovb0224 {
    u32 value;
    UnkStaticZero_ovb0224() : value(0) {}
};

inline const UnkStaticZero_ovb0224& FirstZero_ovb0224() {
    static UnkStaticZero_ovb0224 s_value asm("g_unk_716e34ae30");
    return s_value;
}

inline const UnkStaticZero_ovb0224& SecondZero_ovb0224() {
    static UnkStaticZero_ovb0224 s_value asm("g_unk_716e34ae40");
    return s_value;
}

struct UnkRecord_0xb8_ovb0224 {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
    u32 field_0x18;
    u32 field_0x1c;
    s32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;
    u32 field_0x2c;
    u32 field_0x30;
    u8 field_0x34;
    u8 pad_0x35[3];
    u64 field_0x38;
    u64 field_0x40;
    u64 field_0x48[14];
};
static_assert(sizeof(UnkRecord_0xb8_ovb0224) == 0xb8, "record size");

// The pooled object (0x1a8 bytes; src/engine/UnkGfxSamplerSlots.cpp).
struct UnkGfxObject;

void fn_7100161654(UnkGfxObject* obj) {
    UnkRecord_0xb8_ovb0224* self = reinterpret_cast<UnkRecord_0xb8_ovb0224*>(obj);
    self->field_0x10 = 0;
    self->field_0x18 = 0;
    self->field_0x1c = 0;
    self->field_0x20 = -1;
    self->field_0x0 = 0;
    self->field_0x8 = 0;
    self->field_0x24 = FirstZero_ovb0224().value;
    u32 second = SecondZero_ovb0224().value;
    self->field_0x34 = 0;
    self->field_0x28 = second;
    self->field_0x38 = 0;
    self->field_0x40 = 0;
    // The original keeps this word store after the pair above (a volatile store holds its place).
    *reinterpret_cast<volatile u32*>(&self->field_0x18) = 0;
    for (u64& word : self->field_0x48)
        word = 0;
    self->field_0x28 = SecondZero_ovb0224().value;
    self->field_0x2c = 0;
    self->field_0x30 = 0;
}

bool fn_710015bd80();
s32 fn_710015e9fc(s32* state);
void* fn_7100153cd0();  // the device
void* fn_7100153d50();  // the command buffer
extern void* (*g_unk_716e29ac90)(void* device, u32 a, u32 b);

struct UnkSlotBound_ovb0224 {
    void* ptr;
    s32 field_0x8;
    u8 pad_0xc[4];
    s32 state;  // 0x10
    u8 pad_0x14[0x5c];
    bool dirty;  // 0x70
    bool field_0x71;
    u8 pad_0x72[6];
};
static_assert(sizeof(UnkSlotBound_ovb0224) == sizeof(UnkGraphicsSlot), "slot size");

struct UnkBoundObject_ovb0224 {
    u8 pad_0x0[0xd8];
    u32 field_0xd8;
};

// For every dirty slot of a group: re-derives its state value and, with an object bound, reports
// the resource made from it to the bind callback.
static inline __attribute__((always_inline)) void RefreshGroup_ovb0224(u32 group) {
    UnkSlotBound_ovb0224* slots = reinterpret_cast<UnkSlotBound_ovb0224*>(g_unk_716e2a9730[group]);
    for (u64 i = 0; i < 16; ++i) {
        UnkSlotBound_ovb0224& slot = slots[i];
        if (!slot.dirty)
            continue;
        slot.field_0x8 = fn_710015e9fc(&slot.state);
        UnkBoundObject_ovb0224* object = static_cast<UnkBoundObject_ovb0224*>(slot.ptr);
        if (!object)
            continue;
        void* device = fn_7100153cd0();
        u32 id = object->field_0xd8;
        u32 mode = slot.field_0x8;
        void* resource = g_unk_716e29ac90(device, id, mode);
        void* context = fn_7100153d50();
        g_unk_716e29b858(context, group, i, resource);
    }
}

void fn_7100161250() {
    RefreshGroup_ovb0224(0);
    RefreshGroup_ovb0224(1);
    RefreshGroup_ovb0224(2);
}
