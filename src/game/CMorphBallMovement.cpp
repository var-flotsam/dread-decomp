// From unit 1428 of tools/tu_map.py (0x710093b200-0x71009506b0, CMorphBallMovement): the offset
// from the actor fn_7100defcd4 gives (through the game manager) to this movement's actor, from the
// centres of their bounds and half their size difference. Names and layouts are placeholders.
// Holds fn_7100943118.
#include "base/math/CVector.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::math::CVector3D;

// A 3D vector returned in memory (its user-provided copy keeps it out of registers).
struct UnkMorphVec3 {
    f32 x;
    f32 y;
    f32 z;

    UnkMorphVec3() {}
    UnkMorphVec3(const UnkMorphVec3& other) : x(other.x), y(other.y), z(other.z) {}
};

// A box (minimum and maximum corner).
struct UnkMorphBounds {
    CVector3D min;
    CVector3D max;
};

// The shape whose bounds vf_0x28 gives (from the transform at +8).
struct UnkMorphShape {
    void** vtable;
    u8 transform[1];

    UnkMorphBounds vf_0x28(const void* t) {
        return reinterpret_cast<UnkMorphBounds (*)(UnkMorphShape*, const void*)>(vtable[0x28 / 8])(this, t);
    }
};

struct UnkMorphCollision {
    u8 pad_0x0[0x158];
    UnkMorphShape* field_0x158;
};

// The actor's component at +0x200; its vf_0x2a8 gives the collision.
struct UnkMorphComponent {
    void** vtable;

    UnkMorphCollision* vf_0x2a8() {
        return reinterpret_cast<UnkMorphCollision* (*)(UnkMorphComponent*)>(vtable[0x2a8 / 8])(this);
    }
};

struct UnkOwner {
    u8 pad_0x0[0x200];
    UnkMorphComponent* field_0x200;
    u8 pad_0x208[0x174];
    f32 field_0x37c;
    u8 pad_0x380[0xc];
    f32 field_0x38c;
    u8 pad_0x390[0x4];
    f32 field_0x394;
};
static_assert(__builtin_offsetof(UnkOwner, field_0x394) == 0x394, "UnkOwner placeholder layout");

UnkOwner* fn_7100defcd4(UnkGameManager* manager);

struct UnkMorphMovement {
    u8 pad_0x0[0x30];
    UnkOwner* field_0x30;
};

static inline UnkMorphBounds Bounds(UnkMorphComponent* component) {
    UnkMorphCollision* collision = component->vf_0x2a8();
    UnkMorphShape* shape = collision ? collision->field_0x158 : nullptr;
    return shape->vf_0x28(shape->transform);
}

// NON_MATCHING(minor): the same arithmetic and results, compiled differently: the original keeps
// the flag's two cases as branches (x - dx or x + dx stored in each, the height difference times
// -0.5 or 0.5 added at the join) and keeps the first box's values in d8-d14 (a 0xa0 frame); ours
// selects between both cases with fcsel, stores x and y once more before the final pair, and uses
// d8-d13 (a 0x90 frame). The stores all go to the returned object, which nothing reads meanwhile.
UnkMorphVec3 fn_7100943118(UnkMorphMovement* self, bool flag) {
    UnkMorphVec3 offset;
    UnkOwner* other = fn_7100defcd4(g_unk_7101d47310);
    UnkOwner* owner = self->field_0x30;
    UnkMorphComponent* component = owner->field_0x200;
    if (other && component && other->field_0x200) {
        UnkMorphBounds mine = Bounds(component);
        f32 mine_w = mine.max.x - mine.min.x;
        f32 mine_x = (mine.max.x + mine.min.x) * 0.5f;
        f32 mine_y = (mine.max.y + mine.min.y) * 0.5f;
        f32 mine_z = (mine.max.z + mine.min.z) * 0.5f;
        f32 mine_h = mine.max.y - mine.min.y;
        UnkMorphBounds theirs = Bounds(other->field_0x200);
        offset.x = mine_x - (theirs.max.x + theirs.min.x) * 0.5f;
        offset.y = mine_y - (theirs.max.y + theirs.min.y) * 0.5f;
        offset.z = mine_z - (theirs.max.z + theirs.min.z) * 0.5f;
        f32 dx = ((theirs.max.x - theirs.min.x) - mine_w) * 0.5f;
        f32 theirs_h = theirs.max.y - theirs.min.y;
        if (flag) {
            offset.x = offset.x - dx;
            offset.y = offset.y + (theirs_h - mine_h) * -0.5f;
        } else {
            offset.x = offset.x + dx;
            offset.y = offset.y + (theirs_h - mine_h) * 0.5f;
        }
    } else if (other) {
        f32 x = other->field_0x37c - owner->field_0x37c;
        f32 y = (owner->field_0x394 - other->field_0x394) + (other->field_0x38c - owner->field_0x38c) * 0.5f;
        offset.x = x;
        offset.y = y;
    } else {
        offset.x = 0.0f;
        offset.y = 0.0f;
    }
    offset.z = 0.0f;
    return offset;
}
