// CYamplotXAIComponent's unit: whether the owner's collision object is clear of the world: builds a 2D box
// shape from the object's bounds, placed at its position, and asks the collision tree whether anything
// overlaps it. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game/logic/collision/CAABoxShape2D.h"
#include "game/logic/collision/CCollider.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

__attribute__((visibility("hidden"))) extern base::global::CStrId g_unk_717270c498;

struct UnkBox3_l6efe18 {
    CVector3D min;
    CVector3D max;
};

struct UnkBox2_l6efe18 {
    f32 min_x;
    f32 min_y;
    f32 max_x;
    f32 max_y;
};

struct UnkBounds_l6efe18 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual UnkBox3_l6efe18 vf_0x28(const void* arg);
    u64 field_0x8;
};

struct UnkBoundsObject_l6efe18 {
    u8 pad_0x0[0x158];
    UnkBounds_l6efe18* field_0x158;
};

struct UnkCollisionShape {
    u8 data[0xc0];
};
void fn_7100d5e260(UnkCollisionShape* shape, const UnkMatrix44* xf, const UnkBox2_l6efe18* box);

struct UnkOwnerSub_l6efe18 {
    u8 pad_0x0[0x3c8];
    u32 field_0x3c8;
};

struct UnkOwner_l6efe18 {
    u8 pad_0x0[0x218];
    UnkOwnerSub_l6efe18* field_0x218;
    u8 pad_0x220[0x80];
    UnkCollisionContainer* field_0x2a0;
};

struct UnkYamplot_l6efe18 {
    u8 pad_0x0[0x30];
    UnkOwner_l6efe18* owner;
};

static inline void SetIdentity(UnkMatrix44& m) {
    for (f32& v : m.m)
        v = 0.0f;
    m.m[15] = 1.0f;
    m.m[10] = 1.0f;
    m.m[5] = 1.0f;
    m.m[0] = 1.0f;
}

bool fn_71006efe18(UnkYamplot_l6efe18* self) {
    UnkCollisionObject* object = fn_71008640a4(self->owner->field_0x2a0, &g_unk_717270c498);
    UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);
    UnkMatrix44 placement;
    SetIdentity(placement);
    CVector2D lo, hi;
    {
        UnkMatrix44 at = fn_7100d36a2c(reinterpret_cast<UnkShape*>(object));
        placement.m[3] = at.m[3];
        placement.m[7] = at.m[7];
        placement.m[11] = at.m[11];
    }
    UnkBounds_l6efe18* bounds = reinterpret_cast<UnkBoundsObject_l6efe18*>(object)->field_0x158;
    {
        UnkBox3_l6efe18 first = bounds->vf_0x28(&bounds->field_0x8);
        fn_710007b430(&lo, &first.min);
    }
    {
        UnkBox3_l6efe18 second = bounds->vf_0x28(&bounds->field_0x8);
        fn_710007b430(&hi, &second.max);
    }
    UnkBox2_l6efe18 box = {lo.x, lo.y, hi.x, hi.y};
    UnkCollisionShape shape;
    fn_7100d5e260(&shape, &placement, &box);
    UnkMatrix44 identity;
    SetIdentity(identity);
    bool hit = fn_7100d5b484(tree, self->owner->field_0x218->field_0x3c8, reinterpret_cast<UnkShape*>(&shape),
                             reinterpret_cast<const UnkMtx44*>(&identity), nullptr, 0);
    fn_7100d5e2c8(&shape);
    return !hit;
}
