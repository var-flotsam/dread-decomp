// CAudioComponent unit (tools/tu_map.py): keeps a sound entry bound to its playing sound, restarts the
// preset's fade when its target was reached (fn_71001060e8), and stops the sound once the component
// is out of the preset's range (fn_7100105034). Names and layouts are placeholders.
#include <math.h>

#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU101.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkSoundObject_b50;
struct UnkSoundRef {
    UnkSoundObject_b50* object;
    s64 serial;
};

void fn_71001060e8(UnkSoundObject_b50* sound, f32 a, f32 b, f32 c);

// What this function reads of a playing sound.
struct UnkSoundObjectView_l71f208 {
    u8 pad_0x0[0x88];
    u64 field_0x88;
    u8 pad_0x90[0x8];
    f32 x, y, z;
};

inline const UnkSoundObjectView_l71f208* View_l71f208(const UnkSoundObject_b50* sound) {
    return reinterpret_cast<const UnkSoundObjectView_l71f208*>(sound);
}

// The object a handle names while it is still the one the handle was made for.
inline UnkSoundObject_b50* Resolve_l71f208(const UnkSoundHandle* handle) {
    if (handle->ref && handle->ref->serial == handle->serial && handle->serial != -1)
        return handle->ref->object;
    return nullptr;
}

// The same, with the checks in the order the sound manager's callers make them.
inline UnkSoundObject_b50* GetSoundObject_l71f208(const UnkSoundHandle* handle) {
    if (handle->ref && handle->serial != -1 && handle->ref->serial == handle->serial)
        return handle->ref->object;
    return nullptr;
}

// The audio preset an entry plays (sound::CAudioPreset).
struct UnkAudioPreset_l71f208 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x50];
    u64 field_0x58;
    u8 pad_0x60[0x514];
    u32 field_0x574;
    u8 pad_0x578[0x8];
    f32 field_0x580;
    f32 field_0x584;
    u8 pad_0x588[0x2c];
    f32 field_0x5b4;
    f32 field_0x5b8;
    f32 field_0x5bc;
};
static_assert(__builtin_offsetof(UnkAudioPreset_l71f208, field_0x5bc) == 0x5bc, "layout");

struct UnkAudioEntry_l71f208 {
    UnkSoundHandle handle;
    u8 pad_0x10[0x80];
    CStrId id;
    CFilePathStrId path;
    UnkAudioPreset_l71f208* preset;
    u8 field_0xa8;
    u8 field_0xa9;
};
static_assert(__builtin_offsetof(UnkAudioEntry_l71f208, preset) == 0xa0, "layout");

struct UnkAudioSlot_l71f208 {
    u8 pad_0x0[0x78];
    UnkSoundHandle handle;
    u8 pad_0x88[0x48];
};
static_assert(sizeof(UnkAudioSlot_l71f208) == 0xd0, "layout");

// A ramp from one value to another over a duration, and the time spent on it so far.
struct UnkFadeRamp_l71f208 {
    f32 from;
    f32 to;
    f32 duration;
    f32 elapsed;

    UnkFadeRamp_l71f208(f32 from, f32 to, f32 duration) : from(from), to(to), duration(duration), elapsed(0.0f) {}
};

struct UnkFade_l71f208 {
    f32 field_0x0;
    f32 field_0x4;
    UnkFadeRamp_l71f208 ramp;
};

struct UnkVec3_l71f208 {
    f32 x, y, z;
};

struct UnkAudioComponent_l71f208 {
    u8 pad_0x0[0x30];
    void* field_0x30;
    u8 pad_0x38[0x10];
    UnkAudioSlot_l71f208* slots;
    u32 slotCount;
    u8 pad_0x54[0x44];
    UnkFade_l71f208* fades;
    const UnkAudioPreset_l71f208** fadeKeys;
    u32 fadeCount;
    u8 pad_0xac[0x14];
    bool field_0xc0;
    UnkVec3_l71f208 position;

    UnkFade_l71f208* FindFade(const UnkAudioPreset_l71f208* preset) {
        for (u64 i = 0; i != fadeCount; ++i) {
            if (fadeKeys[i] == preset)
                return &fades[i];
        }
        return &fades[fadeCount];
    }
};
static_assert(__builtin_offsetof(UnkAudioComponent_l71f208, position) == 0xc4, "layout");

inline bool InRange_l71f208(const UnkVec3_l71f208& at, const UnkSoundObjectView_l71f208* sound, f32 radius) {
    if (fabsf(at.x - sound->x) > radius || fabsf(at.y - sound->y) > radius)
        return false;
    f32 dx = sound->x - at.x;
    f32 dy = sound->y - at.y;
    f32 dz = sound->z - at.z;
    return dx * dx + dy * dy + dz * dz <= radius * radius;
}

void fn_710071f208(UnkAudioComponent_l71f208* self, UnkAudioEntry_l71f208* entry) {
    if (entry->id.data == &g_unk_7101d47108->empty || entry->path.data == &g_unk_7101d47200->empty)
        return;
    UnkAudioPreset_l71f208* preset = entry->preset;
    if (!preset || !fn_710009d50c(preset->vf_0x0(), fn_710102df18()))
        return;

    UnkSoundHandle handle = entry->handle;
    UnkSoundObject_b50* sound = GetSoundObject_l71f208(&handle);
    if (!sound) {
        // Not bound yet: take the handle of the component's sound that plays this preset.
        for (u32 i = 0; i != self->slotCount; ++i) {
            UnkAudioSlot_l71f208* slot = &self->slots[i];
            UnkSoundObject_b50* other = Resolve_l71f208(&slot->handle);
            if (other && View_l71f208(other)->field_0x88 == preset->field_0x58) {
                entry->handle = slot->handle;
                return;
            }
        }
        return;
    }

    if (self->field_0xc0) {
        UnkFade_l71f208* fade = self->FindFade(entry->preset);
        if (fade != &self->fades[self->fadeCount] && preset->field_0x5b8 > 0.0f &&
            fade->field_0x0 > preset->field_0x5b8) {
            f32 start = fade->field_0x4;
            if (start - 1e-5f <= fade->ramp.to && start + 1e-5f >= fade->ramp.to) {
                fade->ramp = UnkFadeRamp_l71f208(start, preset->field_0x5b4, preset->field_0x5bc);
                fn_71001060e8(Resolve_l71f208(&handle), preset->field_0x5b4, preset->field_0x5bc, 1.0f);
            } else {
                fn_71001060e8(sound, fade->ramp.to, fade->ramp.duration - fade->ramp.elapsed, start);
            }
        }
    }

    if (preset->field_0x574 == 1 && preset->field_0x580 > preset->field_0x584) {
        f32 radius = preset->field_0x580;
        const UnkSoundObjectView_l71f208* playing = View_l71f208(Resolve_l71f208(&entry->handle));
        if (!self->field_0x30 || !InRange_l71f208(self->position, playing, radius)) {
            if (UnkSoundObject_b50* stopped = Resolve_l71f208(&entry->handle)) {
                fn_7100105034(stopped);
                entry->field_0xa9 = 1;
                entry->handle = g_unk_7108b6c460;
                entry->field_0xa8 = 0;
            }
        }
    }
}
