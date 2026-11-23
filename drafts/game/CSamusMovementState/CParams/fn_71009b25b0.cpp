// CSamusMovementState's unit (tools/tu_map.py unit 1468): picks the movement state to go to when the animation
// forces the default action (or when asked to). In the air it takes the movement's air transition (or the melee
// one), else the fall one with the id table's default; on the ground the land transition, unless no floor is
// found below the owner, when the tweak-fall track's parameters are sent instead. The request goes to the state
// machine (+0x8) unless its current state refuses it or forwards it to the animation. Names and layouts are
// placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU1455.h"
#include "game/UnkTU1730.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector3D;

extern UnkDesc_SpitSend g_unk_710164b958;

// As src/game/CCharacterMovement/CCharacterMovement_Movement_ovb0021.cpp declares them.
struct UnkTrackTime_ovb0021 {
    u64 key;
    u64 time;
};
struct UnkTrackBlend_ovb0021 {
    u32 a, b, c;
};

// What a track request is built from (0x30 bytes): no id, the default time and blend descriptors,
// no flags and an all-bits mask.
struct UnkTrackParams_ovb0021 {
    s64 id;
    UnkTrackTime_ovb0021 time;
    UnkTrackBlend_ovb0021 blend;
    u32 flags;
    u32 mask;

    __attribute__((always_inline)) UnkTrackParams_ovb0021(UnkTrackTime_ovb0021 time_, UnkTrackBlend_ovb0021 blend_, u32 mask_)
        : id(-1), time(time_), blend(blend_), flags(0), mask(mask_) {}
    __attribute__((always_inline)) UnkTrackParams_ovb0021()
        : UnkTrackParams_ovb0021(reinterpret_cast<const UnkTrackTime_ovb0021&>(g_unk_710164b958),
                                 reinterpret_cast<const UnkTrackBlend_ovb0021&>(g_unk_7172769740),
                                 base::reflection::refl_GetEnumNot8<0x7101d57cb8, 0x7101d57cc8>()) {}
};

struct UnkTrackMovement_ovb0021;
UnkTrackParams_ovb0021 fn_71009767f4(UnkTrackMovement_ovb0021* self, bool forced);
UnkTrackParams_ovb0021 fn_7100976944(UnkTrackMovement_ovb0021* self, bool forced);
UnkTrackParams_ovb0021 fn_71009ed9bc(UnkTrackMovement_ovb0021* self, bool forced);

struct UnkSamusMove_b50a5;
bool fn_71009ea3e4(UnkSamusMove_b50a5* self, f32 speed);

struct UnkCharacterMovement;
struct UnkCollisionHit;
bool fn_7100977e50(UnkCharacterMovement* self, f32 max_dot, f32 extra, f32 step, f32 back, u32 mask,
                   const CVector3D* pos, CVector3D* out_point, UnkCollisionHit* out_hit, const CVector3D* dir);

// A 64-bit word at a 4-byte aligned offset.
typedef u64 UnkU64Align4_l9b25b0 __attribute__((aligned(4)));

// CSamusMovementState::CParams (vtable g_unk_7101a22cb8), finished as one of its derived classes.
struct UnkStateParams_l9b25b0 {
    void* vtable;
    UnkTrackParams_ovb0021 transition;  // 0x8
    u32 field_0x38;
    UnkU64Align4_l9b25b0 field_0x3c;
    u32 field_0x44;

    explicit UnkStateParams_l9b25b0(void* derived_vtable)
        : vtable(reinterpret_cast<char*>(g_unk_7101a22cb8) + 0x10), transition() {
        field_0x38 = 3;
        field_0x3c = 0;
        field_0x44 = 0;
        vtable = derived_vtable;
    }
};
static_assert(sizeof(UnkStateParams_l9b25b0) == 0x48, "UnkStateParams_l9b25b0 layout");

// The CCharClassForceDefaultActionTrack record the animation resolves.
struct UnkForceDefaultTrack_l9b25b0 {
    u8 pad_0x0[0x18];
    bool on_ground;  // 0x18
    bool in_air;     // 0x19
    bool any_body;   // 0x1a
};

