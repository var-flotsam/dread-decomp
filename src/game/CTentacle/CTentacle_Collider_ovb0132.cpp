// CTentacle's unit (tools/tu_map.py unit 1133): a segment's corners (fn_710070a5f8) and the tentacle's
// collider, recreated and enabled (fn_7100709c8c). Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/CCollisionComponent.h"
#include "game/UnkTU108.h"
#include "types.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkVec3;

struct UnkTentacleRange_ovb0132 {
    f32 min;
    f32 max;
};

static inline void AddPoint(void* sink, CVector3D& point, f32 x, f32 y) {
    point.x = x;
    point.y = y;
    point.z = 0.0f;
    fn_7100118b94(sink, reinterpret_cast<UnkVec3*>(&point));
}

// When the segment from `a` to `b` overlaps `range` in z, adds to `sink` the four corners of the
// rectangle of the given width around the segment's projection.
bool fn_710070a5f8(void* self, f32 width, const CVector3D* a, const CVector3D* b, const UnkTentacleRange_ovb0132* range,
                   void* sink) {
    f32 za = a->z;
    f32 zb = b->z;
    if (!((za < zb ? za : zb) <= range->max))
        return false;
    if (!((za > zb ? za : zb) >= range->min))
        return false;
    CVector2D pa;
    CVector2D pb;
    CVector3D point;
    fn_710007b430(&pa, a);
    fn_710007b430(&pb, b);
    f32 dx = pb.x - pa.x;
    f32 dy = pb.y - pa.y;
    f32 len = __builtin_sqrtf(dx * dx + dy * dy);
    if (len != 0.0f) {
        dx /= len;
        dy /= len;
    }
    f32 wy = dy * width;
    f32 wx = dx * width;
    f32 nx = wy * 0.5f;
    f32 ny = wx * -0.5f;
    f32 x0 = pa.x + nx, y0 = ny + pa.y;
    f32 x1 = pa.x - nx, y1 = pa.y - ny;
    f32 x2 = pb.x - nx, y2 = pb.y - ny;
    f32 x3 = nx + pb.x, y3 = ny + pb.y;
    AddPoint(sink, point, x0, y0);
    AddPoint(sink, point, x1, y1);
    AddPoint(sink, point, x2, y2);
    AddPoint(sink, point, x3, y3);
    return true;
}

using base::global::CStrId;
using base::global::UnkStrIdInline;

using UnkTentacleCreateCollider_ovb0132 = UnkCollisionObject* (*)(UnkCollisionContainer*, const CStrId*,
                                                                  const CStrId*, const CStrId*, const CStrId*, u32,
                                                                  u32, u32, void*, s32, void*, s32, void*, s32,
                                                                  bool, bool, bool);

// An empty string id: the pool's empty string, the pool created on first use.
struct UnkTentacleEmptyId_ovb0132 {
    base::global::UnkStringData* data;

    UnkTentacleEmptyId_ovb0132() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
    ~UnkTentacleEmptyId_ovb0132() { reinterpret_cast<CStrId*>(this)->~CStrId(); }
};

struct UnkTentacleOwner_ovb0132 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* collision;  // +0x2a0
};

struct UnkTentacleCollider_ovb0132 {
    void** vtable;
    CStrId name;  // +0x8
};

// (Re)creates the collider named at +0x8 in the owner's collision container (the old one is removed
// first) and enables it.
UnkCollisionObject* fn_7100709c8c(UnkTentacleCollider_ovb0132* self, UnkTentacleOwner_ovb0132* owner,
                                  const CStrId* group, u32 flags) {
    if (!owner)
        return nullptr;
    UnkCollisionContainer* collision = owner->collision;
    if (!collision)
        return nullptr;
    if (UnkCollisionObject* old = fn_71008640a4(collision, &self->name))
        fn_7100863e04(collision, old);
    UnkCollisionObject* collider;
    {
        UnkTentacleEmptyId_ovb0132 none;
        const CStrId* type = reinterpret_cast<const CStrId*>(
            reinterpret_cast<char*>(&base::reflection::refl_GetSingleton()) + 0x1f98);
        UnkTentacleCreateCollider_ovb0132 create =
            (*reinterpret_cast<UnkTentacleCreateCollider_ovb0132**>(collision))[0x168 / 8];
        collider = create(collision, &self->name, group, reinterpret_cast<const CStrId*>(&none), type, flags, 0, 1, nullptr, -1, nullptr, -1,
                          nullptr, -1, false, false, true);
    }
    if (collider)
        fn_7100d35a98(reinterpret_cast<UnkEntry*>(collider), false);
    return collider;
}
