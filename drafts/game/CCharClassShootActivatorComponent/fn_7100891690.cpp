// CShootActivatorComponent (unit of CCharClassShootActivatorComponent, tools/tu_map.py): the update. After the
// base update, runs the activation timer (+0x4bc) down to zero within the duration (+0x4b0), calling slot 0x1f0
// while it runs and 0x218 once it reaches the duration; while the sound fades (+0x570) lowers its volume with the
// remaining share, and stops it on the owner when the fade is over. Names and layouts are placeholders.
#include "base/reflection/ReflectedAttributes.h"
#include "game/CCharClassShootActivatorComponent.h"
#include "game/UnkTU101.h"
#include "game/UnkTU2523.h"
#include "types.h"

struct UnkInterp_7100105d34;

struct UnkItemLifeUpdate_ovb0114;
void fn_71008e1174(UnkItemLifeUpdate_ovb0114* self, f32 dt);
struct UnkActor;
struct UnkStrIdPathPair;

struct UnkSoundHandle;
struct UnkEntityWithSound;

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

void fn_7100891690(UnkShootActivator_ovb0126* self, f32 dt) {
    fn_71008e1174(reinterpret_cast<UnkItemLifeUpdate_ovb0114*>(self), dt);
    f32 timer = self->timer;
    f32 duration = self->duration;
    if (timer > 0.0f && timer < duration) {
        self->timer = Clamp(timer - dt, 0.0f, duration);
        self->vtable->vf_0x1f0(self);
    } else if (timer >= duration) {
        self->vtable->vf_0x218(self);
    }
    if (!self->field_0x570)
        return;
    f32 fade = self->fade - dt;
    self->fade = fade;
    f32 share = fade / self->duration;
    f32 level = !(share <= 0.0f) ? (!(share >= 1.0f) ? share : 1.0f) : 0.0f;
    f32 scaled = self->volume_max * level;
    auto* tunable = reinterpret_cast<UnkShootTunable_ovb0126*>(&g_unk_717272c450);
    f32 volume = Clamp(tunable->decrease_volume_factor * scaled, 0.0f, self->volume_max);
    if (self->sound.Playing())
        fn_7100105d34(self->sound.Get(), volume, false, 0.2f);
    if (level <= 0.0f) {
        fn_7100d7c548(reinterpret_cast<UnkActor*>(self->owner),
                      reinterpret_cast<const UnkStrIdPathPair*>(self->sound_name), 0.0f, 0.0f);
        self->field_0x570 = false;
    }
}
