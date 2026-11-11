// Unit 1452 of tools/tu_map.py (no name hint): the destructor and deleting destructor of the event holder that
// fn_710099a084 (src/game_hi/fn_710099a084.cpp) embeds at +0x8e0: three callback-slot members (+0xe8, +0x158,
// +0x1c8), the record at +0x30, then the base (vtable g_unk_7101a85c80, member at +0x18). Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU1449.h"
#include "game/UnkTU1452.h"
#include "types.h"

namespace {

struct UnkEventBase_A4_b74 {
    void* vtable;
    u8 pad_0x8[0x10];
    base::reflection::UnkReflValue field_0x18;

    void DestroyBase(void** vt) {
        vtable = vt + 2;
        fn_71000a1c60(&field_0x18);
    }
};

struct UnkSlots_A4_b74 : UnkEventBase_A4_b74 {
    void* slots[8];

    void Destroy(void** vt) {
        slots[7] = nullptr;
        slots[6] = nullptr;
        slots[5] = nullptr;
        slots[4] = nullptr;
        slots[3] = nullptr;
        slots[2] = nullptr;
        slots[1] = nullptr;
        slots[0] = nullptr;
        DestroyBase(vt);
    }
};

struct UnkRecord_A4_b74 {
    void* vtable;
    u8 pad_0x8[0x8];
    void* field_0x10;
    void* field_0x18;
    u32 field_0x20;

    void Destroy(void** vt) {
        vtable = vt + 2;
        field_0x10 = nullptr;
        field_0x18 = nullptr;
        field_0x20 = 0;
    }
};

}  // namespace

struct UnkEventHolder_A4_b74 : UnkEventBase_A4_b74 {
    UnkRecord_A4_b74 field_0x30;
    u8 pad_0x58[0x90];
    UnkSlots_A4_b74 field_0xe8;
    UnkSlots_A4_b74 field_0x158;
    UnkSlots_A4_b74 field_0x1c8;
};
static_assert(sizeof(UnkEventHolder_A4_b74) == 0x238, "layout");

static inline __attribute__((always_inline)) void Destroy_A4_b74(UnkEventHolder_A4_b74* self) {
    self->vtable = g_unk_7101a85e98 + 2;
    self->field_0x1c8.Destroy(g_unk_7101a85f98);
    self->field_0x158.Destroy(g_unk_7101a85f98);
    self->field_0xe8.Destroy(g_unk_7101a85f98);
    self->field_0x30.Destroy(g_unk_7101a85ff0);
    self->DestroyBase(g_unk_7101a85c80);
}

void fn_7100982254(UnkEventHolder_A4_b74* self) {
    Destroy_A4_b74(self);
}

void fn_7100982318(UnkEventHolder_A4_b74* self) {
    Destroy_A4_b74(self);
    fn_7100083c00(self);
}
