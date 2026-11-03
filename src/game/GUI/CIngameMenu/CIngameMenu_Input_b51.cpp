// GUI::CIngameMenu unit (tools/tu_map.py, the name from reflection): input in states 1 and 2 of the menu
// (the state at +0x2e0, as in fn_7100b2de48.cpp): state 2 goes back to 1; state 1 goes to 2 once the
// game manager's object at +0x498 allows it (fn_710118a8d0). Names and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkPlacedObj;
struct UnkStateOwner;
bool fn_710118a8d0(UnkPlacedObj* placed);
void fn_7100b2eaac(UnkStateOwner* self, s32 state, bool a, bool b);

struct UnkIngameMenuInput_b51 {
    u8 unk_0x0[0x1a];
    bool handled;
};

struct UnkIngameMenuState_b51 {
    u8 unk_0x0[0x2e0];
    s32 state;  // 0x2e0
};

void fn_7100b2ed08(UnkIngameMenuState_b51* self, const UnkIngameMenuInput_b51* event) {
    if (event->handled)
        return;
    UnkStateOwner* owner = reinterpret_cast<UnkStateOwner*>(self);
    switch (self->state) {
    case 1: {
        UnkPlacedObj* placed = static_cast<UnkPlacedObj*>(g_unk_7101d47310->field_0x498);
        if (!placed || !fn_710118a8d0(placed))
            return;
        fn_7100b2eaac(owner, 2, true, true);
        return;
    }
    case 2:
        fn_7100b2eaac(owner, 1, true, true);
        return;
    }
}
