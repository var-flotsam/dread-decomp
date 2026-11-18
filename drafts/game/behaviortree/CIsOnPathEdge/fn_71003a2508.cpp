// behaviortree::CIsOnPathEdge's unit (tools/tu_map.py): whether the owner, moving along its path, is
// within the condition's distance (+0x3c) of the path's last point going forwards (+0x38) or its first
// going backwards. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/math/CVectorOps.h"
#include "base/math/UnkSegment2D.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CEnemyMovement.h"
#include "types.h"

using base::math::CVector2D;
using base::math::Length;
using base::math::operator-;
using base::reflection::UnkReflObject;

struct UnkPathPoint {
    CVector2D position;
    f32 field_0x8;
};

struct UnkPath {
    u8 pad_0x0[0x68];
    UnkPathPoint* points;  // 0x68
    u32 count;             // 0x70
    u8 pad_0x74[0x20];
    bool looped;  // 0x94

    u32 SegmentCount() const { return count - (!looped && count != 0); }
    const UnkPathPoint& Next(u32 index) const { return index == count - 1 ? points[0] : points[index + 1]; }
};

struct UnkPathMovement : UnkReflObject {
    u8 pad_0x8[0xf1];
    bool field_0xf9;
    bool field_0xfa;
    u8 pad_0xfb[0x879];
    CVector2D position;  // 0x974
    u8 pad_0x97c[8];
    u32 segment;  // 0x984
    u8 pad_0x988[0x18];
    CVector2D start;  // 0x9a0
    CVector2D end;    // 0x9a8
};
static_assert(__builtin_offsetof(UnkPathMovement, position) == 0x974, "layout");
static_assert(__builtin_offsetof(UnkPathMovement, end) == 0x9a8, "layout");

struct UnkPathOwner {
    u8 pad_0x0[0x1f0];
    UnkReflObject* movement;  // 0x1f0
    u8 pad_0x1f8[0x48];
    UnkPath* path;  // 0x240
};

struct UnkIsOnPathEdge {
    u8 pad_0x0[0x18];
    UnkPathOwner* owner;  // 0x18
    u8 pad_0x20[0x18];
    bool forward;    // 0x38
    f32 distance;    // 0x3c
};

bool fn_71003a2508(UnkIsOnPathEdge* self) {
    UnkReflObject* raw = self->owner->movement;
    UnkPathMovement* movement = nullptr;
    if (raw && fn_710009d50c(raw->vf_0x0(), fn_71010c74e0()))
        movement = static_cast<UnkPathMovement*>(raw);
    if (fn_71010cc3a4(movement) != 1 || movement->field_0xf9 || movement->field_0xfa)
        return false;
    UnkPath* path = self->owner->path;
    u32 segments = path->SegmentCount();
    if (segments == 0)
        return true;
    const CVector2D& target = self->forward ? movement->end : movement->start;
    u32 index = movement->segment;
    f32 travelled = Length(movement->position - target);
    u32 last = segments - 1;
    bool at_end = index == (self->forward ? last : 0);
    bool in_range = travelled < self->distance;
    while (!at_end && in_range) {
        index += self->forward ? 1 : -1;
        UnkSegment2D segment(path->points[index].position, path->Next(index).position);
        travelled += fn_7100125fbc(&segment);
        at_end = index == (self->forward ? last : 0);
        in_range = travelled < self->distance;
    }
    return at_end && in_range;
}
