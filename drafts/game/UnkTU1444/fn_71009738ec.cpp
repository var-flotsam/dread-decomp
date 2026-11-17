// Unit 1444 of tools/tu_map.py: puts the owner against the wall its stored target names: moves it to the
// wall's point, turns it to the target's angles (the side axes mirrored when +0x1ff is set) and, unless it
// already faces that way or is a side enemy, stores the new angles. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkObject_0x100.h"
#include "game/UnkTU376.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkUnit1444_o0309;
UnkVec3f fn_7100973658(UnkUnit1444_o0309* self);
void fn_7100077a60(UnkMatrix44* mat, f32 a, f32 b, f32 c);
void fn_71000769f0(UnkMatrix44* out, const void* a, const void* b, const void* c, const void* d);
// The origin the facing matrix is built at.
__attribute__((visibility("hidden"))) extern const CVector3D g_unk_710164b31c;

// The stored target: the side of the wall (0xff for none) and the angles to face.
struct UnkWallTarget_l9738ec {
    u8 pad_0x0[0x1c];
    u8 side;  // 0x1c
    u8 pad_0x1d[0xb];
    f32 angle_x;  // 0x28
    f32 angle_y;  // 0x2c
    f32 angle_z;  // 0x30
    u8 pad_0x34[0x100 - 0x34];

    UnkWallTarget_l9738ec() { fn_71002b57bc(AsObject()); }
    ~UnkWallTarget_l9738ec() { fn_71002b6094(AsObject()); }
    UnkObject_0x100* AsObject() { return reinterpret_cast<UnkObject_0x100*>(this); }
};
static_assert(sizeof(UnkWallTarget_l9738ec) == 0x100, "layout");

struct UnkWallState_l9738ec {
    u8 pad_0x0[0x70];
    u32 state;  // 0x70
};

struct UnkWallAI_l9738ec {
    void** vtable;
    u8 pad_0x8[0x16e6 - 0x8];
    u8 wall_side;  // 0x16e6

    void vf_0x598(UnkWallTarget_l9738ec* target) {
        reinterpret_cast<void (*)(UnkWallAI_l9738ec*, UnkWallTarget_l9738ec*)>(vtable[0x598 / 8])(this, target);
    }
};

struct UnkWallMovement_l9738ec {
    void** vtable;
    const base::reflection::CType* vf_0x0() {
        return reinterpret_cast<const base::reflection::CType* (*)(UnkWallMovement_l9738ec*)>(vtable[0])(this);
    }
};

struct UnkWallOwner_l9738ec {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x1b8 - 0x94];
    UnkWallState_l9738ec* state;  // 0x1b8
    u8 pad_0x1c0[0x1f0 - 0x1c0];
    UnkWallMovement_l9738ec* movement;  // 0x1f0
    u8 pad_0x1f8[0x240 - 0x1f8];
    UnkWallAI_l9738ec* ai;  // 0x240
    u8 pad_0x248[0x3b0 - 0x248];
    bool moved;  // 0x3b0
    u8 pad_0x3b1[0xf];
    bool turned;  // 0x3c0

    void vf_0x118(UnkXform* x, u32* what) {
        reinterpret_cast<void (*)(UnkWallOwner_l9738ec*, UnkXform*, u32*)>(vtable[0x118 / 8])(this, x, what);
    }
    bool IsSideEnemy() {
        return movement && fn_710009d50c(movement->vf_0x0(), fn_71009b57c4());
    }
};
static_assert(__builtin_offsetof(UnkWallOwner_l9738ec, turned) == 0x3c0, "layout");

struct UnkWallSensor_l9738ec {
    void** vtable;
    UnkVec3f vf_0x370() {
        return reinterpret_cast<UnkVec3f (*)(UnkWallSensor_l9738ec*)>(vtable[0x370 / 8])(this);
    }
};

