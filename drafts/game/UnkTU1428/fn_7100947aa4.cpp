// CMorphBallMovement's unit: entering the ball from the previous movement. Carries its velocity over
// (normalised or along the owner's facing, scaled by a CCharClassTweakConvertToMorphBallVelocityTrack
// playing on the actor), lets the ball pop up when there is room above it, then resets the
// bone control's angle. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/math/CVectorOps.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CMovementComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU399.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

namespace base { namespace reflection { struct UnkTunable; } }

struct UnkLegNode;
void fn_71002d60c4(UnkLegNode* self, f32 weight);

// As in src/game/CKraidBouncingCreaturesMovementComponent/CKraidBouncingCreaturesMovementComponent_3.cpp.
struct UnkKraidCreatureVec {
    f32 x;
    f32 y;
    f32 z;
    UnkKraidCreatureVec(const UnkKraidCreatureVec& other) : x(other.x), y(other.y), z(other.z) {}
};

extern base::reflection::UnkTunable g_unk_71727393d8;
extern __attribute__((visibility("hidden"))) char g_unk_71727394b0[];

struct UnkMorphBallTunable_7aa4 {
    u8 pad_0x0[0x3c];
    s32 field_0x3c;
};

// What a CCharClassTweakConvertToMorphBallVelocityTrack resolves to.
struct UnkVelocityTweak_7aa4 {
    u8 pad_0x0[0x18];
    bool use_facing;  // 0x18
    u8 pad_0x19[3];
    f32 scale;  // 0x1c
};

struct UnkPrevMovement_7aa4 {
    u8 pad_0x0[0xf9];
    bool field_0xf9;
    u8 pad_0xfa[0x1d8 - 0xfa];
    CVector3D velocity;  // 0x1d8
    u8 pad_0x1e4[0x2324 - 0x1e4];
    u8 field_0x2324;
    u8 pad_0x2325[0x51a5 - 0x2325];
    bool field_0x51a5;

    void SetVelocity(const void* v) {
        fn_71010d08d4(reinterpret_cast<UnkMovement_71010d08d4*>(this),
                      reinterpret_cast<const UnkMovementVec_71010d08d4*>(v), true);
    }
};
static_assert(__builtin_offsetof(UnkPrevMovement_7aa4, field_0x51a5) == 0x51a5, "layout");

struct UnkMorphBallActor_7aa4 {
    u8 pad_0x0[0x38];
    UnkMatrix44 xform;  // 0x38
    u8 pad_0x78[0x208 - 0x78];
    UnkPrevMovement_7aa4* movement;  // 0x208
    u8 pad_0x210[0x220 - 0x210];
    UnkAnimationComponent* animation;  // 0x220

    CVector3D Position() const { return CVector3D{xform.m[3], xform.m[7], xform.m[11]}; }
};
static_assert(__builtin_offsetof(UnkMorphBallActor_7aa4, animation) == 0x220, "layout");

struct UnkMorphBallMovement_7aa4 {
    u8 pad_0x0[0x30];
    UnkMorphBallActor_7aa4* owner;  // 0x30
    u8 pad_0x38[0x3c8 - 0x38];
    u32 collision_mask;  // 0x3c8
    u8 pad_0x3cc[0x2534 - 0x3cc];
    f32 timer;  // 0x2534
    u8 pad_0x2538[0x253a - 0x2538];
    bool can_pop_up;  // 0x253a
    u8 pad_0x253b[0x2542 - 0x253b];
    bool boosting;  // 0x2542
    u8 pad_0x2543[0x2608 - 0x2543];
    u8 field_0x2608;
    u8 pad_0x2609[0x2630 - 0x2609];
    bool field_0x2630;
};
static_assert(__builtin_offsetof(UnkMorphBallMovement_7aa4, field_0x2630) == 0x2630, "layout");

struct UnkBoneControl_7aa4 {
    void** vtable;
    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkBoneControl_7aa4*)>(vtable[0])(this);
    }
};
void fn_71002e28d0(UnkBoneControl_7aa4* self, const UnkMatrix44* m);

struct UnkProjected_7aa4 : CVector2D {
    explicit UnkProjected_7aa4(const CVector3D& v) { fn_710007b430(this, &v); }
};

