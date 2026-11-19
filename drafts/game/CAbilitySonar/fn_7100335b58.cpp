// CAbilitySonar's unit (tools/tu_map.py unit 434): the sonar's update. Pays the energy cost, follows the
// owner's position, grows the pulse along its curve and places its model, gathers the objects it reaches
// (nearest first), fades the sounds in and updates each pulse while the fade runs, and stops once the
// active time is over. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CSpecialEnergyComponent.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU101.h"
#include "game/UnkTU133.h"
#include "game/UnkTU284.h"
#include "game/UnkTU53.h"
#include "game/UnkTU55.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkSonarCurve_l335b58 {
    u8 pad_0x0[0x10];
    const char* name;  // 0x10
    u8 pad_0x18[0x2c];
    u32 uses;  // 0x44
};

struct UnkSonarTunable_l335b58 {
    u8 pad_0x0[0x28];
    f32 fade_time;   // 0x28
    f32 max_radius;  // 0x2c
    u8 pad_0x30[0x48];
    f32 grow_time;                 // 0x78
    u8 pad_0x7c[4];
    UnkSonarCurve_l335b58* curve;  // 0x80
};
extern UnkSonarTunable_l335b58 g_unk_71726b0c48;
extern const char* g_unk_7101a235b8;

struct UnkSonarPos_l335b58 {
    f32 x;
    f32 y;
};

struct UnkSonarPulse_l335b58 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Update(const UnkSonarPos_l335b58* center, f32 radius, f32 fade);  // 0x10
};

struct UnkSonarTarget_l335b58;

// The objects the pulse reaches, nearest first.
struct __attribute__((packed, aligned(4))) UnkSonarHits_l335b58 {
    u32 count;
    UnkSonarTarget_l335b58* items[64];
};

// The pulses running (as the hit list, 4-byte aligned).
struct __attribute__((packed, aligned(4))) UnkSonarPulses_l335b58 {
    u32 count;
    UnkSonarPulse_l335b58* items[32];
};

struct UnkSonarSound_l335b58 {
    UnkInterp_7100105d34* sound;
    u64 id;
};

struct UnkSonarSoundRef_l335b58 {
    UnkSonarSound_l335b58* handle;
    u64 handle_id;
};

struct UnkSonarModel_l335b58 {
    u8 pad_0x0[0x28];
    UnkXformNode node;  // 0x28
};

struct UnkSonarEnergyHolder_l335b58 {
    u8 pad_0x0[0x10];
    UnkTarget429* energy;  // 0x10
};

struct UnkSonarPart_l335b58 {
    u8 pad_0x0[0xe8];
    UnkSonarEnergyHolder_l335b58* energy_holder;  // 0xe8
};

struct UnkSonarActor_l335b58 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkSonarOwner_l335b58 {
    u8 pad_0x0[0x30];
    UnkSonarActor_l335b58* actor;  // 0x30
    u8 pad_0x38[0xb0];
    UnkSonarEnergyHolder_l335b58* energy_holder;  // 0xe8
};

struct UnkAbilitySonar_l335b58;
struct UnkSonarNearer_l335b58 {
    const UnkSonarPos_l335b58* target;
};
void fn_7100335eb8(UnkAbilitySonar_l335b58* self, UnkSonarPulses_l335b58* pulses, UnkSonarHits_l335b58* hits);
void fn_7100336128(UnkAbilitySonar_l335b58* self, UnkSonarPulses_l335b58* pulses, UnkSonarHits_l335b58* hits);
void fn_7100338620(UnkSonarTarget_l335b58** first, UnkSonarTarget_l335b58** last, UnkSonarNearer_l335b58* nearer);

struct UnkAbilitySonar_l335b58 {
    void** vtable;
    u8 pad_0x8[0xb8];
    UnkSonarOwner_l335b58* owner;  // 0xc0
    u8 pad_0xc8[0x390 - 0xc8];
    UnkSonarModel_l335b58* model;  // 0x390
    UnkSwitchObject* field_0x398;  // 0x398
    u8 pad_0x3a0[0x3cc - 0x3a0];
    f32 active_time;  // 0x3cc
    f32 fade_left;    // 0x3d0
    UnkSonarPulses_l335b58 pulses;  // 0x3d4
    UnkSonarHits_l335b58 hits;      // 0x4d8
    u8 pad_0x6dc[4];
    UnkSonarPos_l335b58 position;   // 0x6e0
    UnkSonarPos_l335b58 last_position;  // 0x6e8
    f32 radius;  // 0x6f0
    f32 grow;    // 0x6f4
    u8 pad_0x6f8[0x790 - 0x6f8];
    UnkSonarSoundRef_l335b58 sound_0x790;
    UnkSonarSoundRef_l335b58 sound_0x7a0;

