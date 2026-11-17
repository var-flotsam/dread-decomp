// Unit 1442: turns the walker to `angle` along its path, stepping to the next or previous edge of the polygon it
// follows, moves it by its speed, recomputes its direction and the normal that faces its owner, and resets its
// segment to its new position. Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "game/malloc.h"
#include "types.h"

using base::math::CVector2D;

// The polygon the walker follows: `count` points, closed or not.
struct UnkWalkPolygon_l96e480 {
    u8 pad_0x0[0x70];
    u32 count;  // 0x70
    u8 pad_0x74[0x20];
    bool closed;  // 0x94

    u32 EdgeCount() const { return count - (u32)(!closed & (count != 0)); }
};

struct UnkWalkPath_l96e480 {
    u8 pad_0x0[0x240];
    UnkWalkPolygon_l96e480* polygon;  // 0x240
};

struct UnkWalkOwner_l96e480 {
    void** vtable;
    u8 pad_0x8[0x38];
    f32 velocity[3];  // 0x40

    UnkVec3f vf_0x370() { return reinterpret_cast<UnkVec3f (*)(UnkWalkOwner_l96e480*)>(vtable[0x370 / 8])(this); }
};

// The first 0x14 bytes of an UnkSegment2D (its points and cached length), as the walker keeps them.
struct UnkWalkSegment_l96e480 {
    CVector2D start;
    CVector2D end;
    f32 length;

    UnkSegment2D* AsSegment() { return reinterpret_cast<UnkSegment2D*>(this); }
};

// An angle (radians) that fn_710007a170 moves by a full turn toward a target.
struct UnkWalkAngle_l96e480 {
    f32 value;

    void TurnToward(f32 target) { fn_710007a170(reinterpret_cast<UnkAim*>(this), target); }
};

struct UnkWalker_l96e480 {
    u8 pad_0x0[0x30];
    UnkWalkPath_l96e480* path;  // 0x30
    UnkWalkOwner_l96e480* owner;  // 0x38
    u8 pad_0x40[0x49];
    u8 reversed;  // 0x89
    u8 pad_0x8a[0x12];
    u32 edge;  // 0x9c
    u8 pad_0xa0[8];
    bool moved;  // 0xa8
    f32 angle;  // 0xac
    UnkWalkAngle_l96e480 angle_from;  // 0xb0
    UnkWalkAngle_l96e480 angle_to;  // 0xb4
    UnkWalkSegment_l96e480 segment;  // 0xb8
    UnkWalkSegment_l96e480 forward_segment;  // 0xcc
    UnkWalkSegment_l96e480 backward_segment;  // 0xe0
    CVector2D normal;  // 0xf4
    CVector2D direction;  // 0xfc
    CVector2D forward_direction;  // 0x104
    CVector2D backward_direction;  // 0x10c
    CVector2D position;  // 0x114
    u8 pad_0x11c[8];
    u8 forward;  // 0x124
    u8 pad_0x125[7];
    f32 speed;  // 0x12c

    // Whether the walker steps forward along the polygon's edges.
    bool Advancing() const { return (forward != 0) ^ (reversed != 0); }
};
static_assert(__builtin_offsetof(UnkWalker_l96e480, speed) == 0x12c, "UnkWalker layout");

static inline bool IsNearZero(f32 v) { return v < 0.001f && v > -0.001f; }

void fn_710096e480(UnkWalker_l96e480* self, f32 angle) {
    self->angle = angle;
    const CVector2D& saved = self->forward ? self->backward_direction : self->forward_direction;
    UnkWalkAngle_l96e480 from = {atan2f(saved.y, saved.x)};
    UnkWalkAngle_l96e480 to = {atan2f(self->direction.y, self->direction.x)};
    from.TurnToward(self->angle);
    to.TurnToward(self->angle);
    bool advancing = self->Advancing();
    self->angle_from = advancing ? from : to;
    self->angle_to = advancing ? to : from;

    if (self->forward) {
        UnkWalkPolygon_l96e480* polygon = self->path->polygon;
        fn_7100125b44(self->forward_segment.AsSegment(), self->segment.AsSegment());
        if (self->Advancing())
            self->edge = self->edge == polygon->EdgeCount() - 1 ? 0 : self->edge + 1;
        else
            self->edge = self->edge == 0 ? polygon->EdgeCount() - 1 : self->edge - 1;
        self->forward_direction.x = self->direction.x;
        self->forward_direction.y = self->direction.y;
    } else {
        fn_7100125b44(self->backward_segment.AsSegment(), self->segment.AsSegment());
        self->backward_direction.x = self->direction.x;
        self->backward_direction.y = self->direction.y;
    }

    self->position.x = self->speed * self->normal.x + self->position.x;
    self->position.y = self->speed * self->normal.y + self->position.y;
    self->moved = true;
    self->direction = CVector2D{cosf(self->angle), sinf(self->angle)};

    {
        UnkVec3f owner_pos = self->owner->vf_0x370();
        self->normal.x = self->direction.y;
        self->normal.y = -self->direction.x;
        CVector2D to_owner;
        fn_710007b430(&to_owner, reinterpret_cast<const base::math::CVector3D*>(&owner_pos));
        f32 dot = self->normal.x * to_owner.x + self->normal.y * to_owner.y;
        if (dot < -0.001f || (IsNearZero(dot) && !self->reversed)) {
            self->normal.x = -self->normal.x;
            self->normal.y = -self->normal.y;
        }
    }

    self->owner->velocity[0] = 0.0f;
    self->owner->velocity[1] = 0.0f;
    self->owner->velocity[2] = 0.0f;
    UnkSegment2D segment(self->position, self->position);
    fn_7100125b44(self->segment.AsSegment(), &segment);
}
