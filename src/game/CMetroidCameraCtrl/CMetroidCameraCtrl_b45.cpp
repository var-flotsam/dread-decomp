// CMetroidCameraCtrl's unit (tools/tu_map.py unit 1424): the value at +0x18 of the player's
// character class (+0x220) record for the class fn_7100c56e7c names, else -1. Names are
// placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkPlayer;

struct UnkCameraPlayer_b45a2 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* cls;
};

struct UnkTrackRecordValue2_b45a2 {
    u8 pad_0x0[0x18];
    f32 value;
};

f32 fn_71009359dc() {
    auto* player = reinterpret_cast<UnkCameraPlayer_b45a2*>(fn_7100dec788(g_unk_7101d47310));
    if (player) {
        UnkTrackQuery query = {fn_7100c56e7c(), nullptr, -1.0f};
        if (fn_710117b08c(player->cls, &query))
            return reinterpret_cast<UnkTrackRecordValue2_b45a2*>(query.result)->value;
    }
    return -1.0f;
}
