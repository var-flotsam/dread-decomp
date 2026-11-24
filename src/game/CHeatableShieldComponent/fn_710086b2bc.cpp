// CHeatableShieldComponent's unit (tools/tu_map.py, unit 1322): for every shield entry (0xd0 bytes) whose sound
// handle (+0xb8) still names a live sound, has the owner (+0x30) stop the shield sound (fn_7100d7c48c, no fade)
// and empties the handle. Names and layouts are placeholders.
#include "game/UnkSoundRef.h"
#include "game/UnkTU101.h"
#include "game/UnkTU2523.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"


// As src/game/CHeatableShieldComponent/CHeatableShieldEntry_Reset_A5_b71.cpp views it.
// The sound the handle names while it is current.
static inline UnkSound* SoundOf(const UnkSoundHandle& handle) {
    if (handle.ref && handle.ref->serial == handle.serial && handle.serial != -1)
        return handle.ref->object;
    return nullptr;
}

struct UnkShieldSoundName_l86b2bc;
__attribute__((visibility("hidden"))) extern UnkShieldSoundName_l86b2bc g_unk_7172729398;

struct UnkShieldEntry_l86b2bc {
    u8 pad_0x0[0xb8];
    UnkSoundHandle sound;  // 0xb8
    u8 pad_0xc8[0x8];
};
static_assert(sizeof(UnkShieldEntry_l86b2bc) == 0xd0, "UnkShieldEntry_l86b2bc layout");

struct UnkHeatableShieldComponent_l86b2bc {
    u8 pad_0x0[0x30];
    void* owner;  // 0x30
    u8 pad_0x38[0x8];
    UnkShieldEntry_l86b2bc* entries;  // 0x40
    u8 pad_0x48[0x8];
    u32 count;  // 0x50
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_710086b2bc(UnkHeatableShieldComponent_l86b2bc* self) {
    for (UnkShieldEntry_l86b2bc *it = self->entries, *end = it + self->count; it != end; ++it) {
        if (!SoundOf(it->sound))
            continue;
        fn_7100d7c48c(self->owner, &g_unk_7172729398, 0.0f, 0.0f);
        it->sound = g_unk_7108b6c460;
    }
}
