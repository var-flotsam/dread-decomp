// CMorphBallMovement's unit (hint only): rolls the ball around the circle it touches. The circle's turn since
// last time (+0x6f8) is added to the ball's angle about its centre; the owner is placed on the rim at the two
// radii and the contact (+0x128) and its copy (+0x178) get the touching point and the normal. Returns the
// turn, or zero without a contact between two circles. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "game/CCharacterMovement.h"
#include "game/UnkVec3fOps.h"
#include "game/UnkXformNode.h"
#include "game/logic/collision/CCollider.h"
#include "types.h"

// A collision shape: its kind (2 a circle) and, for a circle, its radius.
struct UnkRollShape_l9433ac {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 vf_0x18();
    u8 pad_0x8[0xa0];
    f32 radius;  // 0xa8
};

struct UnkRollCollider_l9433ac {
    u8 pad_0x0[0x158];
    UnkRollShape_l9433ac* shape;  // 0x158
};

struct UnkRollLink_l9433ac {
    u8 pad_0x0[0x10];
    UnkRollCollider_l9433ac* target;  // 0x10
};

// A collision hit (0x48 bytes).
struct UnkRollHit_l9433ac {
    UnkRollLink_l9433ac* link;
    void* field_0x8;
    s32 id;
    UnkVec3f point;
    UnkVec3f tangent;
    UnkVec3f normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;
};
static_assert(sizeof(UnkRollHit_l9433ac) == 0x48, "UnkRollHit_l9433ac layout");

struct UnkRollOwner_l9433ac {
    void** vtable;
    u8 pad_0x8[0x3c];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64

    UnkVec3f Position() const { return UnkVec3f(x, y, z); }
    void vf_0xd0(const UnkVec3f* pos, bool flag) {
        reinterpret_cast<void (*)(UnkRollOwner_l9433ac*, const UnkVec3f*, bool)>(vtable[0xd0 / 8])(this, pos, flag);
    }
};

struct UnkRollBall_l9433ac {
    u8 pad_0x0[0x30];
    UnkRollOwner_l9433ac* owner;  // 0x30
    u8 pad_0x38[0x128 - 0x38];
    UnkRollHit_l9433ac contact;  // 0x128
    u8 pad_0x170[0x8];
    UnkRollHit_l9433ac previous;  // 0x178
    u8 pad_0x1c0[0x6f8 - 0x1c0];
    f32 angle;  // 0x6f8
};
static_assert(__builtin_offsetof(UnkRollBall_l9433ac, angle) == 0x6f8, "layout");

static inline UnkVec3f Translation(const UnkMatrix44& m) {
    return UnkVec3f(m.m[3], m.m[7], m.m[11]);
}

static inline UnkRollShape_l9433ac* AsCircle(UnkRollShape_l9433ac* shape) {
    return shape->vf_0x18() == 2 ? shape : nullptr;
}

f32 fn_71009433ac(UnkRollBall_l9433ac* self) {
    f32 result = 0.0f;
    if (self->contact.link && self->contact.link->target) {
        UnkMatrix44 m = fn_7100d36a2c(reinterpret_cast<UnkShape*>(self->contact.link->target));
        f32 angle = atan2f(m.m[5], m.m[1]);
        f32 turn = angle - self->angle;
        self->angle = angle;

        UnkVec3f pos = self->owner->Position();
        UnkVec3f offset = pos - Translation(m);
        UnkVec3f dir = offset * (1.0f / (Length(offset) + 0.0001f));
        f32 around = turn + atan2f(dir.y, dir.x);
        f32 c = cosf(around);
        f32 s = sinf(around);

        UnkRollShape_l9433ac* circle = AsCircle(self->contact.link->target->shape);
        UnkRollShape_l9433ac* own = AsCircle(
            reinterpret_cast<UnkRollCollider_l9433ac*>(fn_7100976588(reinterpret_cast<UnkFlagged*>(self)))->shape);
        if (circle && own) {
            UnkVec3f normal(c, s, 0.0f);
            pos = Translation(m) + normal * (circle->radius + own->radius);
            self->owner->vf_0xd0(&pos, true);

            UnkVec3f point = Translation(m) + normal * circle->radius;
            UnkRollHit_l9433ac& hit = self->contact;
            hit.id = -1;
            hit.point = point;
            hit.tangent = -normal;
            hit.normal = normal;
            hit.flags |= 0x38;

            UnkRollHit_l9433ac& copy = self->previous;
            copy.id = hit.id;
            copy.point = hit.point;
            copy.tangent = hit.tangent;
            copy.normal = hit.normal;
            copy.field_0x38 = hit.field_0x38;
            copy.flags = hit.flags;
            result = turn;
        }
    }
    return result;
}