// The CCharClassTweakFallActionTrack record: fills in a transition.
struct UnkTweakFallTrack_l9b25b0 {
    void** vtable;
    void vf_0x18(UnkTrackParams_ovb0021* params) {
        reinterpret_cast<void (*)(UnkTweakFallTrack_l9b25b0*, UnkTrackParams_ovb0021*)>(vtable[0x18 / 8])(this,
                                                                                                         params);
    }
};

// The state machine's current state.
struct UnkMoveStateCur_l9b25b0 {
    void** vtable;
    u8 pad_0x8[0x8];
    u32 kind;  // 0x10
    bool vf_0x130(u32 a, u32 b, void* c) {
        return reinterpret_cast<bool (*)(UnkMoveStateCur_l9b25b0*, u32, u32, void*)>(vtable[0x130 / 8])(this, a, b,
                                                                                                        c);
    }
};

struct UnkMoveStateMachine_l9b25b0 {
    u8 pad_0x0[0x10];
    UnkMoveStateCur_l9b25b0* current;  // 0x10
};

struct UnkSamusBody_l9b25b0 {
    u8 pad_0x0[0x360];
    bool field_0x360;
};

struct UnkSamusOwner_l9b25b0 {
    u8 pad_0x0[0x38];
    UnkMatrix44 xform;  // 0x38
    u8 pad_0x78[0x1a8];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x88];
    UnkSamusBody_l9b25b0* body;  // 0x2b0
};

struct UnkSamusMovement_l9b25b0 {
    void** vtable;
    u8 pad_0x8[0x38];
    UnkVec3f velocity;  // 0x40
    u8 pad_0x4c[0x30];
    f32 vertical_speed;  // 0x7c
    u8 pad_0x80[0x350];
    u32 floor_mask;  // 0x3d0
    u8 pad_0x3d4[0x410];
    bool field_0x7e4;

