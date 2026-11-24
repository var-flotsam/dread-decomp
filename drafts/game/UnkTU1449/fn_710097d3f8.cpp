// Unit 1449: steers the body sideways around what is in front of it: queries the collision tree with the
// body's shape at the aim's position, averages up to two hits, and moves the body along the side of the
// line to the target that the hits are not on. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/CCharacterMovement.h"
#include "game/CMovementComponent.h"
#include "game/CSamusGrappleMovementState.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1450.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU54.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern void* g_unk_7101a85cd8[];

// The target: a position source with a virtual 0x188 that gives its position.
class UnkTarget_l97d3f8 {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual void vf_0x170();
    virtual void vf_0x178();
    virtual void vf_0x180();
    virtual UnkVec3f vf_0x188();
};

struct UnkMatrix44_7100078c9c {
    f32 m[16];
};

// The grapple movement tunable (g_unk_717273ba88).
struct UnkGrappleTunable_l97d3f8 {
    u8 pad_0x0[0x48];
    f32 field_0x48;
};

static inline const UnkGrappleTunable_l97d3f8& GrappleTunable() {
    return reinterpret_cast<const UnkGrappleTunable_l97d3f8&>(g_unk_717273ba88);
}

// The body moved: its collision mask and position.
struct UnkBody_l97d3f8 {
    u8 pad_0x0[0x1d8];
    base::math::CVector3D pos;  // 0x1d8
    u8 pad_0x1e4[0x3cc - 0x1e4];
    u32 mask;  // 0x3cc
};

struct UnkCollider_l97d3f8 {
    u8 pad_0x0[0x158];
    UnkShape* shape;  // 0x158
};

struct UnkOwner_l97d3f8 {
    u8 pad_0x0[0x38];
    UnkBody_l97d3f8* body;  // 0x38
};

struct UnkLink_l97d3f8 {
    u8 pad_0x0[8];
    UnkOwner_l97d3f8* owner;  // 0x8
};

struct UnkSelf_l97d3f8 {
    u8 pad_0x0[8];
    UnkLink_l97d3f8* link;  // 0x8
};

// Two 3x4 transforms after a scale: only their translations are read.
struct UnkAim_l97d3f8 {
    f32 scale;  // 0x0
    u8 pad_0x4[0xc];
    f32 from_x;  // 0x10
    u8 pad_0x14[0xc];
    f32 from_y;  // 0x20
    u8 pad_0x24[0xc];
    f32 from_z;  // 0x30
    u8 pad_0x34[0xc4 - 0x34];
    f32 at_x;  // 0xc4
    u8 pad_0xc8[0xc];
    f32 at_y;  // 0xd4
    u8 pad_0xd8[0xc];
    f32 at_z;  // 0xe4
};

static inline void ReleaseRef_l97d3f8(UnkRefObject* p) {
    if (p && --p->ref_count == 0)
        p->vf_0x10();
}

#pragma pack(push, 4)
struct UnkHitRecord_l97d3f8 {
    UnkRefObject* first;
    UnkRefObject* second;
    u8 pad_0x10[4];
    base::math::CVector3D point;  // 0x14
    u8 pad_0x20[0x28];
};

// The hits of one query (vtable g_unk_7101a85cd8), up to 4.
struct UnkHitList_l97d3f8 {
    void** vtable;
    u32 count;
    u8 storage[4 * sizeof(UnkHitRecord_l97d3f8)];  // 0xc

    UnkHitList_l97d3f8() : count(0) { vtable = g_unk_7101a85cd8 + 2; }

    UnkHitRecord_l97d3f8* items() { return reinterpret_cast<UnkHitRecord_l97d3f8*>(storage); }

    ~UnkHitList_l97d3f8() {
        vtable = g_unk_7101a85cd8 + 2;
        for (u64 i = 0; i < count; i++) {
            ReleaseRef_l97d3f8(items()[i].second);
            ReleaseRef_l97d3f8(items()[i].first);
        }
        count = 0;
    }
};
#pragma pack(pop)
static_assert(sizeof(UnkHitRecord_l97d3f8) == 0x48, "record");

static inline base::math::CVector3D Cross(const base::math::CVector3D& a, const base::math::CVector3D& b) {
    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

static inline base::math::CVector3D Sub(const UnkVec3f& a, const UnkVec3f& b) {
    base::math::CVector3D r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
    return r;
}

static inline bool Query(UnkTree* tree, u32 mask, UnkShape* shape, const UnkMatrix44_7100078c9c& xf,
                         UnkHitList_l97d3f8* hits) {
    return QueryCollisionTreeForTransformedShapeOverlaps(tree, mask, shape, reinterpret_cast<const UnkMtx44*>(&xf), hits, 1);
}

static inline void MoveTo(UnkBody_l97d3f8* body, const base::math::CVector3D& pos) {
    fn_71010d08d4(reinterpret_cast<UnkMovement_71010d08d4*>(body),
                  reinterpret_cast<const UnkMovementVec_71010d08d4*>(&pos), true);
}

void fn_710097d3f8(UnkSelf_l97d3f8* self, const UnkAim_l97d3f8* aim) {
    UnkTarget_l97d3f8* target =
        reinterpret_cast<UnkTarget_l97d3f8*>(fn_710097f1e0(reinterpret_cast<UnkUnit1452_b56*>(self)));
    base::math::CVector2D to_target;
    {
        base::math::CVector3D d = Sub(target->vf_0x188(), UnkVec3f(aim->from_x, aim->from_y, aim->from_z));
        fn_710007b430(&to_target, &d);
    }
    UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);
    UnkBody_l97d3f8* body = self->link->owner->body;
    UnkCollider_l97d3f8* collider =
        reinterpret_cast<UnkCollider_l97d3f8*>(fn_7100976588(reinterpret_cast<UnkFlagged*>(body)));
    UnkVec3f at(aim->at_x, aim->at_y, aim->at_z);
    UnkHitList_l97d3f8 hits;
    if (!Query(tree, body->mask, collider->shape, BuildTranslationMatrixFromVec3(at), &hits))
        return;
    if (hits.count > 2)
        return;

    UnkVec3f sum(0.0f, 0.0f, 0.0f);
    for (UnkHitRecord_l97d3f8* it = hits.items(); it != hits.items() + hits.count; ++it) {
        sum.x += it->point.x;
        sum.y += it->point.y;
        sum.z += it->point.z;
    }
    f32 n = (f32)hits.count;

    // Turn away from the hits: the side of the line to the target they are not on.
    base::math::CVector3D side;
    {
        base::math::CVector3D d = Sub(UnkVec3f(sum.x / n, sum.y / n, sum.z / n), at);
        base::math::CVector2D to_hits;
        fn_710007b430(&to_hits, &d);
        f32 turn = to_target.x * to_hits.y - to_target.y * to_hits.x;
        base::math::CVector3D up = {0.0f * turn, 0.0f * turn, turn};
        side = Cross(base::math::CVector3D{to_target.x, to_target.y, 0.0f}, up);
    }
    f32 len = __builtin_sqrtf(side.x * side.x + side.y * side.y + side.z * side.z);
    f32 inv = 1.0f / (len + 0.0001f);
    f32 speed = GrappleTunable().field_0x48;
    f32 scale = aim->scale;
    MoveTo(body, {body->pos.x + scale * (speed * (side.x * inv)), body->pos.y + scale * (speed * (side.y * inv)),
                  scale * (speed * (side.z * inv)) + body->pos.z});
}
