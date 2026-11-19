// CKraidSpikeMovablePlatformComponent unit. fn_7100608ee8: the base check (fn_710095268c), and when it
// passes and there is a player with a life component (+0x1b8), whether that life component's
// fn_7101161088 is false. Names and layouts are placeholders.
#include "game/CLifeComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkKraidSpikePlatform_l608ee8;

struct UnkKraidSpikePlayer_l608ee8 {
    u8 pad_0x0[0x1b8];
    void* life;  // 0x1b8
};

bool fn_710095268c(UnkKraidSpikePlatform_l608ee8* self);

// NON_MATCHING(functional): Control flow differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
bool fn_7100608ee8(UnkKraidSpikePlatform_l608ee8* self) {
    bool result = fn_710095268c(self);
    if (result) {
        auto* player = reinterpret_cast<UnkKraidSpikePlayer_l608ee8*>(fn_7100dec788(g_unk_7101d47310));
        if (player && player->life)
            result = !fn_7101161088(player->life);
    }
    return result;
}
