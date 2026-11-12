// CDoorEmmyFXComponent unit (tools/tu_map.py). fn_71007b6be4 adds the unit's key
// (g_unk_717271a598) to the key list at +0x70 (fn_710002fd10) or removes it (fn_710002ffb4), unless
// the player's component at +0x200 has +0x7b9 and +0x7ba both set; +0x120 is cleared, and when the
// list's state (fn_710002fb70) changed the effect is refreshed (fn_71007b5f58). Names and layouts
// are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU39.h"
#include "types.h"

using base::global::CStrId;

// As src/engine/HostArguments.cpp declares it.
struct UnkPlayer;

extern CStrId g_unk_717271a598 __attribute__((visibility("hidden")));

struct UnkEmmyState_b50 {
    u8 pad_0x0[0x7b9];
    bool field_0x7b9;
    bool field_0x7ba;
};

struct UnkEmmyPlayer_b50 {
    u8 pad_0x0[0x200];
    UnkEmmyState_b50* field_0x200;
};

struct UnkDoorEmmyFX_b50 {
    u8 pad_0x0[0x70];
    u8 field_0x70[0xb0];
    u64 field_0x120;
    UnkGuiKeyList* keys() { return reinterpret_cast<UnkGuiKeyList*>(field_0x70); }
};

void fn_71007b5f58(UnkDoorEmmyFX_b50* self, u32 a, u32 b);

void fn_71007b6be4(UnkDoorEmmyFX_b50* self, bool add) {
    UnkEmmyPlayer_b50* player = reinterpret_cast<UnkEmmyPlayer_b50*>(fn_7100dec788(g_unk_7101d47310));
    if (!player)
        return;
    UnkEmmyState_b50* state = player->field_0x200;
    if (!state || (state->field_0x7b9 && state->field_0x7ba))
        return;
    UnkGuiKeyList* list = self->keys();
    self->field_0x120 = 0;
    bool before = fn_710002fb70(list);
    bool after;
    if (add) {
        fn_710002fd10(list, &g_unk_717271a598);
        after = fn_710002fb70(list);
    } else {
        fn_710002ffb4(list, &g_unk_717271a598);
        after = fn_710002fb70(list);
    }
    if (before != after)
        fn_71007b5f58(self, 0, 1);
}
