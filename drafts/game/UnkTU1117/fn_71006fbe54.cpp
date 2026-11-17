// Unit 1117 of tools/tu_map.py (no class hint): places the object on its path. Along the path's main
// axis (fixed, or the longer side of the path's bounds), finds the two points around the goal (a set
// position, or the player's), then clamps the goal to them or interpolates the other coordinate
// between them, and records the distance from the owner. Names and layouts are placeholders.
#include <math.h>

#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkPlayer;

struct UnkBounds_l6fbe54 {
    UnkVec3f min;
    UnkVec3f max;
};

// A path, its pieces and their points: a tree of nodes.
struct UnkPathNode_l6fbe54 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const UnkVec3f& Position();  // 0x18
    virtual void vf_0x20();
    virtual u32 ChildCount();                           // 0x28
    virtual UnkPathNode_l6fbe54* Child(u32 index);       // 0x30
    virtual const UnkBounds_l6fbe54& Bounds();          // 0x38
};

struct UnkEntity_l6fbe54 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
};

struct UnkPathFollower_l6fbe54 {
    u8 pad_0x0[0x30];
    UnkEntity_l6fbe54* owner;  // 0x30
    UnkVec3f position;         // 0x38
    u8 pad_0x44[0xc];
    f32 field_0x50;
    u8 pad_0x54[0x74];
    f32 field_0xc8;
    bool has_field_0xc8;  // 0xcc
    u8 pad_0xcd[0x1b];
    UnkPathNode_l6fbe54* path;  // 0xe8
    bool has_path;              // 0xf0
    u8 pad_0xf1[0xb];
    s32 axis;          // 0xfc: 1 along x, 2 along y
    bool fixed_axis;   // 0x100
    bool field_0x101;
    bool field_0x102;
    u8 pad_0x103;
    UnkVec3f goal;     // 0x104
    bool has_goal;     // 0x110
    u8 pad_0x111[0x7];
    bool interpolated;  // 0x118
    u8 pad_0x119[0x3];
    f32 distance;  // 0x11c
};
static_assert(__builtin_offsetof(UnkPathFollower_l6fbe54, distance) == 0x11c, "layout");

// The x (false) or y (true) coordinate.
static inline f32 Component(const UnkVec3f& v, bool y) {
    return (&v.x)[y];
}

// -1, 0 or 1.
static inline f32 Sign(f32 v) {
    return (s32)((v > 0.0f) - (v < 0.0f));
}

void fn_71006fbe54(UnkPathFollower_l6fbe54* self) {
    UnkPathNode_l6fbe54* path = self->has_path ? self->path : nullptr;
    bool along_y;
    if (self->fixed_axis && self->axis == 1) {
        along_y = false;
    } else if (self->fixed_axis && self->axis == 2) {
        along_y = true;
    } else {
        const UnkBounds_l6fbe54& bounds = path->Child(0)->Bounds();
        along_y = !(bounds.max.x - bounds.min.x > bounds.max.y - bounds.min.y);
    }

    UnkVec3f target(0.0f, 0.0f, 0.0f);
    if (self->field_0x102 && self->field_0x101 && self->has_goal)
        target = self->goal;
    else
        target = reinterpret_cast<UnkEntity_l6fbe54*>(fn_7100dec788(g_unk_7101d47310))->xform.GetTranslation();
    f32 goal = Component(target, along_y);
    f32 here = Component(self->owner->xform.GetTranslation(), along_y);

    // The point the owner is nearest to on the goal's side, and the first one past it.
    UnkPathNode_l6fbe54* best = path->Child(0)->Child(0);
    f32 first =
        Sign(Component(best->Position(), along_y) - Component(self->owner->xform.GetTranslation(), along_y));
    UnkPathNode_l6fbe54* next;
    if (path->Child(0)->ChildCount() < 2) {
        next = nullptr;
    } else {
        f32 toward_goal = Sign(goal - here);
        if (first == 0.0f) {
            next = path->Child(0)->Child(1);
        } else {
            u32 i = 1;
            do {
                next = path->Child(0)->Child(i);
                f32 side = Sign(Component(next->Position(), along_y) -
                                Component(self->owner->xform.GetTranslation(), along_y));
                if (first == -side)
                    break;
                if (i != path->Child(0)->ChildCount() - 1)
                    best = side == 0.0f && toward_goal == first ? best : next;
                ++i;
            } while (i < path->Child(0)->ChildCount());
        }
    }

    bool best_higher = Component(best->Position(), along_y) > Component(next->Position(), along_y);
    UnkPathNode_l6fbe54* upper = best_higher ? best : next;
    UnkPathNode_l6fbe54* lower = best_higher ? next : best;
    if (self->has_field_0xc8)
        self->field_0x50 = self->field_0xc8;
    else
        self->field_0x50 = 1.0f;
    if (goal >= Component(upper->Position(), along_y)) {
        UnkVec3f point = upper->Position();
        self->position = point;
        self->interpolated = false;
    } else if (goal <= Component(lower->Position(), along_y)) {
        UnkVec3f point = lower->Position();
        self->position = point;
        self->interpolated = false;
    } else {
        self->interpolated = true;
        bool across = !along_y;
        f32 slope = (Component(lower->Position(), across) - Component(upper->Position(), across)) /
                    (Component(lower->Position(), along_y) - Component(upper->Position(), along_y));
        f32 value = slope * (goal - Component(upper->Position(), along_y)) + Component(upper->Position(), across);
        if (!along_y) {
            self->position.x = target.x;
            self->position.y = value;
        } else {
            self->position.x = value;
            self->position.y = target.y;
        }
        self->position.z = self->owner->xform.m[2][3];
    }
    const UnkXform& owner = self->owner->xform;
    f32 dx = self->position.x - owner.m[0][3];
    f32 dy = self->position.y - owner.m[1][3];
    f32 dz = self->position.z - owner.m[2][3];
    self->distance = sqrtf(dx * dx + dy * dy + dz * dz);
}
