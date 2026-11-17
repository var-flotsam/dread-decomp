// CAutsniperShootAttack's unit (tools/tu_map.py unit 576): aims the shot. The direction is the one from the
// attack's start (+0x58) to its target (+0x70) when it is within 0.2 of the owner's up axis, else that axis;
// the aim point lies 2500 along it, cut short where a segment query hits something, and the flag is set when
// the first query hits the target's body. Names and layouts are placeholders.
#include <algorithm>
#include <math.h>
#include <string.h>

#include "base/UnkDestructors.h"
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "game/UnkTU55.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkTree;
struct UnkBox;
struct UnkVec3;
struct UnkEntity;
struct UnkPlayerMovementCache;
void* fn_71010d7a6c(UnkPlayerMovementCache* self);

// The key of the owner's up axis.
extern const u8 g_unk_71016492a8[];

// The bodies a collider belongs to, copied by value.
struct __attribute__((packed, aligned(4))) UnkBodyList_l3d7c70 {
    u32 count;
    void* bodies[4];

    UnkBodyList_l3d7c70(const UnkBodyList_l3d7c70& other) {
        memcpy(bodies, other.bodies, other.count * sizeof(void*));
        count = other.count;
    }
    bool Contains(void* body) const {
        return std::find(bodies, bodies + count, body) != bodies + count;
    }
};

struct UnkHitCollider_l3d7c70 {
    u8 pad_0x0[0x164];
    UnkBodyList_l3d7c70 bodies;  // 0x164
};

struct UnkHitObject_l3d7c70 : base::UnkRefCounted {
    UnkHitCollider_l3d7c70* collider;  // 0x10
};

// A reference to a counted object, released when the count drops to zero.
struct UnkHitRef_l3d7c70 {
    UnkHitObject_l3d7c70* ptr;

    UnkHitRef_l3d7c70() : ptr(nullptr) {}
    ~UnkHitRef_l3d7c70() {
        if (UnkHitObject_l3d7c70* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
};

struct UnkHitVec_l3d7c70 {
    f32 x, y, z;

    UnkHitVec_l3d7c70(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

// A collision hit (0x48 bytes): the object hit and a second reference, an id, the point, two vectors and
// flags.
struct UnkHit_l3d7c70 {
    UnkHitRef_l3d7c70 object;
    UnkHitRef_l3d7c70 field_0x8;
    s32 id;
    UnkHitVec_l3d7c70 point;
    UnkHitVec_l3d7c70 field_0x20;
    UnkHitVec_l3d7c70 normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkHit_l3d7c70()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
};
static_assert(sizeof(UnkHit_l3d7c70) == 0x48, "UnkHit_l3d7c70 layout");

// The segment query's callback object: the closest hit so far and the segment's origin. The vtables
// (reached through the GOT) are the callback's and its base's.
struct UnkSegmentQuery_l3d7c70 {
    void** vtable;
    UnkHit_l3d7c70 hit;
    bool field_0x50;
    u32 field_0x54;
    CVector3D origin;
    u64 field_0x68;

    explicit UnkSegmentQuery_l3d7c70(const CVector3D& o) {
        vtable = g_unk_7101a20fa8 + 2;
        field_0x50 = false;
        origin = o;
        field_0x68 = 0;
    }
    ~UnkSegmentQuery_l3d7c70() { vtable = g_unk_7101a20ed8 + 2; }
};

// The segment fn_7100d5c518 tests: its two ends and the vector between them.
struct UnkSegment_l3d7c70 {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkSegment_l3d7c70(const CVector2D& s, const CVector2D& e) : start(s), end(e) {
        delta.x = e.x - s.x;
        delta.y = e.y - s.y;
    }
};

// The aim point, returned by value.
struct UnkAimPoint_l3d7c70 {
    f32 x, y;

    UnkAimPoint_l3d7c70() {}
    UnkAimPoint_l3d7c70(const UnkAimPoint_l3d7c70& o) : x(o.x), y(o.y) {}
};

struct UnkSniperTarget_l3d7c70 {
    u8 pad_0x0[0x200];
    UnkPlayerMovementCache* movement;  // 0x200
};

struct UnkSniperShootAttack_l3d7c70 {
    void** vtable;
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x48];
    CVector3D start;  // 0x58
    u8 pad_0x64[0xc];
    CVector3D target;  // 0x70
};
static_assert(__builtin_offsetof(UnkSniperShootAttack_l3d7c70, target) == 0x70, "layout");

static inline CVector3D Normalized(f32 x, f32 y, f32 z) {
    f32 inv = 1.0f / (sqrtf(x * x + y * y + z * z) + 0.0001f);
    return {x * inv, y * inv, z * inv};
}

static inline bool Near(f32 v, f32 c) {
    return c - 0.2f <= v && c + 0.2f >= v;
}

static inline CVector2D Project(const CVector3D& v) {
    CVector2D p;
    fn_710007b430(&p, &v);
    return p;
}

UnkAimPoint_l3d7c70 fn_71003d7c70(UnkSniperShootAttack_l3d7c70* self, bool* hit_body, CVector2D* out_dir) {
    UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);

    UnkMatrix44 m;
    fn_71011735e8(static_cast<UnkEntity*>(self->owner->field_0x228), g_unk_71016492a8, &m, 0);
    CVector3D up = Normalized(m.m[1], m.m[5], m.m[9]);
    CVector3D dir = Normalized(self->target.x - self->start.x, self->target.y - self->start.y,
                               self->target.z - self->start.z);
    const CVector3D* aim = Near(dir.x, up.x) && Near(dir.y, up.y) && Near(dir.z, up.z) ? &dir : &up;
    fn_710007b444(out_dir, reinterpret_cast<const UnkVec3*>(aim));

    UnkAimPoint_l3d7c70 result;
    CVector2D from = Project(self->start);
    f32 reach_x = out_dir->x * 2500.0f;
    f32 reach_y = out_dir->y * 2500.0f;
    result.x = reach_x + from.x;
    result.y = reach_y + from.y;
    CVector2D end;
    end.x = result.x;
    end.y = result.y;
    UnkSegment_l3d7c70 segment(Project(self->start), end);

    UnkActor* target = fn_7100d7c990(self->owner);
    if (void* body = fn_71010d7a6c(reinterpret_cast<UnkSniperTarget_l3d7c70*>(target)->movement)) {
        UnkSegmentQuery_l3d7c70 query(self->start);
        bool hit = fn_7100d5c518(tree, 0x80429, reinterpret_cast<const UnkBox*>(&segment), &query, 2);
        UnkHitObject_l3d7c70* object = query.hit.object.ptr;
        if (object && object->collider) {
            UnkBodyList_l3d7c70 bodies(object->collider->bodies);
            if (bodies.count != 0 && hit && bodies.Contains(body))
                *hit_body = true;
        }
    }

    UnkSegmentQuery_l3d7c70 query(self->start);
    if (fn_7100d5c518(tree, 0x80409, reinterpret_cast<const UnkBox*>(&segment), &query, 2)) {
        CVector2D point = Project(*reinterpret_cast<const CVector3D*>(&query.hit.point));
        result.x = point.x;
        result.y = point.y;
    }
    return result;
}
