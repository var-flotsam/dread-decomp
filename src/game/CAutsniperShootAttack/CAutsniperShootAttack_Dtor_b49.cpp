// CAutsniperShootAttack (tools/tu_map.py): the destructor. The vtable, the path member at +0xd0
// (fn_71002ad0dc), the listener at +0x30 (its own vtable and unregister function fn_71010cb2fc;
// game_5821e4/CAttackDtor.h), then CAttack's destructor. Names and layouts are placeholders.
#include "../../game_5821e4/CAttackDtor.h"

extern void* g_unk_7101a2e5d0[];  // the attack's vtable
extern void* g_unk_7101a2aef8[];  // the listener's vtable
struct UnkObject_71002ad0dc;
void fn_71002ad0dc(UnkObject_71002ad0dc* self);
void fn_71010cb2fc(void*, void*);

struct UnkAutsniperShootAttack_b49 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkAttackListenerRaw listener;
    u8 pad_0x50[0x80];
    u8 field_0xd0[8];
};

void fn_71003d37d8(UnkAutsniperShootAttack_b49* self) {
    self->vtable = g_unk_7101a2e5d0 + 2;
    fn_71002ad0dc(reinterpret_cast<UnkObject_71002ad0dc*>(self->field_0xd0));
    self->listener.Destroy(g_unk_7101a2aef8, fn_71010cb2fc);
    fn_71003606c0(self);
}
