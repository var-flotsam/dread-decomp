// Unit 437 (CAimComponent): the root motion for a time step: the motion of the owner's and the aimer's
// animations over the step, plus the fall of a flying character, slid along whatever the owner's collision
// shape would hit on the way. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "game/CCharacterMovement.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1141.h"
#include "game/UnkTU365.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkBvhTree;
struct UnkBvhShape;
bool fn_7100d5b720(UnkBvhTree* self, u32 layers, UnkBvhShape* shape, const base::math::CVector2D* delta, void* arg);
UnkVec3f fn_7100714d78(const UnkXform* self, const UnkMatrix44* m);

extern void* g_unk_7101a23590[];

// The weighted transforms an animation step fills.
struct UnkMotionXform_l3519c0 {
    u8 data[0x70];
    UnkMotionXform_l3519c0() { fn_71007142e8(reinterpret_cast<UnkStruct_71007142e8*>(this)); }
};

struct UnkMotionStep_l3519c0 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    f32 time;  // 0xc
    u16 field_0x10;
};
void fn_7101179d9c(UnkAnimationComponent* self, UnkMotionXform_l3519c0* xforms, const UnkMotionStep_l3519c0* step);

struct UnkMotionEntry_l3519c0 {
    u8 pad_0x0[0x28];
    f32 duration;  // 0x28
};

struct UnkHitObj_l3519c0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
};

struct UnkHitRef_l3519c0 {
    UnkHitObj_l3519c0* ptr;

