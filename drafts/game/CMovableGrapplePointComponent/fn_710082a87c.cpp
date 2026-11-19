// CMovableGrapplePointComponent's unit (1292): the pull. After the base step (fn_710082ccec), when pulled and
// fn_710082ac54 accepts the puller, moves the owner 100 units away from the puller's side, sets the owner's
// mover (+0x238) going, spawns the effect g_unk_71727249d8 at the bone the owner's model names and starts it
// on the owner's FX component (+0x280), offset to the side's collision object (fn_710082a500); then reports
// the owner's position (vf_0x1b0) with the game manager's blackboard. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAutclastChargeAttack.h"
#include "game/CFXComponent.h"
#include "game/CPullableGrapplePointComponent.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkModelManager.h"
#include "game/UnkTU134.h"
#include "game/UnkTU253.h"
#include "game/UnkTU2739.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector3D;

extern const u8 g_unk_71727249d8[] __attribute__((visibility("hidden")));  // the effect's path id

// The effect object the model manager hands out, as its virtual calls see it.
struct UnkPullFx_l82a87c;
struct UnkPullFxVtable_l82a87c {
    void* unk_0x0[2];
    void (*set_position)(UnkPullFx_l82a87c* self, const UnkVec3f* v);  // 0x10
    void (*set_rotation)(UnkPullFx_l82a87c* self, const UnkVec3f* v);  // 0x18
    void (*set_scale)(UnkPullFx_l82a87c* self, const UnkVec3f* v);     // 0x20
    void* unk_0x28[13];
    void (*set_loop)(UnkPullFx_l82a87c* self, bool loop);               // 0x90
    void (*set_speed)(UnkPullFx_l82a87c* self, f32 speed);              // 0x98
};
struct UnkPullFx_l82a87c {
    UnkPullFxVtable_l82a87c* vtable;
};

struct UnkPullNameHolder_l82a87c {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

// What the owner's model shows (its bones are looked up in +0x138).
struct UnkPullModelData_l82a87c {
    u8 pad_0x0[0x138];
    char* resource;  // 0x138
};

struct UnkPullModel_l82a87c {
    u8 pad_0x0[0x50];
    UnkPullModelData_l82a87c* data;  // 0x50
};

// The owner's mover.
struct UnkPullMover_l82a87c {
    u8 pad_0x0[0x58];
    f32 field_0x58;
    u8 pad_0x5c[0x4];
    u32 field_0x60;
    u32 field_0x64;
    f32 field_0x68;
};

// A collision object of the owner, with its horizontal extent at +0x40/+0x48.
struct UnkPullCollision_l82a87c {
    void** vtable;
    u8 pad_0x8[0x38];
    f32 min_x;  // 0x40
    u8 pad_0x44[0x4];
    f32 max_x;  // 0x48
    u8 pad_0x4c[0x14];
    bool field_0x60;

    void vf_0x98() { reinterpret_cast<void (*)(UnkPullCollision_l82a87c*)>(vtable[0x98 / 8])(this); }
};

struct UnkPullOwner_l82a87c {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x228 - 0x68];
    UnkPullModel_l82a87c* model;  // 0x228
    u8 pad_0x230[0x8];
    UnkPullMover_l82a87c* mover;  // 0x238
    u8 pad_0x240[0x40];
    void* fx;  // 0x280
    u8 pad_0x288[0x18];
    UnkCollisionContainer* collision;  // 0x2a0
    u8 pad_0x2a8[0x3b0 - 0x2a8];
    bool moved;  // 0x3b0
};
static_assert(__builtin_offsetof(UnkPullOwner_l82a87c, moved) == 0x3b0, "layout");

struct UnkPuller_l82a87c {
    u8 pad_0x0[0x44];
    f32 x;
};

struct UnkMovableGrapplePoint_A5_b70;

struct UnkMovablePull_l82a87c {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkPullOwner_l82a87c* owner;  // 0x30
    u8 pad_0x38[0x64];
    f32 field_0x9c;

    void vf_0x1b0(CBlackboard* blackboard, const UnkVec3f& position) {
        reinterpret_cast<void (*)(UnkMovablePull_l82a87c*, CBlackboard*, UnkVec3f)>(vtable[0x1b0 / 8])(
            this, blackboard, position);
    }
};

struct UnkPullMatrix_l82a87c {
    f32 m[4][4];
};

bool fn_710082ac54(UnkMovablePull_l82a87c* self, UnkPuller_l82a87c* puller);
void** fn_710082a500(UnkMovableGrapplePoint_A5_b70* self, f32 side);

// `a` moved by `b`.
static inline UnkVec3f Add_l82a87c(const UnkVec3f& a, UnkVec3f b) {
    return UnkVec3f(a.x + b.x, a.y + b.y, a.z + b.z);
}

// The zero vector, as a UnkVec3f.
static inline const UnkVec3f& Zero_l82a87c() { return reinterpret_cast<const UnkVec3f&>(g_unk_710164779c); }

