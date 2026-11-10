// CChozoCommanderKiGrabAttack's destructor: the seven property entries at +0x150.. clear their
// blocks, reset their vtable and destroy their reflected value (last first), then the property
// list's vtable, the two listeners (+0x68, +0x48) and CAttack's destructor. Names and layouts are
// placeholders.
#include "base/reflection/UnkReflCast.h"
#include "../../game_5821e4/CAttackDtor.h"

extern void* g_unk_7101a35b28[];  // the class's vtable
extern void* g_unk_7101a35c98[];  // the entries' base vtable
extern void* g_unk_7101a35cf0[];  // the property list's vtable

struct UnkKiGrabBlock_b0266 {
    u64 v[8];
};

struct UnkKiGrabEntry_b0266 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    base::reflection::UnkReflValue value;
    UnkKiGrabBlock_b0266 block;

    void Destroy() {
        for (int i = 0; i < 8; i++)
            block.v[i] = 0;
        vtable = g_unk_7101a35c98 + 2;
        fn_71000a1c60(&value);
    }
};

struct UnkKiGrab_b0266 {
    void** vtable;
    u8 pad_0x8[0x40];
    UnkAttackListenerRaw listener_a;  // 0x48
    UnkAttackListenerRaw listener_b;  // 0x68
    void** list_vtable;               // 0x88
    u64 list_field_0x8;               // 0x90
    u64 list_field_0x10;              // 0x98
    u64 list_field_0x18;              // 0xa0
    u32 list_count;                   // 0xa8
    u8 pad_0xac[0xa4];
    UnkKiGrabEntry_b0266 entries[7];  // 0x150
};
static_assert(__builtin_offsetof(UnkKiGrab_b0266, entries) == 0x150, "layout");

void fn_710043348c(UnkKiGrab_b0266* self) {
    self->vtable = g_unk_7101a35b28 + 2;
    for (int i = 6; i >= 0; --i)
        self->entries[i].Destroy();
    self->list_vtable = g_unk_7101a35cf0 + 2;
    self->list_field_0x10 = 0;
    self->list_field_0x18 = 0;
    self->list_count = 0;
    self->listener_b.Destroy();
    self->listener_a.Destroy();
    fn_71003606c0(self);
}
