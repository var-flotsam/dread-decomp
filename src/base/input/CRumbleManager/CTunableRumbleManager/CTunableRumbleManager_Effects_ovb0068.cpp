// The rumble manager's unit (tools/tu_map.py unit 51, reflected class base::input::CRumbleManager::
// CTunableRumbleManager; the object is base/input/UnkRumbleManager.h's): its active rumble effects
// (the fading objects of src/base/UnkFadeTimer.cpp, 0xa8-byte blocks of the pool at +0x2190, listed
// at +0x2158), the channel mask at +0x2204 that enables them, and the destructor of its map of rumble
// files by path. Names and layouts are placeholders.
#include <string.h>

#include "base/UnkBlockFreeList.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkVector.h"
#include "game/operator_new.h"
#include "types.h"

struct UnkFade;
struct UnkVoicePool_b56;
struct UnkUnit50Object_b49a1;

void fn_71000723cc(UnkFade* fade, f32 time, bool flag);
void fn_7100071dac(UnkUnit50Object_b49a1* self, u32 id);

// An effect as the manager reads it: the channel it plays on.
struct UnkRumbleEffect_b0068 {
    u8 pad_0x0[0x54];
    u32 channel;  // 0x54
};

struct UnkTunableRumbleManager_b0068 {
    u8 pad_0x0[0x2158];
    base::UnkVector<UnkRumbleEffect_b0068*> effects;  // 0x2158
    u8 pad_0x2180[0x10];
    UnkBlockFreeList effect_blocks;  // 0x2190
    u8 pad_0x21b8[0x28];
    u64 free_effects;  // 0x21e0
    u8 pad_0x21e8[0x18];
    u32 next_id;       // 0x2200
    u32 channel_mask;  // 0x2204
};
static_assert(__builtin_offsetof(UnkTunableRumbleManager_b0068, channel_mask) == 0x2204, "layout");

// Enables or disables the given channels, switching every effect on one of them (after 0.2 s).
void fn_7100075020(UnkTunableRumbleManager_b0068* self, bool flag, const u32* channels) {
    u32 mask = self->channel_mask;
    self->channel_mask = !flag ? (mask & ~*channels) : (mask | *channels);
    UnkRumbleEffect_b0068** it = self->effects.data;
    UnkRumbleEffect_b0068** end = it + self->effects.count;
    for (; it != end; ++it) {
        UnkRumbleEffect_b0068* effect = *it;
        u32 bit = 1u << effect->channel;
        if (*channels & bit)
            fn_71000723cc(reinterpret_cast<UnkFade*>(effect), 0.2f, flag);
    }
}

// A new effect from the pool (null when none is free), built with the next id and listed.
UnkRumbleEffect_b0068* fn_7100074b38(UnkTunableRumbleManager_b0068* self) {
    if (!self->free_effects)
        return nullptr;
    --self->free_effects;
    auto* effect = static_cast<UnkRumbleEffect_b0068*>(
        fn_7100083804(reinterpret_cast<UnkVoicePool_b56*>(&self->effect_blocks)));
    if (effect) {
        fn_7100071dac(reinterpret_cast<UnkUnit50Object_b49a1*>(effect), self->next_id++);
        self->effects.PushBack(effect);
    }
    return effect;
}

using UnkRumbleFileKey_b0068 = base::UnkStrIdMemberOf<g_unk_7101d47200>;

// The map's base destructor (base::refl_HashMapClear's copy for these keys).
void fn_7100075aac(char* self);

// The destructor of the map of rumble files by path (base/UnkHashMap.h layout, with its node pool at
// +0x48): marks the pool, releases every node's key and frees the node, then frees the pool's block.
void fn_71000736c0(char* self) {
    *reinterpret_cast<bool*>(self + 0x84) = true;
    if (*reinterpret_cast<u32*>(self + 0xc)) {
        char* node = *reinterpret_cast<char**>(self + 0x40);
        while (node) {
            char* next = *reinterpret_cast<char**>(node + 0x10);
            reinterpret_cast<UnkRumbleFileKey_b0068*>(node)->~UnkRumbleFileKey_b0068();
            (*reinterpret_cast<base::UnkAllocator**>(self))->vf_0x48(node);
            node = next;
        }
        memset(*reinterpret_cast<void**>(self + 0x10), 0, *reinterpret_cast<u32*>(self + 0x1c) * u64(8));
        *reinterpret_cast<u32*>(self + 0xc) = 0;
        *reinterpret_cast<char**>(self + 0x38) = nullptr;
        *reinterpret_cast<char**>(self + 0x40) = nullptr;
    }
    *reinterpret_cast<char**>(self) = nullptr;
    reinterpret_cast<base::UnkBlockPool*>(self + 0x48)->~UnkBlockPool();
    fn_7100075aac(self);
}
