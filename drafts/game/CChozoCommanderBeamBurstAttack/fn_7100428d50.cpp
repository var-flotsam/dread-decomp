// CChozoCommanderBeamBurstAttack's unit (tools/tu_map.py unit 646): the burst's update, by state. 0 waits
// until the target is close enough and starts the burst; 1 aims and, after the aiming time, starts the
// charge; 2 charges, then casts the beam (state 3); 3 turns the beam, hurts what it hits (fn_7100429b58)
// and ends with fn_7100429d10 when the shooting time runs out or the hit lands; 4 sends the fired event's
// request once. Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkTU1730.h"
#include "game/UnkTimelineCalls.h"
#include "game_s2/UnkChozoCommanderAttack.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;
using base::reflection::UnkReflTypeIdArg;
using base::reflection::UnkTunable;

extern UnkTunable g_unk_71726c56e8;
extern void* g_unk_7101a35478[];
extern void* g_unk_7101a35488[];

// CTunableCharClassChozoCommanderBeamBurstAttack, as far as the update reads it.
struct UnkBurstTunable_l428d50 {
    u8 pad_0x0[0x28];
    f32 closest_dist_to_start_attack;  // 0x28 fClosestDistToStartAttack
    f32 aim_angle_interp_speed;        // 0x2c fAimAngleInterpSpeed
    f32 shot_angle_interp_speed_min;   // 0x30 fShotAngleInterpSpeedMin
    f32 shot_angle_interp_speed_max;   // 0x34 fShotAngleInterpSpeedMax
    f32 time_aiming;                   // 0x38 fTimeAiming
    f32 time_shooting_init;            // 0x3c fTimeShootingInit
    u8 pad_0x40[0x8];
    f32 time_to_impact;                 // 0x48 fTimeToImpact
    f32 time_to_reset_hitting_player;   // 0x4c fTimeToResetHittingPlayer
};

// What the beam's ray met: a handle on the collider, a second reference and the hit point.
struct UnkBurstHit_l428d50 {
    UnkWeakHandle* collider;  // 0x0
    UnkRefObject* field_0x8;
    s32 field_0x10;
    f32 point[3];  // 0x14
    f32 field_0x20[9];

    UnkBurstHit_l428d50() : collider(nullptr), field_0x8(nullptr), field_0x10(-1) {
        point[0] = 0.0f;
        point[1] = 0.0f;
        point[2] = 0.0f;
        for (f32& f : field_0x20)
            f = 0.0f;
    }
    ~UnkBurstHit_l428d50() {
        UnkRefObject* second = field_0x8;
        if (second && --second->ref_count == 0)
            second->vf_0x10();
        UnkWeakHandle* handle = collider;
        if (handle && --handle->ref_count == 0)
            handle->vf_0x10();
    }
};

// The collider a hit handle points at: its component sits at +0x10.
struct UnkBurstCollider_l428d50 {
    u8 pad_0x0[0x10];
    UnkReflObject* component;
};

struct UnkBurstVictim_l428d50 {
    u8 pad_0x0[0x1c0];
    void* field_0x1c0;
};

// CCollisionComponent, as far as the update reads it.
struct UnkBurstCollision_l428d50 : UnkReflObject {
    u8 pad_0x8[0x28];
    UnkBurstVictim_l428d50* owner;  // 0x30
};

struct UnkBeamBurstUpdate_l428d50 {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x48];
    s32 state;  // 0x58
    u8 pad_0x5c[0x10];
    f32 aim_time;       // 0x6c
    f32 charge_time;    // 0x70
    f32 shoot_time;     // 0x74
    f32 shooting_time;  // 0x78
    f32 hit_time;       // 0x7c
    f32 miss_time;      // 0x80
    u8 pad_0x84[0x24];
    bool fired;  // 0xa8
    u8 pad_0xa9[3];
    u32 fired_value;  // 0xac
};

// The request the fired event sends along.
struct UnkBurstFiredArg_l428d50 {
    u32 field_0x0;
    u32 field_0x4;
    u32 value;
};

struct UnkBurstStart_ovb0135;
struct UnkBurstAim_ovb0135;
struct UnkBurstAimPoint_ovb0135;
struct UnkBeamBurstFire_A2_b75;
void fn_7100428af4(UnkBurstStart_ovb0135* self);
void fn_7100426de4(UnkBurstStart_ovb0135* self, u32 a, f32 b, f32 c, f32 d);
void CChozoCommanderBeamBurstAttack_PlaceEffectAtPoint(UnkBurstAim_ovb0135* self, const UnkBurstAimPoint_ovb0135* point);
void fn_7100429d10(UnkBeamBurstFire_A2_b75* self);
void CChozoCommanderBeamBurstAttack_SignalBeamBurstStart(UnkBeamBurstUpdate_l428d50* self);
void fn_71004294d8(UnkBeamBurstUpdate_l428d50* self, UnkBurstHit_l428d50* hit);
void CChozoCommanderAttack_SwitchOnPart(UnkBeamBurstUpdate_l428d50* self);
void fn_7100427048(UnkBeamBurstUpdate_l428d50* self, f32 dt, f32 scale, const void* a, const void* b,
                   UnkBurstHit_l428d50* hit);
void fn_7100427580(UnkBeamBurstUpdate_l428d50* self);
void fn_7100429b58(UnkBeamBurstUpdate_l428d50* self, UnkBurstVictim_l428d50* victim, UnkBurstHit_l428d50* hit,
                   bool impact);