// Starts `effect` on the FX component with an offset and a rotation (fn_710115217c takes both by value,
// though it is declared elsewhere with references).
static inline void StartEffect_l82a87c(void* component, UnkFxObject_ovb0041* effect, UnkVec3f offset,
                                       UnkVec3f rotation) {
    using Start = void (*)(void*, void*, u32, u32, const base::global::CStrId*, UnkVec3f, UnkVec3f, void*);
    reinterpret_cast<Start>(fn_710115217c)(component, effect, 0, 4, &g_unk_7101d470e0, offset, rotation,
                                           const_cast<char*>(g_unk_710162e2ed));
}

static inline u64 PartHash_l82a87c(u64 offset) {
    return (*reinterpret_cast<UnkPullNameHolder_l82a87c**>(
                reinterpret_cast<char*>(&base::reflection::refl_GetSingleton()) + offset))
        ->hash;
}

static inline UnkModelManager* ModelManager() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return g_unk_71726b0cd8;
}

void fn_710082a87c(UnkMovablePull_l82a87c* self, UnkPuller_l82a87c* puller, bool pulled) {
    fn_710082ccec(reinterpret_cast<UnkDoorGrapplePoint_A5_b66*>(self), puller, pulled);
    if (!pulled || !fn_710082ac54(self, puller))
        return;
    UnkPullOwner_l82a87c* owner = self->owner;
    f32 x = owner->x;
    f32 y = owner->y;
    f32 d = puller->x - x;
    s32 side = (d > 0.0f) - (d < 0.0f);
    f32 sidef = side;
    UnkVec3f step(sidef * 100.0f, 0.0f, 0.0f);
    UnkVec3f moved = Add_l82a87c(UnkVec3f(x, y, owner->z), step);
    owner->moved = true;
    owner->x = moved.x;
    owner->y = moved.y;
    owner->z = moved.z;
    if (UnkPullMover_l82a87c* mover = self->owner->mover) {
        if (u32 delay = mover->field_0x60) {
            mover->field_0x60 = 0;
            mover->field_0x64 = delay;
            mover->field_0x58 = 0.8f;
        }
        mover->field_0x68 = 15.0f;
    }
    UnkPullModel_l82a87c* model = self->owner->model;
    UnkPullMatrix_l82a87c bone;
    __builtin_memset(&bone, 0, sizeof(bone));
    bone.m[3][3] = 1.0f;
    bone.m[2][2] = 1.0f;
    bone.m[1][1] = 1.0f;
    bone.m[0][0] = 1.0f;
    if (model && model->data) {
        UnkPullModelData_l82a87c* data = model->data;
        u64 name = PartHash_l82a87c(0xd20);
        fn_71001c1694(data->resource, &name, &bone.m[0][0]);
        UnkModelManager* manager = ModelManager();
        UnkVec3f position(bone.m[0][3], bone.m[1][3], bone.m[2][3]);
        UnkVec3f rotation(0.0f, 0.0f, 0.0f);
        UnkVec3f scale(1.0f, 1.0f, 1.0f);
        UnkFxObject_ovb0041* effect = fn_71003b7db0(manager, g_unk_71727249d8, false, true);
        if (effect) {
            UnkPullFx_l82a87c* fx = reinterpret_cast<UnkPullFx_l82a87c*>(effect);
            fx->vtable->set_position(fx, &position);
            fx->vtable->set_rotation(fx, &rotation);
            fx->vtable->set_scale(fx, &scale);
            fn_7100146dac(reinterpret_cast<UnkSwitchObject*>(fx), true);
            fx->vtable->set_loop(fx, false);
            fx->vtable->set_speed(fx, 1.0f);
            fn_71011a9bb4(manager, effect);
            if (void* component = self->owner->fx) {
                // Turned half a circle on the negative side.
                UnkVec3f spin(0.0f, 0.0f, 0.0f);
                if (side < 0)
                    spin.y = 3.1415927f;
                UnkCollisionContainer* collision = self->owner->collision;
                UnkVec3f offset = Zero_l82a87c();
                const base::global::CStrId* source = reinterpret_cast<const base::global::CStrId*>(
                    fn_710082a500(reinterpret_cast<UnkMovableGrapplePoint_A5_b70*>(self), sidef));
                if (UnkPullCollision_l82a87c* box =
                        reinterpret_cast<UnkPullCollision_l82a87c*>(fn_71008640a4(collision, source))) {
                    if (box->field_0x60)
                        box->vf_0x98();
                    offset.x = (box->min_x + box->max_x) * 0.5f - x;
                    offset.y = self->field_0x9c - y;
                    offset.z = 200.0f;
                }
                UnkVec3f angles = fn_7100076acc(reinterpret_cast<const UnkXform*>(&bone));
                StartEffect_l82a87c(component, effect, offset, Add_l82a87c(angles, spin));
            }
        }
    }
    UnkVec3f position(self->owner->x, self->owner->y, self->owner->z);
    self->vf_0x1b0(&g_unk_7101d47310->field_0x968, position);
}
