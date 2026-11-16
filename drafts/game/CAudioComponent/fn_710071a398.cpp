// CAudioComponent's unit (tools/tu_map.py unit 1148): set the volume of the categories a mask selects,
// rescaling the playing sounds of each category to fit. Names and layouts are placeholders.
#include <algorithm>

#include "game/CAudioComponent.h"
#include "game/UnkTU101.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

// As src/game/CAudioComponent/CAudioComponent_b50.cpp defines them.
struct UnkSoundObject_b50;
struct UnkSoundRef {
    UnkSoundObject_b50* object;
    s64 serial;
};

// A playing sound as the volume update reads it: its category and its volume.
struct UnkSoundVolume_l71a398 {
    u8 pad_0x0[0xb4];
    s32 category;  // 0xb4
    u8 pad_0xb8[0x34];
    f32 volume;  // 0xec
};

struct UnkSoundSlot_l71a398 {
    u8 pad_0x0[0x78];
    UnkSoundHandle handle;  // 0x78
    u8 pad_0x88[0x48];
};
static_assert(sizeof(UnkSoundSlot_l71a398) == 0xd0, "UnkSoundSlot_l71a398");

struct UnkAudioVolumes_l71a398 {
    u8 pad_0x0[0x48];
    UnkSoundSlot_l71a398* slots;  // 0x48
    u32 slot_count;               // 0x50
    u8 pad_0x54[0x98];
    f32 volumes[11];  // 0xec, by category
};
static_assert(__builtin_offsetof(UnkAudioVolumes_l71a398, volumes) == 0xec, "volumes");

// The categories, in the order they are updated.
extern const s32 g_unk_710164a8c0[11];

static inline UnkSoundVolume_l71a398* Resolve_l71a398(const UnkSoundHandle* handle) {
    if (handle->ref && handle->ref->serial == handle->serial && handle->serial != -1)
        return reinterpret_cast<UnkSoundVolume_l71a398*>(handle->ref->object);
    return nullptr;
}

static inline bool IsNearZero(f32 value) {
    return value - 1e-5f <= 0.0f && value + 1e-5f >= 0.0f;
}

// Sound volumes stay within [0.001, 100].
static inline f32 ClampVolume(f32 value) {
    return value > 0.001f ? std::min(value, 100.0f) : 0.001f;
}

void fn_710071a398(void* target, u32* mask, f32 volume, f32 time) {
    UnkAudioVolumes_l71a398* self = static_cast<UnkAudioVolumes_l71a398*>(target);
    for (s32 category : g_unk_710164a8c0) {
        if (!(*mask & (1 << category)))
            continue;
        f32& current = self->volumes[category];
        f32 old = current;
        if (old == volume)
            continue;
        f32 scale = IsNearZero(old) ? volume : volume / old;
        for (UnkSoundSlot_l71a398* slot = self->slots; slot != self->slots + self->slot_count; ++slot) {
            UnkSoundVolume_l71a398* sound = Resolve_l71a398(&slot->handle);
            if (!sound || sound->category != category)
                continue;
            f32 v = sound->volume;
            if (v <= old)
                v = scale * v;
            else
                v = v * volume;
            fn_7100105d34(reinterpret_cast<UnkInterp_7100105d34*>(sound), ClampVolume(v), false, time);
        }
        current = volume;
    }
}
