// CChozoWarriorXUltimateGrabAttack's destructor: the twelve property entries at +0x148.. clear
// their blocks, reset their vtable and destroy their reflected value (last first), then the
// property list's vtable, the two listeners (+0x60, +0x40) and CAttack's destructor. Names and
// layouts are placeholders.
#include "base/reflection/UnkReflCast.h"
#include "../../game_5821e4/CAttackDtor.h"
#include "game/CChozoWarriorXUltimateGrabAttack.h"

extern void* g_unk_7101a3b5b8[];  // the entries' base vtable

struct UnkUltimateGrabBlock_48a60c {
    u64 v[8];
};

struct UnkUltimateGrabEntry_48a60c {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    base::reflection::UnkReflValue value;
    UnkUltimateGrabBlock_48a60c block;

    void Destroy() {
        for (int i = 0; i < 8; i++)
            block.v[i] = 0;
        vtable = g_unk_7101a3b5b8 + 2;
        fn_71000a1c60(&value);
    }
};

struct UnkUltimateGrab_48a60c {
    void** vtable;
    u8 pad_0x8[0x38];
    UnkAttackListenerRaw listener_a;  // 0x40
    UnkAttackListenerRaw listener_b;  // 0x60
    void** list_vtable;               // 0x80
    u64 list_field_0x8;               // 0x88
    u64 list_field_0x10;              // 0x90
    u64 list_field_0x18;              // 0x98
    u32 list_count;                   // 0xa0
    u8 pad_0xa4[0xa4];
    UnkUltimateGrabEntry_48a60c entries[12];  // 0x148
};
static_assert(__builtin_offsetof(UnkUltimateGrab_48a60c, entries) == 0x148, "layout");

void fn_710048a60c(UnkUltimateGrab_48a60c* self) {
    self->vtable = g_unk_7101a3b438 + 2;
    for (int i = 11; i >= 0; --i)
        self->entries[i].Destroy();
    self->list_vtable = g_unk_7101a3b610 + 2;
    self->list_field_0x10 = 0;
    self->list_field_0x18 = 0;
    self->list_count = 0;
    self->listener_b.Destroy();
    self->listener_a.Destroy();
    fn_71003606c0(self);
}