struct UnkUnit1444_l9738ec {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkWallOwner_l9738ec* owner;  // 0x30
    UnkWallSensor_l9738ec* sensor;  // 0x38
    u8 pad_0x40[0x90];
    UnkWallTarget_l9738ec target;  // 0xd0
    bool has_target;  // 0x1d0
    u8 pad_0x1d1[2];
    u8 side;  // 0x1d3
    bool use_side;  // 0x1d4
    u8 pad_0x1d5[0x1ff - 0x1d5];
    bool mirrored;  // 0x1ff
};
static_assert(__builtin_offsetof(UnkUnit1444_l9738ec, mirrored) == 0x1ff, "layout");

// The vector, or its opposite when mirrored.
static inline UnkVec3f Mirror(const UnkVec3f& v, bool mirrored) {
    return mirrored ? UnkVec3f(-v.x, -v.y, -v.z) : v;
}

// The vector's x and y.
static inline void Flatten(CVector2D* out, const UnkVec3f& v) {
    fn_710007b430(out, reinterpret_cast<const CVector3D*>(&v));
}

static inline f32 Dot(const CVector2D& a, const CVector2D& b) { return a.x * b.x + a.y * b.y; }

void fn_71009738ec(UnkUnit1444_l9738ec* self) {
    UnkWallState_l9738ec* state = self->owner->state;
    if (state && state->state == 2)
        return;

    UnkWallTarget_l9738ec target;
    if (self->has_target)
        fn_71002b5c90(target.AsObject(), self->target.AsObject());
    self->owner->ai->vf_0x598(&target);

    UnkNavMeshManager* manager = g_unk_71726a4510;
    if (!manager) {
        manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(manager);
        g_unk_71726a4510 = manager;
    }
    if (!manager->navmesh || target.side == 0xff)
        return;

    UnkVec3f wall = fn_7100973658(reinterpret_cast<UnkUnit1444_o0309*>(self));
    u8 side = self->use_side ? self->side : 0;
    self->owner->ai->wall_side = side;
    UnkWallOwner_l9738ec* owner = self->owner;
    owner->xform.SetTranslation(wall);
    {
        u32 moved = 1;
        owner->vf_0x118(&owner->xform, &moved);
    }
    owner->moved = true;

    UnkMatrix44 rotation;
    for (int i = 0; i < 16; ++i)
        rotation.m[i] = 0.0f;
    rotation.m[15] = 1.0f;
    rotation.m[10] = 1.0f;
    rotation.m[5] = 1.0f;
    rotation.m[0] = 1.0f;
    fn_7100077a60(&rotation, target.angle_x, target.angle_y, target.angle_z);

    bool mirrored = self->mirrored;
    const f32* r = rotation.m;
    UnkMatrix44 facing;
    {
        UnkVec3f side_axis = Mirror(UnkVec3f(r[0], r[4], r[8]), mirrored);
        UnkVec3f up_axis(r[1], r[5], r[9]);
        UnkVec3f front_axis = Mirror(UnkVec3f(r[2], r[6], r[10]), mirrored);
        fn_71000769f0(&facing, &side_axis, &up_axis, &front_axis, &g_unk_710164b31c);
    }

    CVector2D front_flat;
    Flatten(&front_flat, UnkVec3f(facing.m[2], facing.m[6], facing.m[10]));
    CVector2D wanted_flat;
    Flatten(&wanted_flat, self->sensor->vf_0x370());

    bool turn = Dot(front_flat, wanted_flat) < 0.99f && !self->owner->IsSideEnemy();
    if (!turn) {
        CVector2D up_flat;
        Flatten(&up_flat, UnkVec3f(facing.m[1], facing.m[5], facing.m[9]));
        const UnkMatrix44& current = self->owner->xform.GetMatrix();
        CVector2D current_flat;
        Flatten(&current_flat, UnkVec3f(current.m[1], current.m[5], current.m[9]));
        turn = Dot(up_flat, current_flat) < 0.99f && !self->owner->IsSideEnemy();
    }
    if (turn) {
        self->owner->ai->wall_side = side;
        owner = self->owner;
        owner->xform.SetAngle(fn_7100076acc(reinterpret_cast<const UnkXform*>(&facing)));
        u32 turned = 2;
        owner->vf_0x118(&owner->xform, &turned);
        owner->turned = true;
    }
}
