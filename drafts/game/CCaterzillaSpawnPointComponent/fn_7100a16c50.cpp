// CCaterzillaSpawnPointComponent unit (tools/tu_map.py unit 1527): whether the segment's actor (the
// linked one when there is a next segment, else the owner) lies within 70 units of `pos` and faces a
// usable way out (column 2 of its transform, or column 1 for the "spawnpointcaterzilla..." actors);
// stores the distance when it does. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/UnkRefHandle.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/UnkTU341.h"
#include "game/UnkTU55.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;

struct UnkCaterzillaActor_ovb0054 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkCaterzillaTargetRef_ovb0054 {
    u8 pad_0x0[0x10];
    UnkCaterzillaActor_ovb0054* actor;  // 0x10
};

// A counted reference to the actor's weak handle (a base::UnkRefHandle).
struct UnkTargetHandle_la16c50 {
    UnkCaterzillaTargetRef_ovb0054* ptr;
};

// The segment's link to the actor it follows, resolved from the reflected property on first use.
struct UnkSegmentLink_la16c50 {
    u8 pad_0x0[0x8];
    u8 property[0x28];  // 0x8, a reflected property naming the actor
    u32 state;          // 0x30
    u8 pad_0x34[0x4];
    UnkTargetHandle_la16c50 target;  // 0x38

    UnkCaterzillaTargetRef_ovb0054* Target() {
        if (state == 1) {
            {
                UnkRefPtr ref = fn_710023b580(property);
                fn_7100234f78(reinterpret_cast<base::UnkRefHandle*>(&target),
                              reinterpret_cast<const base::UnkRefHandle*>(&ref));
            }
            state = 2;
        }
        return target.ptr;
    }
};

struct UnkNameHolder_b0161 {
    CStrId id;
};

// The actor seen as its transform (at +0x38; its translation is the x, y, z above).
struct UnkCaterzillaBody_la16c50 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
};

// The component's owner: an actor of a more derived kind than the linked ones.
struct UnkCaterzillaOwner_la16c50 : UnkCaterzillaActor_ovb0054 {};

struct UnkCaterzillaSegment_la16c50 {
    u8 pad_0x0[0x30];
    UnkCaterzillaOwner_la16c50* owner;  // 0x30
    u8 pad_0x38[0x3e0 - 0x38];
    UnkSegmentLink_la16c50 link;  // 0x3e0
    UnkCaterzillaSegment_la16c50* next;  // 0x420

    // The linked actor.
    UnkCaterzillaActor_ovb0054* LinkedActor() {
        UnkCaterzillaTargetRef_ovb0054* target = link.Target();
        return target ? target->actor : nullptr;
    }

    // The actor the segment follows: the linked one while there is a next segment.
    UnkCaterzillaActor_ovb0054* Actor() { return next ? LinkedActor() : owner; }
};
static_assert(__builtin_offsetof(UnkCaterzillaSegment_la16c50, next) == 0x420, "segment layout");

static inline const UnkMatrix44& TransformOf(UnkCaterzillaActor_ovb0054* actor) {
    return fn_710002614c(&reinterpret_cast<UnkCaterzillaBody_la16c50*>(actor)->transform);
}

static inline CVector3D Vector(f32 x, f32 y, f32 z) {
    CVector3D v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

// A point or direction of the level, flattened to the 2D plane.
static inline CVector2D Flatten(f32 x, f32 y, f32 z) {
    CVector3D v = Vector(x, y, z);
    CVector2D out;
    fn_710007b430(&out, &v);
    return out;
}

// The same, assigned to an existing vector.
static inline void AssignFlat(CVector2D* out, f32 x, f32 y, f32 z) {
    CVector3D v = Vector(x, y, z);
    fn_710007b444(out, reinterpret_cast<const UnkVec3*>(&v));
}

static inline f32 Distance(const CVector2D& a, const CVector2D& b) {
    f32 dx = a.x - b.x;
    f32 dy = a.y - b.y;
    return sqrtf(dx * dx + dy * dy);
}

// Whether the id names `name`, counting the read.
static inline bool IsNamed(const CStrId& id, const CRntString& name) {
    ++id.data->reads;
    return id.data->Key() == name.key;
}

static inline bool Near(f32 value, f32 target) {
    return value - 0.001f <= target && value + 0.001f >= target;
}

bool fn_7100a16c50(UnkCaterzillaSegment_la16c50* self, const CVector2D* pos, const CVector2D* forward,
                   const CVector2D* side, f32* out_distance) {
    UnkCaterzillaActor_ovb0054* actor = self->Actor();
    f32 distance = Distance(*pos, Flatten(actor->x, actor->y, actor->z));
    if (!(distance < 70.0f))
        return false;

    CVector2D dir;
    dir.x = 0.0f;
    dir.y = 0.0f;
    if (self->next) {
        const UnkMatrix44& m = TransformOf(self->LinkedActor());
        dir = Flatten(m.m[2], m.m[6], m.m[10]);
        CStrId name = fn_7100232fcc(self->LinkedActor())->id;
        if (IsNamed(name, CRntString("spawnpointcaterzilla")) ||
            IsNamed(name, CRntString("spawnpointcaterzillametallic"))) {
            const UnkMatrix44& n = TransformOf(self->LinkedActor());
            dir = Flatten(n.m[1], n.m[5], n.m[9]);
        }
    } else {
        const UnkMatrix44& m = TransformOf(self->owner);
        AssignFlat(&dir, m.m[2], m.m[6], m.m[10]);
        CStrId name = fn_7100232fcc(self->owner)->id;
        if (IsNamed(name, CRntString("spawnpointcaterzilla")) ||
            IsNamed(name, CRntString("spawnpointcaterzillametallic"))) {
            const UnkMatrix44& n = TransformOf(self->owner);
            AssignFlat(&dir, n.m[1], n.m[5], n.m[9]);
        }
    }

    CVector2D facing = *forward;
    if (Near(dir.x * side->x + dir.y * side->y, -1.0f))
        return false;
    f32 d = dir.x * facing.x + dir.y * facing.y;
    if (d < 0.0f || Near(d, 0.0f)) {
        *out_distance = distance;
        return true;
    }
    return false;
}
