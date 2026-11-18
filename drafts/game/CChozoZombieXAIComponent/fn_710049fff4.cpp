// CChozoZombieXAIComponent's unit (tools/tu_map.py): after the base update, probes the collision tree
// from the target down along the owner's up axis (800 units); when the floor hit lies within 10 of the
// path of points the owner's AI follows, the output's value becomes 700. Names and layouts are
// placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

// A hit object, released through its vtable when its count drops to zero.
struct UnkHitRef_l49fff4 {
    base::UnkRefCounted* ptr;

    UnkHitRef_l49fff4() : ptr(nullptr) {}
    ~UnkHitRef_l49fff4() {
        if (base::UnkRefCounted* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
};

// A collision hit (0x48 bytes): the object hit and a second reference, an id, the point, two
// vectors and flags.
struct UnkHit_l49fff4 {
    UnkHitRef_l49fff4 object;
    UnkHitRef_l49fff4 field_0x8;
    s32 id;
    CVector3D point;
    CVector3D field_0x20;
    CVector3D normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkHit_l49fff4()
        : id(-1), point{0.0f, 0.0f, 0.0f}, field_0x20{0.0f, 0.0f, 0.0f}, normal{0.0f, 0.0f, 0.0f}, field_0x38(0),
          flags(0), field_0x40(0) {}
};
static_assert(sizeof(UnkHit_l49fff4) == 0x48, "UnkHit_l49fff4 layout");

// The segment query's callback object: the closest hit so far and the segment's origin. The
// vtables (reached through the GOT) are the callback's and its base's.
struct UnkSegmentQuery_l49fff4 {
    void** vtable;
    UnkHit_l49fff4 hit;
    bool field_0x50;
    u32 field_0x54;
    CVector3D origin;
    u64 field_0x68;

    explicit UnkSegmentQuery_l49fff4(const CVector3D& o) {
        vtable = g_unk_7101a20fa8 + 2;
        field_0x50 = false;
        origin.x = o.x;
        origin.y = o.y;
        origin.z = o.z;
        field_0x68 = 0;
    }
    ~UnkSegmentQuery_l49fff4() { vtable = g_unk_7101a20ed8 + 2; }
};

// The segment fn_7100d5c518 tests: its two ends and the vector between them.
struct UnkSegment_l49fff4 {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkSegment_l49fff4(const CVector2D& s, const CVector2D& e) {
        start.x = s.x;
        start.y = s.y;
        end.x = e.x;
        end.y = e.y;
        delta.x = e.x - s.x;
        delta.y = e.y - s.y;
    }
};

struct UnkTree;
struct UnkBox;

// A list of points (12-byte entries).
struct UnkPathList_l49fff4 {
    CVector3D* data;
    u32 count;
};

// The AI's path: its points (at +0x68) and whether it closes on itself (+0x94).

struct UnkUnit108List_A1_b73;
struct UnkUnit108Point_A1_b73;
bool fn_7100118b1c(const UnkUnit108List_A1_b73* self, const UnkUnit108Point_A1_b73* p, f32 r);

struct UnkZombieAI_l49fff4 {
    u8 pad_0x0[0x68];
    UnkPathList_l49fff4 path;  // 0x68
    u8 pad_0x78[0x1c];
    bool closed;  // 0x94

    // The path's segments: one fewer than its points unless it is closed.
    u32 SegmentCount() const { return path.count - ((path.count != 0) & !closed); }
};

struct UnkZombieX_l49fff4 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
};

struct UnkZombieXOut_l49fff4 {
    u32 field_0x0;
    f32 value;  // 0x4
};

void fn_7101106ae8(UnkZombieX_l49fff4* self, UnkZombieXOut_l49fff4* out);

static inline CVector3D Position(const UnkActor* actor) {
    return {actor->field_0x44, actor->field_0x54, actor->field_0x64};
}

void fn_710049fff4(UnkZombieX_l49fff4* self, UnkZombieXOut_l49fff4* out) {
    fn_7101106ae8(self, out);
    UnkActor* target = fn_7100d7c990(self->owner);
    if (!target || !target->field_0x208 || fn_71009e12d0(target->field_0x208))
        return;

    CVector2D from = {target->field_0x44, target->field_0x54};
    const UnkMatrix44& m =
        fn_710002614c(reinterpret_cast<const UnkMatrix44*>(fn_7100d7c990(self->owner)->field_0x38));
    CVector2D up = {m.m[1], m.m[5]};
    UnkSegment_l49fff4 segment(from, CVector2D{from.x - up.x * 800.0f, from.y - up.y * 800.0f});
    UnkSegmentQuery_l49fff4 query(Position(target));
    UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);
    if (!fn_7100d5c518(tree, 0x80409, reinterpret_cast<const UnkBox*>(&segment), &query, 2))
        return;

    CVector3D floor = query.hit.point;
    UnkZombieAI_l49fff4* ai = static_cast<UnkZombieAI_l49fff4*>(self->owner->field_0x240);
    if (ai->SegmentCount() != 0) {
        CVector2D p;
        fn_710007b430(&p, &floor);
        if (fn_7100118b1c(reinterpret_cast<const UnkUnit108List_A1_b73*>(&ai->path),
                          reinterpret_cast<const UnkUnit108Point_A1_b73*>(&p), 10.0f))
            out->value = 700.0f;
    }
}