struct UnkHitBox_7aa4 {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkHitBox_7aa4(const CVector2D& s, const CVector2D& e) {
        start.x = s.x;
        start.y = s.y;
        end.x = e.x;
        end.y = e.y;
        delta.x = e.x - s.x;
        delta.y = e.y - s.y;
    }
};

// Sets the angle of the owner's bone control (a CBoneControl registered under g_unk_71727394b0) to a
// quarter turn about y, at full weight.
inline void ResetBoneControl(UnkMorphBallMovement_7aa4* self) {
    UnkVec3f angle(0.0f, 1.5707964f, 0.0f);
    UnkBoneControl_7aa4* bone = reinterpret_cast<UnkBoneControl_7aa4*>(
        fn_71002d57ac(self->owner->animation->field_0x40, g_unk_71727394b0));
    if (bone && !fn_710009d50c(bone->vf_0x0(), fn_71002e2544()))
        bone = nullptr;
    UnkMatrix44 rotation = fn_7100078fc8(reinterpret_cast<const UnkMatrix44&>(angle));
    fn_71002e28d0(bone, &rotation);
    fn_71002d60c4(reinterpret_cast<UnkLegNode*>(bone), 1.0f);
}

void fn_71010d9a1c(UnkMorphBallMovement_7aa4* self, UnkMorphBallActor_7aa4* actor);

void fn_7100947aa4(UnkMorphBallMovement_7aa4* self, UnkMorphBallActor_7aa4* actor) {
    fn_71010d9a1c(self, actor);
    self->boosting = true;
    if (actor) {
        self->timer = 0.0f;
        if (actor->movement) {
            UnkKraidCreatureVec v = fn_7100d7be08(actor);
            UnkTrackQuery query = {fn_7100d2e0b8(), nullptr, -1.0f};
            if (UnkAnimationComponent* animation = actor->animation) {
                fn_710117b08c(animation, &query);
                if (query.result) {
                    const UnkVelocityTweak_7aa4* tweak = reinterpret_cast<UnkVelocityTweak_7aa4*>(query.result);
                    if (tweak->use_facing) {
                        const UnkMatrix44& m = fn_710002614c(&self->owner->xform);
                        v.x = m.m[2];
                        v.y = m.m[6];
                        v.z = m.m[10];
                    } else {
                        f32 inv = 1.0f / (sqrtf(v.x * v.x + v.y * v.y + v.z * v.z) + 0.0001f);
                        v.x = v.x * inv;
                        v.y = v.y * inv;
                        v.z = v.z * inv;
                    }
                    f32 scale = reinterpret_cast<UnkVelocityTweak_7aa4*>(query.result)->scale;
                    v.x = scale * v.x;
                    v.y = scale * v.y;
                    v.z = scale * v.z;
                }
            }
            actor->movement->SetVelocity(&v);
            self->field_0x2608 = actor->movement->field_0x2324;
            self->can_pop_up = false;
            const auto* tunable = reinterpret_cast<const UnkMorphBallTunable_7aa4*>(&g_unk_71727393d8);
            if (actor->movement->field_0x51a5 && tunable->field_0x3c == 2) {
                self->can_pop_up = true;
                if (actor->movement->field_0xf9) {
                    UnkGameManagerSub_0x608* sub = g_unk_7101d47310->field_0x608;
                    UnkTree* tree = sub ? static_cast<UnkTree*>(sub->field_0x820) : nullptr;
                    UnkHitBox_7aa4 box(UnkProjected_7aa4(actor->Position()),
                                       UnkProjected_7aa4(actor->Position()) + CVector2D{0.0f, 120.0f});
                    bool blocked = fn_7100d5c518(tree, self->collision_mask,
                                                 reinterpret_cast<const UnkBox*>(&box), nullptr, 2);
                    UnkPrevMovement_7aa4* movement = actor->movement;
                    if (blocked) {
                        CVector3D up = {movement->velocity.x, 1000.0f, movement->velocity.z};
                        movement->SetVelocity(&up);
                    } else {
                        CVector3D flat = {movement->velocity.x, 0.0f, movement->velocity.z};
                        movement->SetVelocity(&flat);
                    }
                }
            }
        }
    }
    self->field_0x2630 = false;
    ResetBoneControl(self);
}
