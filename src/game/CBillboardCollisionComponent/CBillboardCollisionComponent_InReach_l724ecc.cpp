// CBillboardCollisionComponent's unit (tools/tu_map.py): whether any key of the billboard the query
// names lies within reach (the shape's size plus the billboard's own margin) of the transform's position.
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkPoint2D_d52bd0.h"
#include "game/UnkTU1159.h"
#include "game/UnkTU357.h"
#include "game/UnkVec3Result.h"
#include "game/UnkXformNode.h"
#include "game/logic/collision/CAABoxShape2D.h"
#include "types.h"

struct UnkStruct_71007455dc;

// The query's outcome: 1 found, 2 not.
struct UnkQueryResult_l724ecc {
    s32 value;
    UnkQueryResult_l724ecc(s32 v) : value(v) {}
    UnkQueryResult_l724ecc(const UnkQueryResult_l724ecc& o) : value(o.value) {}
};

struct UnkBox_l724ecc {
    UnkVec3f min;
    UnkVec3f max;
};

// A collision shape: its kind (vf_0x18), its bounds (vf_0x28), and for kind 2 a radius at +0xa8.
struct UnkShape_l724ecc {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 GetKind();
    virtual void vf_0x20();
    virtual UnkBox_l724ecc GetBounds(const void* transform);
    u8 field_0x8[0xa0];
    f32 radius;  // 0xa8
};

// The extents of a kind-4 shape (a box): its upper and lower corners.
struct UnkBoxShape_d52bd0;
UnkPoint2D_d52bd0 fn_7100d5e52c(const UnkBoxShape_d52bd0* box);

struct UnkBillboardDef_l724ecc {
    virtual const base::reflection::CType* GetClass();
    u8 pad_0x8[0x58];
    f32 margin;  // 0x60
};

struct UnkBillboardItem_l724ecc {
    u8 pad_0x0[0xb8];
    UnkSwarmMember_ovb6** keys;  // 0xb8
    u32 keyCount;                // 0xc0
    u8 pad_0xc4[0x84];
};
static_assert(sizeof(UnkBillboardItem_l724ecc) == 0x148, "UnkBillboardItem_l724ecc layout");

struct UnkBillboardSetData_l724ecc {
    u8 pad_0x0[0x10];
    UnkBillboardDef_l724ecc* def;  // 0x10
    u8 pad_0x18[0x38];
    UnkBillboardItem_l724ecc* items;  // 0x50
    u8 pad_0x58[0xb8];
    s32 count;  // 0x110
};

struct UnkBillboardSet_l724ecc {
    u8 pad_0x0[0x10];
    UnkBillboardSetData_l724ecc* data;  // 0x10
};

struct UnkBillboardCollision_l724ecc {
    u8 pad_0x0[0x30];
    void* owner;  // 0x30
    u8 pad_0x38[0xf8];
    UnkBillboardSet_l724ecc* billboards;  // 0x130
};

struct UnkQuery_l724ecc {
    u8 pad_0x0[0x88];
    s32 billboard;  // 0x88
};

// A 3x4 transform; the position is its last column.
struct UnkTransform_l724ecc {
    f32 m[3][4];
};

struct UnkTarget_l724ecc {
    void** vtable;
    bool Accepts(UnkSwarmMember_ovb6* key) {
        return reinterpret_cast<bool (*)(UnkTarget_l724ecc*, UnkSwarmMember_ovb6*)>(vtable[0x1c8 / 8])(this, key);
    }
};

static inline f32 DistanceSq2D(const base::math::CVector2D& a, const base::math::CVector2D& b) {
    f32 dx = a.x - b.x;
    f32 dy = a.y - b.y;
    return dx * dx + dy * dy;
}

UnkQueryResult_l724ecc fn_7100724ecc(UnkBillboardCollision_l724ecc* self, UnkQuery_l724ecc* query, const void*,
                                     UnkShape_l724ecc* shape, UnkTransform_l724ecc* transform) {
    s32 index = query->billboard;
    if (index == -1)
        return 2;

    // The shape's size: a sphere's radius, else the larger side of a box.
    s32 kind = shape->GetKind();
    f32 size;
    if (!shape || kind != 2 || (size = shape->radius) < 0.0f) {
        auto* box = reinterpret_cast<const UnkBoxShape_d52bd0*>(shape->GetKind() == 4 ? shape : nullptr);
        UnkPoint2D_d52bd0 hi = fn_7100d5e52c(box);
        UnkPoint2D_d52bd0 lo = fn_7100d5e540(box);
        f32 dx = hi.x - lo.x;
        f32 dy = hi.y - lo.y;
        size = dx < dy ? dy : dx;
    }
    if (size < 0.0f) {
        UnkBox_l724ecc bounds = shape->GetBounds(shape->field_0x8);
        f32 hy = (bounds.max.y - bounds.min.y) * 0.5f;
        f32 hx = (bounds.max.x - bounds.min.x) * 0.5f;
        size = hx > hy ? hx : hy;
    }

    UnkBillboardSetData_l724ecc* data = self->billboards->data;
    UnkBillboardItem_l724ecc* item;
    if (data->count > index)
        item = data->items ? &data->items[index] : nullptr;
    else
        item = nullptr;
    UnkBillboardDef_l724ecc* def = data->def;
    UnkBillboardDef_l724ecc* billboard = fn_710009d50c(def->GetClass(), fn_710072be34()) ? def : nullptr;
    f32 margin = billboard->margin;
    UnkTarget_l724ecc* target = reinterpret_cast<UnkTarget_l724ecc*>(fn_7100286ad4(self->owner));

    if (item->keyCount == 0)
        return 2;
    f32 reach = size + margin;
    f32 reachSq = reach * reach;
    for (u64 i = 0; i < item->keyCount; ++i) {
        UnkSwarmMember_ovb6* key = item->keys[i];
        if (key && !fn_7100744e1c(key))
            continue;
        UnkVec3Result at(transform->m[0][3], transform->m[1][3], transform->m[2][3]);
        base::math::CVector2D at2;
        base::math::CVector2D pos2;
        fn_710007b430(&at2, reinterpret_cast<const base::math::CVector3D*>(&at));
        UnkVec3Result pos = GetPositionFromParentAndOffset(reinterpret_cast<UnkStruct_71007455dc*>(key));
        fn_710007b430(&pos2, reinterpret_cast<const base::math::CVector3D*>(&pos));
        if (DistanceSq2D(at2, pos2) <= reachSq && (!target || target->Accepts(key)))
            return 1;
    }
    return 2;
}
