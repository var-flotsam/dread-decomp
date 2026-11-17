// Unit 2702 (hint CAIComponent): an AI component's limits: the two ends of its path (moved inwards
// by the offsets at +0x13c0 and +0x13c4 for kind 1), or, with no path, the first and last nodes of
// its navigation or logic path, in increasing x. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkRefHandle.h"
#include "base/math/CVector.h"
#include "base/math/CVectorOps.h"
#include "base/math/UnkSegment2D.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/UnkTU2702.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkNavComponent_b66a4;
struct UnkNavPath_b66a4;
UnkNavPath_b66a4* fn_71006f4e78(UnkNavComponent_b66a4* self);

struct UnkRefl_fn_7100385cd8;
template <>
base::reflection::UnkReflObject*
base::reflection::refl_FindTyped<UnkRefl_fn_7100385cd8>(void*);

// A node list of a navigation or logic path: its size, its entries, and a node's position.
struct UnkPathNode_l0fc864 {
    void** vtable;

    const UnkVec3f* Position() {
        return reinterpret_cast<const UnkVec3f* (*)(UnkPathNode_l0fc864*)>(vtable[0x18 / 8])(this);
    }
    s32 Count() { return reinterpret_cast<s32 (*)(UnkPathNode_l0fc864*)>(vtable[0x28 / 8])(this); }
    UnkPathNode_l0fc864* At(s32 i) {
        return reinterpret_cast<UnkPathNode_l0fc864* (*)(UnkPathNode_l0fc864*, s32)>(vtable[0x30 / 8])(this, i);
    }
};

// The "LOGICPATH" object: its node list at +0x30.
struct UnkLogicPath_l0fc864 {
    u8 pad_0x0[0x30];
    UnkPathNode_l0fc864 nodes;  // 0x30
};

struct UnkOwner_l0fc864 {
    u8 pad_0x0[0x64];
    f32 z;  // 0x64
    u8 pad_0x68[0x230];
    UnkNavComponent_b66a4* navigation;  // 0x298
};

struct UnkKind_l0fc864 {
    u32 value;
};

struct UnkSelf_l0fc864 {
    u8 pad_0x0[0x30];
    UnkOwner_l0fc864* owner;  // 0x30
    u8 pad_0x38[0x30];
    CVector3D* points;  // 0x68
    u32 count;          // 0x70
    u8 pad_0x74[0x20];
    u8 closed;  // 0x94
    u8 pad_0x95[0x12c3];
    u8 logic_path_ref[0x28];  // 0x1358, the reflected reference to the logic path's actor
    u32 logic_path_state;     // 0x1380, 1 until the reference is resolved
    u8 pad_0x1384[4];
    base::UnkRefHandle logic_path;  // 0x1388
    u8 pad_0x1390[0x30];
    f32 start_offset;  // 0x13c0
    f32 end_offset;    // 0x13c4

    // The path's points other than a closing one.
    u32 Open() const {
        bool open = closed == 0;
        return (count != 0) & open;
    }

    UnkSegment2D Segment(u32 i) const {
        return UnkSegment2D(*reinterpret_cast<const CVector2D*>(&points[i]),
                            *reinterpret_cast<const CVector2D*>(i + 1 == count ? &points[0] : &points[i + 1]));
    }

    // The path's first point, at the owner's z.
    UnkVec3f First() const {
        if (count == Open())
            return UnkVec3f(0.0f, 0.0f, 0.0f);
        UnkSegment2D seg(*reinterpret_cast<const CVector2D*>(points),
                         *reinterpret_cast<const CVector2D*>(count == 1 ? points : points + 1));
        f32 z = owner->z;
        return UnkVec3f(seg.start.x, seg.start.y, z);
    }

    // The end of the path's last segment, at the owner's z.
    UnkVec3f Last() const {
        u32 n = count - Open();
        if (n == 0)
            return UnkVec3f(0.0f, 0.0f, 0.0f);
        UnkSegment2D seg(*reinterpret_cast<const CVector2D*>(&points[n - 1]),
                         *reinterpret_cast<const CVector2D*>(Open() ? &points[n] : &points[0]));
        f32 z = owner->z;
        return UnkVec3f(seg.end.x, seg.end.y, z);
    }
};
static_assert(__builtin_offsetof(UnkSelf_l0fc864, logic_path_state) == 0x1380, "UnkSelf_l0fc864 layout");
static_assert(__builtin_offsetof(UnkSelf_l0fc864, start_offset) == 0x13c0, "UnkSelf_l0fc864 layout");

