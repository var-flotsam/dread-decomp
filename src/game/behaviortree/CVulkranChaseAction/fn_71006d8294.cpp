// A destructor in the behaviortree::CVulkranChaseAction unit, as CGoliathAttack's
// (src/game/CGoliathXSlamAttack/fn_7100587280.cpp): its listener (+0x68), its string id (+0x40), then
// the tree node base's destructor fn_7100393ea0.
#include "../../../game_5821e4/CAttackDtor.h"

extern void* g_unk_7101a68738[];  // the action's vtable

void fn_7100393ea0(void* self);  // the tree node base's destructor

struct UnkAction_71006d8294 {
    void** vtable;
    u8 pad_0x8[0x38];
    UnkAttackStrIdRaw field_0x40;
    u8 pad_0x48[0x20];
    UnkAttackListenerRaw field_0x68;
};

void fn_71006d8294(UnkAction_71006d8294* self) {
    self->vtable = g_unk_7101a68738 + 2;
    self->field_0x68.Destroy();
    self->field_0x40.Release();
    fn_7100393ea0(self);
}
