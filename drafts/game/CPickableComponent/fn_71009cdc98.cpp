// CPickableComponent unit (tools/tu_map.py): whether `actor` may pick the item up. Only the player
// (fn_7100dec788 of the game manager) may, and not while the component at +0x1b8, when it has one,
// is in state 2. Names and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkPickerState_l9cdc98 {
    u8 pad_0x0[0x70];
    s32 state;  // 0x70
};

struct UnkPicker_l9cdc98 {
    u8 pad_0x0[0x1b8];
    UnkPickerState_l9cdc98* field_0x1b8;
};

bool fn_71009cdc98(void* self, UnkPicker_l9cdc98* actor) {
    UnkPlayer* player = fn_7100dec788(g_unk_7101d47310);
    bool can_pick = reinterpret_cast<UnkPicker_l9cdc98*>(player) == actor;
    if (can_pick && actor->field_0x1b8)
        can_pick = actor->field_0x1b8->state != 2;
    return can_pick;
}
