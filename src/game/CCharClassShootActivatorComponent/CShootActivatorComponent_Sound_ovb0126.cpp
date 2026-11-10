// CShootActivatorComponent (unit of CCharClassShootActivatorComponent, tools/tu_map.py): fn_71008923cc sets
// the activator's sound (+0x468) from a charge amount: while it plays, the flag at +0x570 is cleared and its
// pitch and volume go from the tunables' minimums (CTunableShootActivatorComponent's fPitchMin and fVolumeMin,
// g_unk_717272c450) up by the amount times the maximums at +0x4c8 and +0x4cc, clamped to the maximums, over
// 0.2 (fn_7100105da8, fn_7100105d34); otherwise a positive amount has the owner (+0x30) play the sound named at
// +0x450 (fn_7100d7c45c). Names and layouts are placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "types.h"

using base::reflection::UnkTunable;

extern UnkTunable g_unk_717272c450;

struct UnkInterp_7100105d34;
void fn_7100105d34(UnkInterp_7100105d34* self, f32 value, bool relative, f32 time);
void fn_7100105da8(UnkInterp_7100105d34* self, f32 value, f32 time);

struct UnkItemLifeUpdate_ovb0114;
void fn_71008e1174(UnkItemLifeUpdate_ovb0114* self, f32 dt);
struct UnkActor;
struct UnkStrIdPathPair;
void fn_7100d7c548(UnkActor* self, const UnkStrIdPathPair* ids, f32 blend, f32 start);

struct UnkSoundHandle;
struct UnkEntityWithSound;
UnkSoundHandle fn_7100d7c45c(UnkEntityWithSound* self, void* a, void* b, void* c);

// CShootActivatorComponent::CTunableShootActivatorComponent's fields (reflection).
struct UnkShootTunable_ovb0126 {
    u8 pad_0x0[0x24];
    f32 volume_min;               // fVolumeMin
    f32 decrease_volume_factor;   // fDecreaseVolumeFactor
    f32 pitch_min;                // fPitchMin
};

// A sound handle as src/game/CEntity_2.cpp returns it: the sound's record (its own serial at +8) and the
// serial it had.
struct UnkSoundRef_ovb0126 {
    UnkInterp_7100105d34* sound;
    s64 serial;
};
struct UnkSoundHandle_ovb0126 {
    UnkSoundRef_ovb0126* ref;
    s64 serial;

    bool Playing() const { return ref && ref->serial == serial && serial != -1 && ref->sound; }
    UnkInterp_7100105d34* Get() const { return ref && ref->serial == serial && serial != -1 ? ref->sound : nullptr; }
};

struct UnkShootActivator_ovb0126;
struct UnkShootActivatorVtable_ovb0126 {
    void* unk_0x0[0x1f0 / 8];
    void (*vf_0x1f0)(UnkShootActivator_ovb0126* self);
    void* unk_0x1f8[4];
    void (*vf_0x218)(UnkShootActivator_ovb0126* self);
};

struct UnkShootActivator_ovb0126 {
    UnkShootActivatorVtable_ovb0126* vtable;
    u8 pad_0x8[0x28];
    UnkEntityWithSound* owner;
    u8 pad_0x38[0x450 - 0x38];
    u8 sound_name[0x18];
    UnkSoundHandle_ovb0126 sound;  // 0x468
    u8 pad_0x478[0x4b0 - 0x478];
    f32 duration;  // 0x4b0
    u8 pad_0x4b4[8];
    f32 timer;  // 0x4bc
    u8 pad_0x4c0[8];
    f32 pitch_max;   // 0x4c8
    f32 volume_max;  // 0x4cc
    u8 pad_0x4d0[0x570 - 0x4d0];
    bool field_0x570;
    f32 fade;  // 0x574
};

// v clamped to [min, max] (max first).
static inline f32 Clamp(f32 v, f32 min, f32 max) {
    f32 r = v >= max ? max : v;
    if (v <= min)
        return min;
    return r;
}

static inline UnkShootTunable_ovb0126& Tunable() {
    return *reinterpret_cast<UnkShootTunable_ovb0126*>(&g_unk_717272c450);
}

static inline f32 Ramp(f32 min, f32 max, f32 amount) {
    return Clamp(min + max * amount, min, max);
}

void fn_71008923cc(UnkShootActivator_ovb0126* self, f32 amount) {
    if (self->sound.Playing()) {
        __builtin_memset(&self->field_0x570, 0, 1);
        auto* tunable = reinterpret_cast<UnkShootTunable_ovb0126*>(&g_unk_717272c450);
        f32 pitch = Ramp(tunable->pitch_min, self->pitch_max, amount);
        f32 volume = Ramp(tunable->volume_min, self->volume_max, amount);
        fn_7100105da8(self->sound.Get(), pitch, 0.2f);
        fn_7100105d34(self->sound.Get(), volume, false, 0.2f);
    } else if (amount > 0.0f) {
        using Play = UnkSoundHandle_ovb0126 (*)(UnkEntityWithSound*, void*, void*, bool);
        self->sound = reinterpret_cast<Play>(&fn_7100d7c45c)(self->owner, self->sound_name, nullptr, false);
    }
}