static inline UnkChozoCommanderAttack* Base(UnkBeamBurstUpdate_l428d50* self) {
    return reinterpret_cast<UnkChozoCommanderAttack*>(self);
}
static inline UnkBurstStart_ovb0135* Start(UnkBeamBurstUpdate_l428d50* self) {
    return reinterpret_cast<UnkBurstStart_ovb0135*>(self);
}

// The collision component the beam met, else null.
inline UnkBurstCollision_l428d50* HitCollision(const UnkBurstHit_l428d50& hit) {
    if (!hit.collider || !hit.collider->target)
        return nullptr;
    UnkReflObject* component = reinterpret_cast<UnkBurstCollider_l428d50*>(hit.collider->target)->component;
    if (!component || !fn_710009d50c(component->vf_0x0(), fn_710085fc44()))
        return nullptr;
    return static_cast<UnkBurstCollision_l428d50*>(component);
}

// Tells the timeline the event `key` happened, with every bit of the mask set.
inline void Notify(UnkTimelineComponent* timeline, const u64* key) {
    UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, key, &mask.id, false, -1.0f);
}

void fn_7100428d50(UnkBeamBurstUpdate_l428d50* self, f32 dt) {
    fn_7100424b5c(Base(self), dt);
    const UnkBurstTunable_l428d50& tunable = reinterpret_cast<const UnkBurstTunable_l428d50&>(g_unk_71726c56e8);
    switch (self->state) {
    case 0: {
        f32 x = self->owner->field_0x44;
        UnkActor* target = fn_7100d7c990(self->owner);
        if (__builtin_fabsf(target->field_0x44 - x) < tunable.closest_dist_to_start_attack)
            fn_7100428af4(Start(self));
        break;
    }
    case 1: {
        fn_7100426de4(Start(self), 0, dt, tunable.aim_angle_interp_speed, tunable.aim_angle_interp_speed);
        UnkBurstHit_l428d50 hit;
        self->aim_time += dt;
        if (self->aim_time >= 0.2f)
            CChozoCommanderAttack_SwitchOnPart(self);
        fn_7100427048(self, dt, 1.0f, g_unk_7101a35478, g_unk_7101a35488, &hit);
        if (self->aim_time >= tunable.time_aiming) {
            void* animation = self->owner->field_0x220;
            UnkReflTypeIdArg mode;
            mode.id = 0;
            UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7101176690(animation, 0xba65be312e2807a6, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958,
                          &g_unk_7172769758, &mask);
            fn_7100427580(self);
            self->state = 2;
            self->charge_time = 0.0f;
            if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
                u64 key = StringCrc64("onbeamburstcharging", 19);
                Notify(timeline, &key);
            }
        }
        break;
    }
    case 2: {
        self->charge_time += dt;
        if (self->charge_time >= tunable.time_shooting_init) {
            CChozoCommanderBeamBurstAttack_SignalBeamBurstStart(self);
            UnkBurstHit_l428d50 hit;
            fn_71004294d8(self, &hit);
            CChozoCommanderBeamBurstAttack_PlaceEffectAtPoint(reinterpret_cast<UnkBurstAim_ovb0135*>(self),
                          reinterpret_cast<const UnkBurstAimPoint_ovb0135*>(&hit));
            self->hit_time = 0.0f;
            self->shoot_time = 0.0f;
            self->state = 3;
        }
        break;
    }
    case 3: {
        fn_7100426de4(Start(self), 0, dt, tunable.shot_angle_interp_speed_min, tunable.shot_angle_interp_speed_max);
        UnkBurstHit_l428d50 hit;
        fn_71004294d8(self, &hit);
        CChozoCommanderBeamBurstAttack_PlaceEffectAtPoint(reinterpret_cast<UnkBurstAim_ovb0135*>(self),
                      reinterpret_cast<const UnkBurstAimPoint_ovb0135*>(&hit));
        UnkBurstCollision_l428d50* collision = HitCollision(hit);
        if (collision && collision->owner->field_0x1c0) {
            UnkBurstVictim_l428d50* victim = collision->owner;
            if (!(self->hit_time < tunable.time_to_impact)) {
                fn_7100429b58(self, victim, &hit, true);
                fn_7100429d10(reinterpret_cast<UnkBeamBurstFire_A2_b75*>(self));
                break;
            }
            fn_7100429b58(self, victim, &hit, false);
            self->hit_time += dt;
            self->miss_time = 0.0f;
        } else {
            self->miss_time += dt;
        }
        if (self->shoot_time <= 0.0f)
            fn_7100426b54(Base(self), 0x705923660309e254, false);
        if (self->miss_time > tunable.time_to_reset_hitting_player)
            self->hit_time = 0.0f;
        self->shoot_time += dt;
        if (self->shoot_time >= self->shooting_time)
            fn_7100429d10(reinterpret_cast<UnkBeamBurstFire_A2_b75*>(self));
        break;
    }
    case 4:
        UnkReflTypeIdArg fired_mode;
        UnkReflTypeIdArg fired_mask;
        if (self->fired) {
            void* animation = self->owner->field_0x220;
            fired_mode.id = 2;
            UnkBurstFiredArg_l428d50 arg;
            arg.field_0x0 = 0;
            arg.field_0x4 = 1;
            arg.value = self->fired_value;
            fired_mask.id = UnkAll_SpitSend().value;
            fn_7101176690(animation, 0xf08bac6b45a8080e, 0xf4061ff25a2163d6, &fired_mode, &g_unk_710164b958, &arg,
                          &fired_mask);
            self->fired = false;
        }
        break;
    default:
        break;
    }
}