    UnkHitRef_l3519c0() : ptr(nullptr) {}
    ~UnkHitRef_l3519c0() {
        if (UnkHitObj_l3519c0* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkHitVec_l3519c0 {
    f32 x, y, z;
    UnkHitVec_l3519c0(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

struct UnkCollisionHit_l3519c0 {
    UnkHitRef_l3519c0 object;
    UnkHitRef_l3519c0 field_0x8;
    s32 id;
    UnkHitVec_l3519c0 point;
    UnkHitVec_l3519c0 field_0x20;
    UnkHitVec_l3519c0 normal;  // 0x2c
    f32 fraction;              // 0x38
    u32 flags;
    u32 field_0x40;

    UnkCollisionHit_l3519c0()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), fraction(0),
          flags(0), field_0x40(0) {}
};

struct UnkSweepQuery_l3519c0 {
    void** vtable;
    UnkCollisionHit_l3519c0 hit;
    bool field_0x50;

    UnkSweepQuery_l3519c0() {
        field_0x50 = false;
        vtable = g_unk_7101a23590 + 2;
    }
    ~UnkSweepQuery_l3519c0() { vtable = g_unk_7101a20ed8 + 2; }
};

struct UnkMotionAI_l3519c0 : base::reflection::UnkReflObject {
    u8 pad_0x8[0xe4];
    u32 mode;  // 0xec
};

struct UnkMotionAIHolder_l3519c0 {
    u8 pad_0x0[0x10];
    UnkMotionAI_l3519c0* ai;  // 0x10
};

struct UnkMotionFlying_l3519c0 {
    u8 pad_0x0[0x108];
    f32 gravity;  // 0x108
};

struct UnkMover_l3519c0 {
    u8 pad_0x0[0x3c8];
    u32 layers;  // 0x3c8
};

struct UnkMotionOwner_l3519c0 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x15c];
    UnkMotionFlying_l3519c0* flying;  // 0x1f0
    UnkMover_l3519c0* mover;          // 0x1f8
    u8 pad_0x200[0x20];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x18];
    UnkMotionAIHolder_l3519c0* ai;  // 0x240
};
static_assert(__builtin_offsetof(UnkMotionOwner_l3519c0, flying) == 0x1f0, "layout");
static_assert(__builtin_offsetof(UnkMotionOwner_l3519c0, animation) == 0x220, "layout");
static_assert(__builtin_offsetof(UnkMotionOwner_l3519c0, ai) == 0x240, "layout");

struct UnkMotionSelf_l3519c0 {
    u8 pad_0x0[0x30];
    UnkMotionOwner_l3519c0* owner;  // 0x30
};

static inline bool NearZero(f32 v) {
    return v - 0.001f <= 0.0f && v + 0.001f >= 0.0f;
}

static const u64 kMotionKey = 0xf4061ff25a2163d6;

// The step to request so that `dt` does not run the clip past its end.
static inline f32 ClipStep(UnkAnimationComponent* animation, f32 duration, f32 dt) {
    f32 time = fn_710117808c(animation, kMotionKey);
    f32 end = time + dt > duration ? duration : time + dt;
    return end - time;
}

static inline void FillStep(UnkMotionStep_l3519c0* request, f32 step) {
    request->field_0x0 = 2;
    request->field_0x4 = 2;
    request->field_0x8 = 0;
    request->time = step;
    request->field_0x10 = 0;
}

// The motion the transforms hold, turned by `angle`.
static inline UnkVec3f Motion(const UnkMotionXform_l3519c0* xforms, const UnkVec3f& angle) {
    return fn_7100714d78(reinterpret_cast<const UnkXform*>(xforms), reinterpret_cast<const UnkMatrix44*>(&angle));
}

UnkVec3f fn_71003519c0(UnkMotionSelf_l3519c0* self, UnkMotionOwner_l3519c0* owner, f32 dt) {
    UnkVec3f result(0.0f, 0.0f, 0.0f);
    UnkAnimationComponent* animation = owner ? owner->animation : nullptr;
    f32 duration = static_cast<UnkMotionEntry_l3519c0*>(fn_7101177d2c(animation, kMotionKey))->duration;
    f32 step = ClipStep(animation, duration, dt);
    if (!NearZero(step)) {
        UnkMotionXform_l3519c0 xforms[3];
        {
            UnkMotionStep_l3519c0 request;
            FillStep(&request, step);
            fn_7101179d9c(animation, xforms, &request);
        }
        result = Motion(xforms, owner->xform.GetAngle());
    }

    UnkAnimationComponent* aimAnimation = self->owner->animation;
    if (UnkMotionEntry_l3519c0* entry =
            static_cast<UnkMotionEntry_l3519c0*>(fn_7101177d2c(aimAnimation, kMotionKey))) {
        f32 step = ClipStep(aimAnimation, entry->duration, dt);
        if (!NearZero(step)) {
            UnkMotionXform_l3519c0 xforms[3];
            {
                UnkMotionStep_l3519c0 request;
                FillStep(&request, step);
                fn_7101179d9c(aimAnimation, xforms, &request);
            }
            UnkVec3f motion = Motion(xforms, self->owner->xform.GetAngle());
            result.x = motion.x + result.x;
            result.y = motion.y + result.y;
            result.z = motion.z + result.z;
        }
    }

    if (owner) {
        UnkMotionFlying_l3519c0* flying = owner->flying;
        if (flying && !fn_710117b348(animation, fn_7100cf96a8()))
            result.y += flying->gravity * -0.5f * dt * dt;
    }

    result.z = 0.0f;
    if (NearZero(result.x) && NearZero(result.y))
        return result;
    UnkMover_l3519c0* mover = owner->mover;
    if (!mover)
        return result;
    UnkShape* shape = fn_7100976588(reinterpret_cast<UnkFlagged*>(mover));
    if (!shape)
        return result;
    UnkGameManagerSub_0x608* collision = g_unk_7101d47310->field_0x608;
    if (!collision)
        return result;
    UnkBvhTree* tree = static_cast<UnkBvhTree*>(collision->field_0x820);
    if (!tree)
        return result;

    base::math::CVector2D delta;
    fn_710007b430(&delta, reinterpret_cast<const base::math::CVector3D*>(&result));
    UnkSweepQuery_l3519c0 query;
    if (fn_7100d5b720(tree, mover->layers, reinterpret_cast<UnkBvhShape*>(shape), &delta, &query)) {
        UnkMotionAI_l3519c0* holder = owner->ai->ai;
        f32 x = result.x;
        f32 y = result.y;
        f32 z = result.z;
        f32 fraction = query.hit.fraction;
        UnkMotionAI_l3519c0* ai = fn_710009d50c(holder->vf_0x0(), fn_7100408b2c()) ? holder : nullptr;
        if (ai->mode == 2) {
            result.x = fraction * x;
            result.y = fraction * y;
            result.z = fraction * z;
        } else {
            // What is left of the motion past the hit, slid along the hit's surface.
            const UnkHitVec_l3519c0& n = query.hit.normal;
            f32 rest = 1.0f - query.hit.fraction;
            f32 rx = result.x;
            f32 ry = result.y;
            f32 rz = result.z;
            f32 k = (rx * rest * n.x + rest * ry * n.y + rest * rz * n.z) / (n.x * n.x + n.y * n.y + n.z * n.z);
            result.x = rx - n.x * k;
            result.y = ry - n.y * k;
            result.z = rz - n.z * k;
        }
    }
    return result;
}
