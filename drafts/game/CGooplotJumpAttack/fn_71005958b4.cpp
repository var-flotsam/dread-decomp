// CGooplotJumpAttack unit (tools/tu_map.py): the jump's leap. Lets the AI component know (fn_710059ae78),
// hands the enemy movement a jump along the attack's direction, turns the owner to face it, moves the owner
// 60 units along it unless the navigation mesh has a hit on the way (fn_7100ebfa7c), and queues the reverse
// direction on the AI's list. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkLinkedList.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/global/CRntVector.h"
#include "game/CAutsharpAIComponent.h"
#include "game/CCharClassAIComponent.h"
#include "game/CCharacterMovement.h"
#include "game/CEnemyMovement.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU54.h"
#include "game/UnkVec3fOps.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkSource_7100286798;
struct UnkFlagged;
struct UnkGooplotAI_l5958b4;
void fn_710059ae78(UnkGooplotAI_l5958b4* ai);

// A direction queued on the AI's list.
struct UnkGooplotDirNode_l5958b4 {
    UnkVec3f dir;
    UnkGooplotDirNode_l5958b4* next;  // 0x10
};

struct UnkGooplotAI_l5958b4 {
    u8 pad_0x0[0x10];
    UnkSource_7100286798* nav_source;  // 0x10
    u8 pad_0x18[0x14808 - 0x18];
    base::UnkLinkedList<UnkGooplotDirNode_l5958b4> directions;  // 0x14808
};
static_assert(__builtin_offsetof(UnkGooplotAI_l5958b4, directions) == 0x14808, "layout");

struct UnkGooplotMovement_l5958b4 {
    void** vtable;
    u8 pad_0x8[0xf1];
    bool field_0xf9;

    void vf_0x260() { reinterpret_cast<void (*)(UnkGooplotMovement_l5958b4*)>(vtable[0x260 / 8])(this); }
};

struct UnkGooplotOwner_l5958b4 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x1f0 - 0x94];
    UnkReflObject* movement;  // 0x1f0
    u8 pad_0x1f8[0x20];
    UnkFlagged* field_0x218;
    u8 pad_0x220[0x20];
    UnkGooplotAI_l5958b4* ai;  // 0x240
    u8 pad_0x248[0x3b0 - 0x248];
    bool field_0x3b0;
    u8 pad_0x3b1[0xf];
    bool field_0x3c0;
};
static_assert(__builtin_offsetof(UnkGooplotOwner_l5958b4, field_0x3c0) == 0x3c0, "layout");

struct UnkGooplotJump_l5958b4 {
    void** vtable;
    UnkGooplotOwner_l5958b4* owner;  // 0x8
    u8 pad_0x10[0x54];
    UnkVec3f dir;  // 0x64
    u8 pad_0x70[0x10];
    u32 field_0x80;
};

// A direction that may be unset: its value and flag.
struct UnkOptVec3Data_l5958b4 {
    UnkVec3f value;
    bool set;
};

// The optional wrapper: constructing it clears the value and the flag (up to the flag, 13 bytes).
struct UnkOptVec3_l5958b4 : UnkOptVec3Data_l5958b4 {
    UnkOptVec3_l5958b4() : UnkOptVec3Data_l5958b4() {}
};

// The block handed to the movement (fn_71010c7e6c); the vtable tells the kinds apart.
struct UnkJumpParams_l5958b4 {
    void** vtable;
    u32 field_0x8;
    bool field_0xc;
    u8 pad_0xd[7];
    bool field_0x14;
    UnkOptVec3_l5958b4 field_0x18;
    UnkOptVec3_l5958b4 dir;  // 0x28
    u32 field_0x38;
    bool field_0x3c;
    bool field_0x3d;

    UnkJumpParams_l5958b4() : field_0xc(false), field_0x14(false), field_0x18(), dir() {
        vtable = g_unk_7101a2e208 + 2;
        field_0x3d = false;
        field_0x38 = 3;
    }
};
static_assert(__builtin_offsetof(UnkJumpParams_l5958b4, field_0x38) == 0x38, "layout");

struct UnkProbePoint_l5958b4 {
    f32 x;
    f32 y;
    UnkProbePoint_l5958b4(f32 x_, f32 y_) : x(x_), y(y_) {}
};

struct UnkProbeSegment_l5958b4 {
    UnkProbePoint_l5958b4 start;
    UnkProbePoint_l5958b4 end;
    UnkProbePoint_l5958b4 delta;
    UnkProbeSegment_l5958b4(const UnkProbePoint_l5958b4& s, const UnkProbePoint_l5958b4& e)
        : start(s), end(e), delta(e.x - s.x, e.y - s.y) {}
};

// fn_7100ebfa7c's result: no cell (0xffff), no side (0xff), points cleared.
struct UnkProbeResult_l5958b4 {
    u16 cell;
    u8 side;
    UnkProbePoint_l5958b4 points[5];
    f32 distance;
    UnkProbeResult_l5958b4()
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

void fn_71005958b4(UnkGooplotJump_l5958b4* self) {
    const f32 half_pi = 1.57079637f;
    UnkGooplotAI_l5958b4* ai = self->owner->ai;
    fn_710059ae78(ai);
    UnkReflObject* object = self->owner->movement;
    UnkGooplotMovement_l5958b4* movement =
        object && fn_710009d50c(object->vf_0x0(), fn_71010c74e0())
            ? reinterpret_cast<UnkGooplotMovement_l5958b4*>(object)
            : nullptr;

    UnkJumpParams_l5958b4 params;
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
    WrapAngles3ToPi(&angle);
    UnkGooplotOwner_l5958b4* owner = self->owner;
    owner->xform.SetAngle(angle);
    owner->field_0x3c0 = true;

    u32 flags = fn_7100413fd0(ai->nav_source);
    UnkNavMeshManager* manager = NavMeshManager();
    UnkProbeResult_l5958b4 result;
    f32 width = fn_7100977bec(self->owner->field_0x218);
    UnkVec3f from;
    from = self->owner->xform.GetTranslation();
    UnkProbeSegment_l5958b4 segment(UnkProbePoint_l5958b4(from.x, from.y),
                                    UnkProbePoint_l5958b4(self->dir.x * 60.0f + from.x,
                                                          self->dir.y * 60.0f + from.y));
    if (!fn_7100ebfa7c(manager, reinterpret_cast<Vec2*>(&segment), reinterpret_cast<Result*>(&result), width,
                       flags, -1.0f)) {
        owner = self->owner;
        UnkVec3f pos = owner->xform.GetTranslation();
        owner->xform.SetTranslation(pos + self->dir * 60.0f);
        owner->field_0x3b0 = true;
    }

    base::UnkLinkedList<UnkGooplotDirNode_l5958b4>& directions = ai->directions;
    UnkVec3f back = -self->dir;
    UnkGooplotDirNode_l5958b4* node = static_cast<UnkGooplotDirNode_l5958b4*>(
        directions.allocator->vf_0x40(sizeof(UnkGooplotDirNode_l5958b4), 8, directions.flags));
    if (node) {
        node->dir = back;
        node->next = nullptr;
        directions.PushBack(node);
    }
}