// Scales to unit length through the reciprocal, leaving a zero vector as it is.
static inline void NormalizeRecip(CVector2D& v) {
    f32 length = base::math::Length(v);
    if (length != 0.0f) {
        f32 inv = 1.0f / length;
        v.x *= inv;
        v.y *= inv;
    }
}

bool fn_71010fc864(UnkBigFistAI3_ovb0050* ai, UnkBigFistKind_ovb0050* kind_arg, UnkBigFistLimit_ovb0050* min_arg,
                   UnkBigFistLimit_ovb0050* max_arg) {
    UnkSelf_l0fc864* self = reinterpret_cast<UnkSelf_l0fc864*>(ai);
    UnkKind_l0fc864* kind = reinterpret_cast<UnkKind_l0fc864*>(kind_arg);
    UnkVec3f* min = reinterpret_cast<UnkVec3f*>(min_arg);
    UnkVec3f* max = reinterpret_cast<UnkVec3f*>(max_arg);

    u32 n = self->count - self->Open();
    if (n == 0) {
        UnkPathNode_l0fc864* nodes = nullptr;
        if (UnkNavComponent_b66a4* navigation = self->owner->navigation)
            nodes = reinterpret_cast<UnkPathNode_l0fc864*>(fn_71006f4e78(navigation));
        if (!nodes) {
            if (self->logic_path_state == 1) {
                {
                    UnkRefPtr ref = fn_710023b580(self->logic_path_ref);
                    fn_7100234f78(&self->logic_path, reinterpret_cast<base::UnkRefHandle*>(&ref));
                }
                self->logic_path_state = 2;
            }
            UnkWeakHandle* handle = reinterpret_cast<UnkWeakHandle*>(self->logic_path.ptr);
            if (!handle)
                return false;
            if (!handle->target)
                return false;
            base::reflection::UnkReflObject* found =
                base::reflection::refl_FindTyped<UnkRefl_fn_7100385cd8>(handle->target);
            if (!found)
                return false;
            nodes = &reinterpret_cast<UnkLogicPath_l0fc864*>(found)->nodes;
        }
        if (!nodes->Count() || !nodes->At(0)->Count())
            return false;
        UnkPathNode_l0fc864* list = nodes->At(0);
        UnkPathNode_l0fc864* first = list->At(0);
        UnkPathNode_l0fc864* last = list->At(list->Count() - 1);
        if (!first || !last)
            return false;
        *min = *first->Position();
        *max = *last->Position();
        if (min->x > max->x) {
            UnkVec3f t = *min;
            *min = *max;
            *max = t;
        }
        return true;
    }

    *min = self->First();
    *max = self->Last();
    if (kind->value != 1)
        return true;

    // Move the start along the path by the start offset.
    f32 offset = self->start_offset;
    f32 left = offset;
    for (u32 i = 0; i < n && !(offset <= 0.0f); i++) {
        UnkSegment2D seg = self->Segment(i);
        f32 length = fn_7100125fbc(&seg);
        if (left > length) {
            left -= length;
        } else {
            CVector2D origin = seg.start;
            CVector2D dir = seg.end - seg.start;
            NormalizeRecip(dir);
            *min = UnkVec3f(origin.x + left * dir.x, origin.y + left * dir.y, 0.0f);
            break;
        }
    }

    // Move the end back along the path by the end offset.
    offset = self->end_offset;
    left = offset;
    for (u32 i = n - 1; i < n && !(offset <= 0.0f); i--) {
        UnkSegment2D seg = self->Segment(i);
        f32 length = fn_7100125fbc(&seg);
        if (left > length) {
            left -= length;
        } else {
            CVector2D origin = seg.end;
            CVector2D dir = seg.end - seg.start;
            NormalizeRecip(dir);
            *max = UnkVec3f(origin.x - dir.x * left, origin.y - dir.y * left, 0.0f);
            break;
        }
    }
    return true;
}
