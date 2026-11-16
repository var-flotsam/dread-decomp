// CAudioComponent's unit: stopping the sound slots a set of kinds selects. Each selected slot that is
// not stopping yet fades its sound out over its own time (fn_7100105a80, or fn_7100105034 at once) and
// its fader too (fn_7100072328, or fn_7100d7c604 on the owner when it has none); the live sound count
// drops for each. Names and layouts are placeholders.
#include "base/snd/UnkSound.h"
#include "game/CAudioComponent.h"
#include "game/UnkTU101.h"
#include "game/UnkTU50.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

// As src/game/CAudioComponent/CAudioComponent_Sounds_ovb0015.cpp has them.
struct UnkSoundObject_b50;
struct UnkSoundRef {
    UnkSoundObject_b50* object;
    s64 serial;
};

void fn_7100d7c604(void* owner, u64* path);

struct UnkAudioFadeRef_l719064 {
    void* fader;
    s64 serial;
};

// A handle on a fader: current while its serial is the block's.
struct UnkAudioFadeHandle_l719064 {
    UnkAudioFadeRef_l719064* ref;
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

// A sound slot of the component (0xd0 bytes).
struct UnkAudioStopSound_l719064 {
    u8 pad_0x0[0x28];
    u64 path;  // 0x28: the sound's file-path id
    u8 pad_0x30[0x58 - 0x30];
    f32 fade_time;  // 0x58
    u8 pad_0x5c[0x76 - 0x5c];
    bool field_0x76;
    u8 pad_0x77;
    UnkSoundHandle handle;             // 0x78
    UnkAudioFadeHandle_l719064 fader;  // 0x88
    u8 pad_0x98[0xc0 - 0x98];
    u8 stopping;  // 0xc0: set once the slot is told to stop
    u8 pad_0xc1[0xc4 - 0xc1];
    u32 field_0xc4_0 : 1;
    u32 restart : 1;  // 0xc4 bit 1
    u32 kind_a : 1;   // 0xc4 bit 2
    u32 kind_b : 1;   // 0xc4 bit 3
    u8 pad_0xc8[0xd0 - 0xc8];
};
static_assert(sizeof(UnkAudioStopSound_l719064) == 0xd0, "UnkAudioStopSound_l719064 layout");

struct UnkAudioStopComponent_l719064 {
    u8 pad_0x0[0x30];
    void* owner;  // 0x30
    u8 pad_0x38[0x48 - 0x38];
    UnkAudioStopSound_l719064* sounds;  // 0x48
    u32 count;                          // 0x50
};

void fn_7100719064(UnkEntitySoundComponent* component, bool a, bool b, bool c, bool d) {
    UnkAudioStopComponent_l719064* self = reinterpret_cast<UnkAudioStopComponent_l719064*>(component);
    if (!c && !d)
        return;
    for (UnkAudioStopSound_l719064* it = self->sounds; it != self->sounds + self->count; ++it) {
        if (it->stopping)
            continue;
        // With both, every slot; otherwise slots to restart go by `c`, the others by `d`.
        if (!(c && d) && (it->restart ? !c : !d))
            continue;
        // Asked for a kind: only the slots of it, and those of kind a.
        bool wanted = (a && it->kind_a) || (b && it->kind_b);
        if (!wanted && (a || b) && !it->kind_a)
            continue;
        if (UnkSoundObject_b50* sound = Resolve(it->handle)) {
            if (it->fade_time <= 0.0f)
                fn_7100105034(sound);
            else
                fn_7100105a80(reinterpret_cast<UnkSound*>(sound), it->fade_time, 0.0f);
            if (void* fader = it->fader.Get())
                fn_7100072328(fader, it->fade_time, 0.0f);
            else if (it->field_0x76)
                fn_7100d7c604(self->owner, &it->path);
        }
        it->stopping = 1;
        --g_unk_717270f61c;
    }
}
