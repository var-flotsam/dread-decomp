// The ammo recharge station's "can recharge now" check (tools/tu_map.py: the CAmmoRechargeComponent
// unit), a copy of fn_7100a9722c in CLifeRechargeComponent_b45.cpp: with a linked object (+0xe8, its
// object at +0x10), the inverse of the unit's own test, else false. The result also goes to the flag
// at +0x235 of the in-game menu's HUD. Names are placeholders.
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkAmmoRechargeLink_b46a2 {
    u8 pad_0x0[0x10];
    void* object;
};

struct UnkAmmoRecharge_b46a2 {
    u8 pad_0x0[0xe8];
    UnkAmmoRechargeLink_b46a2* link;
};

bool fn_7100aa3418(UnkAmmoRecharge_b46a2* self);

bool fn_7100a776fc(UnkAmmoRecharge_b46a2* self) {
    UnkIngameMenu* menu;
    bool flag;
    bool can;
    if (self->link && self->link->object) {
        can = !fn_7100aa3418(self);
        menu = fn_7100deac84(g_unk_7101d47310);
        flag = can;
    } else {
        menu = fn_7100deac84(g_unk_7101d47310);
        asm("");  // the original keeps a call in each branch
        can = false;
        flag = false;
    }
    reinterpret_cast<bool*>(menu->field_0x2d0)[0x235] = flag;
    return can;
}
