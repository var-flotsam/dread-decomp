// sound::CMusicManager::CTunableMusicManager's unit: releases one hold of the active requests (or all of them
// when forced); when the last hold goes, every request with a file path is let go again. Names and layouts
// are placeholders (shared with CTunableMusicManager_Requests_ovb0019.cpp).
#include <algorithm>

#include "base/global/CRntVector.h"
#include "base/snd/CSoundSystem.h"
#include "game/CChozoCommanderAIComponent.h"
#include "game/UnkTU23.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "game/sound/UnkMusicPathVec.h"
#include "types.h"

struct UnkTuneStr_b0019;

struct UnkMusicEntry_b0019 {
    u8 pad_0x0[0x28];
    UnkPathVec_b0019 paths;  // 0x28
    u8 pad_0x50[0x8];
    UnkTuneStr_b0019* name;  // 0x58
};

struct UnkEntryArray_b0019 {
    UnkMusicEntry_b0019* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool autoGrow;

    UnkMusicEntry_b0019& operator[](u64 i) {
        if (autoGrow && i >= count) {
            u32 n = capacity;
            if (i >= capacity && step != 0)
                n = ((u32)i / step + 1) * step;
            fn_7100fddcb4(reinterpret_cast<base::global::CRntVector<UnkMusicEntry_0x60>*>(this), n);
            fn_7100fde4d8(reinterpret_cast<UnkArray*>(this), i + 1);
        }
        return data[i];
    }
};

struct UnkMusicMgr_b0019 {
    u8 pad_0x0[0xaa];
    bool enabled;  // 0xaa
    u8 pad_0xab[0x1];
    s32 hold_count;  // 0xac
    u8 pad_0xb0[0x8];
    UnkEntryArray_b0019 entries;  // 0xb8
    u8 pad_0xd0[0x180];
    u32 slot_count;  // 0x250
};

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
