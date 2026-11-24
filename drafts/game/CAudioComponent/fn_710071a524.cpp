// CAudioComponent's unit: stopping every sound slot whose file-path id is `*path`. Each one fades its
// sound out (fn_7100105a80) and its fader too (fn_7100072328, or fn_7100d7c604 on the owner when it has
// none), then leaves the slot array, and the live sound count drops. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/snd/UnkSound.h"
#include "game/CAudioComponent.h"
#include "game/UnkTU50.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

// As src/game/CAudioComponent/CAudioComponent_Sounds_ovb0015.cpp has them.
struct UnkSoundObject_b50;
struct UnkSoundRef {
    UnkSoundObject_b50* object;
    s64 serial;
};

struct UnkAudioSound_ovb0015;
void CAudioComponent_ReleaseSoundSlotIds(UnkAudioSound_ovb0015* self);
struct UnkStruct_710071faf4;
void CAudioComponent_CopyConstructSoundSlot(UnkStruct_710071faf4* self, const UnkStruct_710071faf4* other);

void fn_7100d7c604(void* owner, u64* path);

struct UnkAudioFadeRef_l71a524 {
    void* fader;
    s64 serial;
};

// A handle on a fader: current while its serial is the block's.
struct UnkAudioFadeHandle_l71a524 {
    UnkAudioFadeRef_l71a524* ref;
    s64 serial;

    void* Get() const {
        if (!ref || ref->serial != serial || serial == -1)
            return nullptr;
        return ref->fader;
    }
};

// The sound object a slot's handle names while it is current.
static inline UnkSoundObject_b50* Resolve(const UnkSoundHandle& handle) {
    if (!handle.ref || handle.ref->serial != handle.serial || handle.serial == -1)
        return nullptr;
    return handle.ref->object;
}

// A sound slot of the component (0xd0 bytes): copied with fn_710071faf4, released with fn_710071f878.
struct UnkAudioStopSound_l71a524 {
    u8 pad_0x0[0x28];
    u64 path;  // 0x28: the sound's file-path id
    u8 pad_0x30[0x58 - 0x30];
    f32 fade_time;  // 0x58
    u8 pad_0x5c[0x76 - 0x5c];
    bool field_0x76;
    u8 pad_0x77;
    UnkSoundHandle handle;             // 0x78
    UnkAudioFadeHandle_l71a524 fader;  // 0x88
    u8 pad_0x98[0xc0 - 0x98];
    u8 stopping;  // 0xc0: set once the slot is told to stop
    u8 pad_0xc1[0xd0 - 0xc1];

    UnkAudioStopSound_l71a524(UnkAudioStopSound_l71a524&& other) {
        CAudioComponent_CopyConstructSoundSlot(reinterpret_cast<UnkStruct_710071faf4*>(this),
                      reinterpret_cast<const UnkStruct_710071faf4*>(&other));
    }
    ~UnkAudioStopSound_l71a524() { CAudioComponent_ReleaseSoundSlotIds(reinterpret_cast<UnkAudioSound_ovb0015*>(this)); }
};
static_assert(sizeof(UnkAudioStopSound_l71a524) == 0xd0, "UnkAudioStopSound_l71a524 layout");

struct UnkAudioStopComponent_l71a524 {
    u8 pad_0x0[0x30];
    void* owner;  // 0x30
    u8 pad_0x38[0x48 - 0x38];
    base::global::CRntVector<UnkAudioStopSound_l71a524> sounds;  // 0x48
};

// Whether a time is within a thousandth of `other`.
static inline bool IsNearly(f32 time, f32 other) {
    return time + 0.001f >= other && time - 0.001f <= other;
}

void fn_710071a524(UnkEntitySoundComponent* component, void* path, f32 time, f32 value) {
    UnkAudioStopComponent_l71a524* self = reinterpret_cast<UnkAudioStopComponent_l71a524*>(component);
    const u64* key = static_cast<const u64*>(path);
    UnkAudioStopSound_l71a524* it = self->sounds.data;
    while (it != self->sounds.data + self->sounds.count) {
        if (it->path != *key) {
            ++it;
            continue;
        }
        UnkSoundObject_b50* sound = Resolve(it->handle);
        if (!sound) {
            it->stopping = 1;
            ++it;
            continue;
        }
        // A negative time asks for the slot's own.
        f32 fade = time < 0.0f ? it->fade_time : time;
        // Already fading out over (about) this time: leave it.
        if (it->stopping && !IsNearly(fade, 0.0f) &&
            IsNearly(fade, reinterpret_cast<UnkSound*>(sound)->field_0x100)) {
            ++it;
            continue;
        }
        // A fade over no time stops it at once.
        bool at_once = fade - 0.001f <= 0.0f && fade + 0.001f >= 0.0f;
        it->stopping = 1;
        UnkSound* playing = reinterpret_cast<UnkSound*>(Resolve(it->handle));
        if (at_once)
            fn_7100105a80(playing, fade, 0.0f);
        else
            fn_7100105a80(playing, fade, value);
        if (void* fader = it->fader.Get())
            fn_7100072328(fader, fade, 0.0f);
        else if (it->field_0x76)
            fn_7100d7c604(self->owner, &it->path);
        u32 index = it - self->sounds.data;
        self->sounds.EraseAt(index);
        --g_unk_717270f61c;
        it = self->sounds.data + index;
    }
}
