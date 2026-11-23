// sound::CMusicManager::CTunableMusicManager's unit: releases one hold of the active requests (or all of them
// when forced); when the last hold goes, every request with a file path is let go again. Names and layouts
// are placeholders (shared with CTunableMusicManager_Requests_ovb0019.cpp).
#include <algorithm>

#include "base/global/CRntVector.h"
#include "base/snd/CSoundSystem.h"
#include "game/CChozoCommanderAIComponent.h"
#include "game/UnkEntryArray_b0019.h"
#include "game/UnkMusicEntry_b0019.h"
#include "game/UnkMusicMgr_b0019.h"
#include "game/UnkTU23.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "game/sound/UnkMusicPathVec.h"
#include "types.h"

struct UnkTuneStr_b0019;

static inline void SetAllHeld(UnkMusicMgr_b0019* self, bool held) {
    for (u64 i = 0; i < self->slot_count; ++i) {
        if (self->entries[i].paths.count == 0)
            continue;
        fn_7100108a98(g_unk_7101d4d820, (u64)&PathAt(self->entries[i].paths, 0), held, 0.2f);
    }
}

// Releases a hold taken by fn_7100fdba30; forcing drops every hold at once.
void fn_7100fdbbd0(UnkMusicMgr_b0019* self, bool force) {
    u32 left = std::max(self->hold_count, 1);
    --left;
    self->hold_count = force ? 0 : left;
    if (left != 0 && !force)
        return;
    SetAllHeld(self, false);
}