    UnkCharacterMovement* vf_0x2a8() {
        return reinterpret_cast<UnkCharacterMovement* (*)(UnkSamusMovement_l9b25b0*)>(vtable[0x2a8 / 8])(this);
    }
    bool vf_0x3c0() { return reinterpret_cast<bool (*)(UnkSamusMovement_l9b25b0*)>(vtable[0x3c0 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkSamusMovement_l9b25b0, field_0x7e4) == 0x7e4, "UnkSamusMovement_l9b25b0 layout");

struct UnkSamusState_l9b25b0 {
    u8 pad_0x0[0x8];
    UnkMoveStateMachine_l9b25b0* machine;  // 0x8
    u8 pad_0x10[0x20];
    UnkSamusOwner_l9b25b0* owner;  // 0x30
    UnkSamusMovement_l9b25b0* movement;  // 0x38
};

// A string id's hash.
struct UnkIdHolder_l9b25b0 {
    u8 pad_0x0[0x30];
    u64 id;
};

static inline UnkTrackQuery Query(const base::reflection::CClass* cls) {
    UnkTrackQuery query;
    query.cls = cls;
    query.result = nullptr;
    query.value = -1.0f;
    return query;
}

static inline UnkForceDefaultTrack_l9b25b0* Track(const UnkTrackQuery& query) {
    return reinterpret_cast<UnkForceDefaultTrack_l9b25b0*>(query.result);
}

// Whether `v` is within 0.001 of zero.
static inline bool NearZero(f32 v) { return v + 0.001f >= 0.0f && v - 0.001f <= 0.0f; }

static inline u64 FallId() {
    // The id table's fall id (its string id at +0x1e50).
    const base::global::CStrId* ids = &GameManagerIdTable().field_0x1d78;
    return reinterpret_cast<const UnkIdHolder_l9b25b0*>(ids[(0x1e50 - 0x1d78) / 8].data)->id;
}

void fn_71009b25b0(UnkSamusState_l9b25b0* self, bool forced) {
    UnkTrackQuery force = Query(fn_7100cfabc8());
    if (UnkAnimationComponent* animation = self->owner->animation)
        fn_710117b08c(animation, &force);
    if (!force.result && !forced)
        return;

    UnkStateParams_l9b25b0 params(reinterpret_cast<char*>(g_unk_7101a22d18) + 0x10);
    UnkStateParams_l9b25b0 fall_params(g_unk_7101a22ce8 + 0x10);
    UnkSamusMovement_l9b25b0* movement = self->movement;
    f32 speed = movement->vertical_speed;
    auto* track_movement = reinterpret_cast<UnkTrackMovement_ovb0021*>(movement);

    if (!NearZero(speed)) {
        bool allowed = forced || Track(force)->in_air;
        if (Track(force) && !Track(force)->any_body)
            allowed &= !self->owner->body->field_0x360;
        if (!allowed)
            return;
        bool done = false;
        if (fn_71009ea3e4(reinterpret_cast<UnkSamusMove_b50a5*>(movement), speed)) {
            UnkTrackParams_ovb0021 air = fn_71009ed9bc(reinterpret_cast<UnkTrackMovement_ovb0021*>(self->movement), true);
            if (air.id != -1) {
                params.transition = air;
                done = true;
            } else {
                done = fn_710117b348(self->owner->animation, fn_7100d1ab58());
            }
        }
        if (!done) {
            f32 vx = self->movement->velocity.x;
            f32 vy = self->movement->velocity.y;
            f32 vz = self->movement->velocity.z;
            const UnkMatrix44& m = fn_710002614c(&self->owner->xform);
            f32 up = vx * m.m[2] + vy * m.m[6] + vz * m.m[10];
            if (up < 0.0f && !NearZero(up))
                return;
            UnkTrackParams_ovb0021 fall = fn_7100976944(reinterpret_cast<UnkTrackMovement_ovb0021*>(self->movement), true);
            if (fall.id == -1) {
                fall.id = FallId();
                fall.flags |= 1;
            }
            params.transition = fall;
        }
    } else {
        if (!forced && !Track(force)->on_ground)
            return;
        UnkTrackParams_ovb0021 land = fn_71009767f4(track_movement, true);
        if (land.id == -1) {
            if (self->movement->field_0x7e4) {
                land.id = self->movement->vf_0x3c0() ? 0x6d2e3762678110fb : 0xf87a0c5667453849;
            } else {
                land.id = reinterpret_cast<const UnkIdHolder_l9b25b0*>(GameManagerIdTable().field_0x1d78.data)->id;
                land.flags |= 1;
            }
        }
        params.transition = land;
        if (fn_710117b348(self->owner->animation, fn_7100c0f318())) {
            bool on_floor;
            {
                // Whether there is a floor within 20 below the owner.
                UnkCharacterMovement* character = self->movement->vf_0x2a8();
                UnkSamusOwner_l9b25b0* owner = self->owner;
                UnkVec3f down(0.0f, -1.0f, 0.0f);
                UnkVec3f pos(owner->xform.m[3], owner->xform.m[7], owner->xform.m[11]);
                on_floor = fn_7100977e50(character, -0.3f, 20.0f, 3.4028235e38f, 0.0f, self->movement->floor_mask,
                                         reinterpret_cast<CVector3D*>(&pos), nullptr, nullptr,
                                         reinterpret_cast<CVector3D*>(&down));
            }
            if (!on_floor) {
                UnkTrackQuery tweak = Query(fn_7100d2fa08());
                fn_710117b08c(self->owner->animation, &tweak);
                if (auto* fall_track = reinterpret_cast<UnkTweakFallTrack_l9b25b0*>(tweak.result))
                    fall_track->vf_0x18(&fall_params.transition);
                fn_710098ae04(reinterpret_cast<UnkAirTarget_B71a6*>(self->machine),
                              reinterpret_cast<UnkAirRequest_B71a6*>(&fall_params));
                return;
            }
        }
    }

    if (UnkMoveStateCur_l9b25b0* current = self->machine->current) {
        if (current->kind == 3) {
            if (!current->vf_0x130(0, 0, nullptr))
                return;
        } else if (current->kind == 2) {
            fn_7101176ea8(reinterpret_cast<UnkStruct_7101176ea8*>(self->owner->animation),
                          reinterpret_cast<const UnkRequest_7101176ea8*>(&params.transition), 0xf4061ff25a2163d6);
            return;
        }
    }
    fn_710098ae04(reinterpret_cast<UnkAirTarget_B71a6*>(self->machine), reinterpret_cast<UnkAirRequest_B71a6*>(&params));
}
