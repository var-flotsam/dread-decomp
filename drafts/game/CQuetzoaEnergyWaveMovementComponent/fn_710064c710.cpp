// CQuetzoaEnergyWaveMovementComponent's unit (tools/tu_map.py): the update. Before the wave is charged
// (+0x5ec 0), once it has waited 0.2 s and passed its target, a holder that is no CQuetzoaXAIComponent
// ends it; then the base update runs, the wave times its charge against the tunable's
// fTimeBeforeElectrify and electrifies (fn_710064ca40) when its own sub-area is under it, and ends
// after fElectrifyTime. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CProjectileMovement.h"
#include "game/UnkActor.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2577.h"
#include "game/UnkTU341.h"
#include "types.h"

struct UnkEnergyWave_b59a4;
void fn_710064ca40(UnkEnergyWave_b59a4* self);

struct UnkObjMgrB_b189;
struct UnkRecB_b189;
struct UnkObjB_b189;
bool fn_7100ee2bd8(UnkObjMgrB_b189* self, base::global::CRntVector<UnkRecB_b189*>* out, const base::math::CVector2D* pos,
                   u32 mask, UnkObjB_b189* skip);

// CCharClassQuetzoaEnergyWaveAttack::CTunableCharClassQuetzoaEnergyWaveAttack
extern base::reflection::UnkTunable g_unk_71726fb238;

struct UnkEnergyWaveTunable_l64c710 {
    u8 pad_0x0[0x28];
    f32 time_before_electrify;  // 0x28
    f32 electrify_time;         // 0x2c
};

// What the owner's +0x1b8 points to.
struct UnkEnergyWaveLife_l64c710 {
    u8 pad_0x0[0x70];
    s32 state;  // 0x70
};

struct UnkEnergyWaveAI_l64c710 {
    void** vtable;
    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkEnergyWaveAI_l64c710*)>(vtable[0])(this);
    }
};

struct UnkEnergyWaveHolder_l64c710 {
    u8 pad_0x0[0x10];
    UnkActor* actor;  // 0x10
};

struct UnkEnergyWaveSubArea_l64c710 {
    u64 id;
};

struct UnkEnergyWaveSubAreaMgr_l64c710 {
    u8 pad_0x0[0x3f0];
    UnkEnergyWaveSubArea_l64c710* current;  // 0x3f0
};

struct UnkEnergyWaveMovement_l64c710 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x8];
    f32 dir_x;  // 0x40
    f32 dir_y;  // 0x44
    f32 dir_z;  // 0x48
    u8 pad_0x4c[0x1e4];
    UnkEnergyWaveHolder_l64c710* holder;  // 0x230
    u8 pad_0x238[0x3ac];
    f32 charge_time;  // 0x5e4
    f32 wait_time;    // 0x5e8
    s32 state;        // 0x5ec
};
static_assert(__builtin_offsetof(UnkEnergyWaveMovement_l64c710, holder) == 0x230, "layout");
static_assert(__builtin_offsetof(UnkEnergyWaveMovement_l64c710, charge_time) == 0x5e4, "layout");

inline bool IsLifeState2_l64c710(UnkActor* owner) {
    UnkEnergyWaveLife_l64c710* life = static_cast<UnkEnergyWaveLife_l64c710*>(owner->field_0x1b8);
    return life && life->state == 2;
}

// The sub-area manager, created on first use.
inline UnkEnergyWaveSubAreaMgr_l64c710* SubAreaManager_l64c710() {
    UnkManager* manager = g_unk_7101d47360;
    if (!manager) {
        manager = static_cast<UnkManager*>(fn_71000839a0(0xae8, 0x3f));
        fn_7100ee1f34(manager);
        g_unk_7101d47360 = manager;
    }
    return reinterpret_cast<UnkEnergyWaveSubAreaMgr_l64c710*>(manager);
}

struct UnkEnergyWaveVec_l64c710 {
    f32 x, y, z;
};

inline UnkEnergyWaveVec_l64c710 operator-(const UnkEnergyWaveVec_l64c710& a, const UnkEnergyWaveVec_l64c710& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

inline UnkEnergyWaveVec_l64c710 Pos_l64c710(UnkActor* actor) {
    return {actor->field_0x44, actor->field_0x54, actor->field_0x64};
}

// Whether the wave's holder is a Quetzoa X (its AI component a CQuetzoaXAIComponent).
inline bool IsHeldByQuetzoaX_l64c710(UnkEnergyWaveMovement_l64c710* self) {
    UnkEnergyWaveHolder_l64c710* holder = self->holder;
    if (!holder || !holder->actor)
        return false;
    auto* ai = static_cast<UnkEnergyWaveAI_l64c710*>(holder->actor->field_0x240);
    return ai && fn_710009d50c(ai->vf_0x0(), fn_710063efe4());
}

void fn_710064c710(UnkEnergyWaveMovement_l64c710* self, f32 dt) {
    if (IsLifeState2_l64c710(self->owner))
        return;
    if (fn_7100d7bea0(self->owner, 2)) {
        self->wait_time = 0.0f;
    } else {
        f32 wait = self->wait_time + dt;
        self->wait_time = wait;
        if (wait > 0.2f && self->state == 0) {
            UnkActor* target = fn_7100d7c990(self->owner);
            UnkEnergyWaveVec_l64c710 to_target = Pos_l64c710(target) - Pos_l64c710(self->owner);
            f32 ahead = to_target.x * self->dir_x + to_target.y * self->dir_y + to_target.z * self->dir_z;
            if (ahead < 0.0f && !IsHeldByQuetzoaX_l64c710(self)) {
                fn_7100232c68(self->owner);
                return;
            }
        }
    }
    fn_7100ac3a78(reinterpret_cast<UnkNailongThornMovement*>(self), dt);
    f32 charge = self->charge_time + dt;
    self->charge_time = charge;
    const UnkEnergyWaveTunable_l64c710* tunable = reinterpret_cast<const UnkEnergyWaveTunable_l64c710*>(&g_unk_71726fb238);
    if (self->state == 2) {
        if (charge > tunable->electrify_time)
            fn_7100232c68(self->owner);
        return;
    }
    if (self->state != 1 || !(charge > tunable->time_before_electrify))
        return;
    // Electrified only over the owner's own sub-area.
    bool found = false;
    if (!IsLifeState2_l64c710(self->owner) && SubAreaManager_l64c710()->current) {
        base::math::CVector2D pos2d;
        base::global::CRntVector<UnkEnergyWaveSubArea_l64c710*> areas(0xf);
        UnkEnergyWaveSubAreaMgr_l64c710* manager = SubAreaManager_l64c710();
        UnkActor* owner = self->owner;
        base::math::CVector3D pos = {owner->field_0x44, owner->field_0x54, owner->field_0x64};
        fn_710007b430(&pos2d, &pos);
        if (fn_7100ee2bd8(reinterpret_cast<UnkObjMgrB_b189*>(manager),
                          reinterpret_cast<base::global::CRntVector<UnkRecB_b189*>*>(&areas), &pos2d, 4, nullptr)) {
            for (UnkEnergyWaveSubArea_l64c710* area : areas) {
                if (area->id == SubAreaManager_l64c710()->current->id) {
                    found = true;
                    break;
                }
            }
        }
        areas.Free();
    }
    if (found)
        fn_710064ca40(reinterpret_cast<UnkEnergyWave_b59a4*>(self));
    else
        fn_7100232c68(self->owner);
}
