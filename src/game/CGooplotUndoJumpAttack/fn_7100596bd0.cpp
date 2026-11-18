// CGooplotUndoJumpAttack unit (tools/tu_map.py): the undo jump's leap. Lets the AI component know
// (fn_710059ae78), hands the enemy movement a jump along the attack's direction, turns the owner to face
// it, and moves the owner 120 units along it unless the navigation mesh has a hit on the way
// (fn_7100ebfa7c). Names and layouts are placeholders.
#include <math.h>

#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/global/CRntVector.h"
#include "game/CAutsharpAIComponent.h"
#include "game/CCharClassAIComponent.h"
#include "game/CCharacterMovement.h"
#include "game/CEnemyMovement.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkSource_7100286798;
struct UnkFlagged;
struct UnkGooplotAI_l596bd0;
void fn_710059ae78(UnkGooplotAI_l596bd0* ai);

struct UnkGooplotAI_l596bd0 {
    u8 pad_0x0[0x10];
    UnkSource_7100286798* nav_source;  // 0x10
    u8 pad_0x18[0x147f8 - 0x18];
    s32 state;  // 0x147f8
    u8 pad_0x147fc[0x14871 - 0x147fc];
    bool field_0x14871;
};
static_assert(__builtin_offsetof(UnkGooplotAI_l596bd0, field_0x14871) == 0x14871, "layout");

struct UnkGooplotMovement_l596bd0 {
    void** vtable;
    u8 pad_0x8[0xf1];
    bool field_0xf9;

    void vf_0x260() { reinterpret_cast<void (*)(UnkGooplotMovement_l596bd0*)>(vtable[0x260 / 8])(this); }
};

struct UnkGooplotOwner_l596bd0 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x1f0 - 0x94];
    UnkReflObject* movement;  // 0x1f0
    u8 pad_0x1f8[0x20];
    UnkFlagged* field_0x218;
    u8 pad_0x220[0x20];
    UnkGooplotAI_l596bd0* ai;  // 0x240
    u8 pad_0x248[0x3b0 - 0x248];
    bool field_0x3b0;
    u8 pad_0x3b1[0xf];
    bool field_0x3c0;
};
static_assert(__builtin_offsetof(UnkGooplotOwner_l596bd0, field_0x3c0) == 0x3c0, "layout");

struct UnkGooplotUndoJump_l596bd0 {
    void** vtable;
    UnkGooplotOwner_l596bd0* owner;  // 0x8
    u8 pad_0x10[0x54];
    UnkVec3f dir;  // 0x64
    u8 pad_0x70[0x10];
    u32 field_0x80;
};

// A direction that may be unset: its value and flag.
struct UnkOptVec3Data_l596bd0 {
    UnkVec3f value;
    bool set;
};

// The optional wrapper: constructing it clears the value and the flag (up to the flag, 13 bytes).
struct UnkOptVec3_l596bd0 : UnkOptVec3Data_l596bd0 {
    UnkOptVec3_l596bd0() : UnkOptVec3Data_l596bd0() {}
};

// The block handed to the movement (fn_71010c7e6c); the vtable tells the kinds apart.
struct UnkJumpParams_l596bd0 {
    void** vtable;
    u32 field_0x8;
    bool field_0xc;
    u8 pad_0xd[7];
    bool field_0x14;
    UnkOptVec3_l596bd0 field_0x18;
    UnkOptVec3_l596bd0 dir;  // 0x28
    u32 field_0x38;
    bool field_0x3c;
    bool field_0x3d;

    UnkJumpParams_l596bd0() : field_0xc(false), field_0x14(false), field_0x18(), dir() {
        vtable = g_unk_7101a2e208 + 2;
        field_0x3d = false;
        field_0x38 = 3;
    }
};
static_assert(__builtin_offsetof(UnkJumpParams_l596bd0, field_0x38) == 0x38, "layout");

struct UnkProbePoint_l596bd0 {
    f32 x;
    f32 y;
    UnkProbePoint_l596bd0(f32 x_, f32 y_) : x(x_), y(y_) {}
};

struct UnkProbeSegment_l596bd0 {
    UnkProbePoint_l596bd0 start;
    UnkProbePoint_l596bd0 end;
    UnkProbePoint_l596bd0 delta;
    UnkProbeSegment_l596bd0(const UnkProbePoint_l596bd0& s, const UnkProbePoint_l596bd0& e)
        : start(s), end(e), delta(e.x - s.x, e.y - s.y) {}
};

// fn_7100ebfa7c's result: no cell (0xffff), no side (0xff), points cleared.
struct UnkProbeResult_l596bd0 {
    u16 cell;
    u8 side;
    UnkProbePoint_l596bd0 points[5];
    f32 distance;
    UnkProbeResult_l596bd0()
        : cell(0xffff), side(0xff),
          points{{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}}, distance(0.0f) {}
};

static inline UnkNavMeshManager* NavMeshManager() {
    UnkNavMeshManager* manager = g_unk_71726a4510;
    if (!manager) {
        manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(manager);
        g_unk_71726a4510 = manager;
    }
    return manager;
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_7100596bd0(UnkGooplotUndoJump_l596bd0* self) {
    const f32 half_pi = 1.57079637f;
    UnkGooplotAI_l596bd0* ai = self->owner->ai;
    fn_710059ae78(ai);
    UnkReflObject* object = self->owner->movement;
    UnkGooplotMovement_l596bd0* movement =
        object && fn_710009d50c(object->vf_0x0(), fn_71010c74e0())
            ? reinterpret_cast<UnkGooplotMovement_l596bd0*>(object)
            : nullptr;

    UnkJumpParams_l596bd0 params;
    if (ai->state == 1)
        ai->field_0x14871 = false;
    params.dir.value = self->dir;
    params.dir.set = true;
    params.field_0x3c = false;
    params.field_0x3d = true;
    params.field_0x8 = self->field_0x80;
    params.field_0xc = true;
    fn_71010c7e6c(reinterpret_cast<UnkEffectSink*>(movement), &params, 1);
    if (movement->field_0xf9 != true) {
        movement->field_0xf9 = true;
        movement->vf_0x260();
    }

    UnkVec3f angle(0.0f, half_pi, 0.0f);
    angle.x = -(atan2f(-self->dir.y, -self->dir.x) + half_pi);
    // The result is dropped in the original.
    fn_710007a220(&angle);
    UnkGooplotOwner_l596bd0* owner = self->owner;
    owner->xform.SetAngle(angle);
    owner->field_0x3c0 = true;

    u32 flags = fn_7100413fd0(ai->nav_source);
    UnkNavMeshManager* manager = NavMeshManager();
    UnkProbeResult_l596bd0 result;
    f32 width = fn_7100977bec(self->owner->field_0x218);
    UnkVec3f from;
    from = self->owner->xform.GetTranslation();
    UnkProbePoint_l596bd0 start(from.x, from.y);
    UnkProbePoint_l596bd0 end(self->dir.x * 120.0f + from.x, self->dir.y * 120.0f + from.y);
    UnkProbeSegment_l596bd0 segment(start, end);
    if (!fn_7100ebfa7c(manager, reinterpret_cast<Vec2*>(&segment), reinterpret_cast<Result*>(&result), width,
                       flags, -1.0f)) {
        owner = self->owner;
        UnkVec3f pos = owner->xform.GetTranslation();
        owner->xform.SetTranslation(
            UnkVec3f(pos.x + self->dir.x * 120.0f, pos.y + self->dir.y * 120.0f, pos.z + self->dir.z * 120.0f));
        owner->field_0x3b0 = true;
    }
}
