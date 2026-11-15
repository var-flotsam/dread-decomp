// CAbilityGhostAura unit: whether a short downward segment from the given point, as long as the ability's
// reach (+0x488) times 1.1, hits something in the collision tree (query mask 0x200). Names and layouts are
// placeholders.
#include "base/UnkDestructors.h"
#include "base/math/CVector.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkHitRef_l30dda8 {
    base::UnkRefCounted* ptr;

    UnkHitRef_l30dda8() : ptr(nullptr) {}
    ~UnkHitRef_l30dda8() {
        if (base::UnkRefCounted* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
};

struct UnkHitVec_l30dda8 {
    f32 x, y, z;
    UnkHitVec_l30dda8(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

struct UnkCollisionHit_l30dda8 {
    UnkHitRef_l30dda8 object;
    UnkHitRef_l30dda8 field_0x8;
    s32 id;
    UnkHitVec_l30dda8 point;
    UnkHitVec_l30dda8 field_0x20;
    UnkHitVec_l30dda8 normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkCollisionHit_l30dda8()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
};

struct UnkSegmentHitQuery_l30dda8 {
    void** vtable;
    UnkCollisionHit_l30dda8 hit;
    bool field_0x50;
    u32 field_0x54;
    CVector3D origin;
    u64 field_0x68;

    explicit UnkSegmentHitQuery_l30dda8(const CVector3D& o) {
        vtable = g_unk_7101a20fa8 + 2;
        field_0x50 = false;
        origin.x = o.x;
        origin.y = o.y;
        origin.z = o.z;
        field_0x68 = 0;
    }
    ~UnkSegmentHitQuery_l30dda8() { vtable = g_unk_7101a20ed8 + 2; }
};

struct UnkHitSegment_l30dda8 {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkHitSegment_l30dda8(f32 start_x, f32 start_y, f32 end_x, f32 end_y) {
        end.x = end_x;
        end.y = end_y;
        start.x = start_x;
        start.y = start_y;
        delta.x = end_x - start_x;
        delta.y = end_y - start_y;
    }
};

struct UnkSelf {
    u8 pad_0x0[0x488];
    f32 reach;
};

bool fn_710030dda8(UnkSelf* self, void* a2) {
    UnkGameManagerSub_0x608* sub = g_unk_7101d47310->field_0x608;
    if (sub == nullptr || sub->field_0x820 == nullptr)
        return false;
    UnkTree* tree = static_cast<UnkTree*>(sub->field_0x820);
    const CVector3D* pos = static_cast<const CVector3D*>(a2);
    f32 reach = self->reach;
    f32 x = pos->x;
    f32 y = pos->y;
    UnkHitSegment_l30dda8 segment(x, y, x + reach * -0.0f * 1.1f, y - reach * 1.1f);
    UnkSegmentHitQuery_l30dda8 query(CVector3D{x, y, pos->z});
    return fn_7100d5c518(tree, 0x200, reinterpret_cast<const UnkBox*>(&segment), &query, 2);
}
