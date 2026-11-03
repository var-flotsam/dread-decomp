// Two recharge stations' "can recharge now" checks of the same shape (tools/tu_map.py: the
// CLifeRechargeComponent unit, fn_7100a83348 the original, and the CTotalRechargeComponent unit,
// fn_7100a9722c): with a linked object (+0xe8, its object at +0x10), the inverse of the unit's own
// test, else false. The result also goes to the flag at +0x235 of the in-game menu's HUD. Names are
// placeholders.
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkLifeRechargeLink_b45a2 {
    u8 pad_0x0[0x10];
    void* object;
};

struct UnkLifeRecharge_b45a2 {
    u8 pad_0x0[0xe8];
    UnkLifeRechargeLink_b45a2* link;
};

bool fn_7100aa33a4(UnkLifeRecharge_b45a2* self);
bool fn_7100aa3330(UnkLifeRecharge_b45a2* self);

template <bool (*Test)(UnkLifeRecharge_b45a2*)>
bool UnkCanRecharge_b45a2(UnkLifeRecharge_b45a2* self) {
    UnkIngameMenu* menu;
    bool flag;
    bool can;
    if (self->link && self->link->object) {
        can = !Test(self);
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

bool fn_7100a83348(UnkLifeRecharge_b45a2* self) {
    return UnkCanRecharge_b45a2<fn_7100aa33a4>(self);
}

bool fn_7100a9722c(UnkLifeRecharge_b45a2* self) {
    return UnkCanRecharge_b45a2<fn_7100aa3330>(self);
}