    f32 EnergyCost() { return reinterpret_cast<f32 (*)(UnkAbilitySonar_l335b58*)>(vtable[0xc0 / 8])(this); }
    void SetActive(bool on) { reinterpret_cast<void (*)(UnkAbilitySonar_l335b58*, bool)>(vtable[0xa0 / 8])(this, on); }
};
static_assert(__builtin_offsetof(UnkAbilitySonar_l335b58, hits) == 0x4d8, "layout");
static_assert(__builtin_offsetof(UnkAbilitySonar_l335b58, position) == 0x6e0, "layout");
static_assert(__builtin_offsetof(UnkAbilitySonar_l335b58, sound_0x7a0) == 0x7a0, "layout");

static inline void FadeTo(const UnkSonarSoundRef_l335b58& ref, f32 volume) {
    UnkSonarSound_l335b58* handle = ref.handle;
    if (handle && handle->id == ref.handle_id && ref.handle_id != ~0ull && handle->sound)
        fn_7100105d34(handle->sound, volume, false, 0.2f);
}

static inline bool IsEqual(f32 a, f32 b, f32 tolerance) {
    return a - tolerance <= b && a + tolerance >= b;
}

void fn_7100335b58(UnkAbilitySonar_l335b58* self, f32 dt) {
    UnkSonarOwner_l335b58* owner = self->owner;
    if (owner->energy_holder) {
        if (UnkTarget429* energy = owner->energy_holder->energy)
            fn_7100a1feac(energy, self->EnergyCost());
    }
    {
        UnkSonarActor_l335b58* actor = self->owner->actor;
        UnkVec3f actor_position(actor->x, actor->y, actor->z);
        fn_710007b444(&self->position, reinterpret_cast<const UnkVec3*>(&actor_position));
    }

    UnkSonarTunable_l335b58& tunable = g_unk_71726b0c48;
    f32 grow = self->grow + dt;
    f32 grow_time = tunable.grow_time;
    grow = grow < grow_time ? grow : grow_time;
    self->grow = grow;
    f32 t = grow / grow_time;
    f32 max_radius = tunable.max_radius;
    ++tunable.curve->uses;
    const char* curve = tunable.curve->name ? tunable.curve->name : g_unk_710162e2ed;
    f32 radius = max_radius * fn_7100076020(t, curve);
    self->radius = radius;
    if (radius <= tunable.max_radius * 0.01f) {
        self->last_position.x = self->position.x;
        self->last_position.y = self->position.y;
    }
    if (UnkSonarModel_l335b58* model = self->model) {
        if (radius >= tunable.max_radius) {
            fn_7100145e94(reinterpret_cast<UnkSwitchObject*>(model), false);
        } else {
            {
                UnkVec3f at;
                at.x = self->last_position.x;
                at.y = self->last_position.y;
                at.z = 0.0f;
                fn_7100145f28(reinterpret_cast<UnkSwitchObject*>(model), &at);
            }
            f32 scale = self->radius * 0.01f;
            UnkXformNode* node = &self->model->node;
            UnkVec3f scales(scale, scale, scale);
            fn_7100207be8(node, &scales);
        }
    }

    self->hits.count = 0;
    fn_7100335eb8(self, &self->pulses, &self->hits);
    {
        UnkSonarNearer_l335b58 nearer = {&self->position};
        fn_7100338620(self->hits.items, self->hits.items + self->hits.count, &nearer);
    }
    fn_7100336128(self, &self->pulses, &self->hits);

    if (self->fade_left > 0.0f) {
        self->fade_left -= dt;
        if (self->fade_left > 0.0f) {
            f32 fade = fn_7100076020(1.0f - self->fade_left / tunable.fade_time, g_unk_7101a235b8);
            FadeTo(self->sound_0x790, fade);
            FadeTo(self->sound_0x7a0, fade);
            const UnkSonarPos_l335b58* center =
                IsEqual(self->radius, tunable.max_radius, 0.001f) ? &self->position : &self->last_position;
            for (UnkSonarPulse_l335b58** it = self->pulses.items; it != self->pulses.items + self->pulses.count;
                 ++it)
                (*it)->Update(center, self->radius, fade);
        }
    }

    f32 active = self->active_time - dt;
    active = !(active < 0.0f) ? active : 0.0f;
    self->active_time = active;
    if (active <= 0.0f) {
        if (self->field_0x398)
            fn_7100146e60(self->field_0x398, false);
        self->SetActive(false);
    }
}
