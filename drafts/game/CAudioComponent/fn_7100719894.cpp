// CAudioComponent's unit: stopping the sound a handle names. The slot playing it fades its sound out
// (fn_7100105a80) and its fader too (fn_7100072328, or fn_7100d7c604 on the owner when it has none),
// then leaves the slot array, and the live sound count drops. Names and layouts are placeholders.
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
void fn_710071f878(UnkAudioSound_ovb0015* self);
struct UnkStruct_710071faf4;
void fn_710071faf4(UnkStruct_710071faf4* self, const UnkStruct_710071faf4* other);

void fn_7100d7c604(void* owner, u64* path);

struct UnkAudioFadeRef_l719894 {
    void* fader;
    s64 serial;
};

// A handle on a fader: current while its serial is the block's.
struct UnkAudioFadeHandle_l719894 {
    UnkAudioFadeRef_l719894* ref;
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
struct UnkAudioStopSound_l719894 {
    u8 pad_0x0[0x28];
    u64 field_0x28;  // the sound's file-path id
    u8 pad_0x30[0x58 - 0x30];
    f32 fade_time;  // 0x58
    u8 pad_0x5c[0x76 - 0x5c];
    bool field_0x76;
    u8 pad_0x77;
    UnkSoundHandle handle;             // 0x78
    UnkAudioFadeHandle_l719894 fader;  // 0x88
    u8 pad_0x98[0xc0 - 0x98];
    bool stopping;  // 0xc0
    u8 pad_0xc1[0xd0 - 0xc1];

    UnkAudioStopSound_l719894(UnkAudioStopSound_l719894&& other) {
        fn_710071faf4(reinterpret_cast<UnkStruct_710071faf4*>(this),
                      reinterpret_cast<const UnkStruct_710071faf4*>(&other));
    }
    ~UnkAudioStopSound_l719894() { fn_710071f878(reinterpret_cast<UnkAudioSound_ovb0015*>(this)); }
};
static_assert(sizeof(UnkAudioStopSound_l719894) == 0xd0, "UnkAudioStopSound_l719894 layout");

struct UnkAudioStopComponent_l719894 {
    u8 pad_0x0[0x30];
    void* owner;  // 0x30
    u8 pad_0x38[0x48 - 0x38];
    base::global::CRntVector<UnkAudioStopSound_l719894> sounds;  // 0x48
};

// Whether a fade time is more than a thousandth away from `other`.
static inline bool Differs(f32 time, f32 other) {
    return time + 0.001f < other || time - 0.001f > other;
}

void fn_7100719894(void* player, UnkSoundHandle handle, f32 time, f32 value) {
    UnkAudioStopComponent_l719894* self = static_cast<UnkAudioStopComponent_l719894*>(player);
    if (!handle.ref || handle.serial == -1 || handle.ref->serial != handle.serial)
        return;
    UnkSoundObject_b50* object = handle.ref->object;
    if (!object)
        return;
    UnkAudioStopSound_l719894 *it = self->sounds.data, *end = it + self->sounds.count;
    f32 fade;
    for (; it != end; ++it) {
        if (Resolve(it->handle) != object)
            continue;
        // A negative time asks for the slot's own.
        fade = time < 0.0f ? it->fade_time : time;
        // Not when it is already fading out over (about) this time.
        if (!it->stopping || !Differs(fade, 0.0f) ||
            Differs(fade, reinterpret_cast<UnkSound*>(it->handle.ref->object)->field_0x100))
            break;
    }
    if (it == end)
        return;
    it->stopping = true;
    fn_7100105a80(reinterpret_cast<UnkSound*>(Resolve(it->handle)), fade, value);
    if (void* fader = it->fader.Get())
        fn_7100072328(fader, fade, value);
    else if (it->field_0x76)
        fn_7100d7c604(self->owner, &it->field_0x28);
    self->sounds.EraseAt(it - self->sounds.data);
    --g_unk_717270f61c;
}
