// Unit 1454: destructor of an event-like object (the member at +0x268 of fn_710099a084's object):
// five callback slot arrays, two small records, then its base. Names and layouts are placeholders.
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU1449.h"
#include "game/UnkTU1454.h"
#include "types.h"

// The common base: a vtable and a reflected value at +0x18 (0x30 bytes).
struct UnkEventBase_l9878fc {
    void* vtable;
    u8 pad_0x8[0x10];
    base::reflection::UnkReflValue field_0x18;

    void DestroyBase(void** vt) {
        vtable = vt + 2;
        fn_71000a1c60(&field_0x18);
    }
};

// Eight callback slots after the base (0x70 bytes).
struct UnkSlots_l9878fc : UnkEventBase_l9878fc {
    void* slots[8];

    void Destroy(void** vt) {
        for (int i = 7; i >= 0; --i)
            slots[i] = nullptr;
        DestroyBase(vt);
    }
};

// A small record with its own vtable (0x28 bytes).
struct UnkRecord_l9878fc {
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

struct UnkEvent_l9878fc : UnkEventBase_l9878fc {
    UnkRecord_l9878fc field_0x30;
    u8 pad_0x58[0x88];
    UnkRecord_l9878fc field_0xe0;
    u8 pad_0x108[0x88];
    UnkSlots_l9878fc field_0x190;
    UnkSlots_l9878fc field_0x200;
    UnkSlots_l9878fc field_0x270;
    UnkSlots_l9878fc field_0x2e0;
    UnkSlots_l9878fc field_0x350;
};
static_assert(sizeof(UnkEvent_l9878fc) == 0x3c0, "layout");

void fn_71009878fc(UnkEvent_l9878fc* self) {
    self->vtable = g_unk_7101a86030 + 2;
    self->field_0x350.Destroy(g_unk_7101a86130);
    self->field_0x2e0.Destroy(g_unk_7101a86130);
    self->field_0x270.Destroy(g_unk_7101a861e0);
    self->field_0x200.Destroy(g_unk_7101a861e0);
    self->field_0x190.Destroy(g_unk_7101a861e0);
    self->field_0xe0.Destroy(g_unk_7101a86238);
    self->field_0x30.Destroy(g_unk_7101a86278);
    self->DestroyBase(g_unk_7101a85c80);
}
