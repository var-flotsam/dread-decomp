// UnkTU365 (tools/tu_map.py unit 365, the nav mesh generator): creates the named collider for a region in the
// collider manager, gives it a polygon collection shape holding every outline of the region larger than
// `min_size` both ways with at least three edges, then enables it. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkTU108.h"
#include "game/UnkTU365.h"
#include "game/logic/collision/CPolygonCollectionShape.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector2D;

// An outline: its points (fn_7100118b94 appends one), a z, flags, its bounds and a word; the
// outline set copies it (0x48 bytes).
struct UnkTilePoint {
    f32 x, y, z;
};
struct UnkTileOutline {
    base::global::CRntVector<UnkTilePoint> points;
    f32 field_0x28;
    bool field_0x2c;
    bool field_0x2d;
    base::math::CVector2D min;
    base::math::CVector2D max;
    u32 field_0x40;

    UnkTileOutline()
        : points(7), field_0x28(0.0f), field_0x2c(false), field_0x2d(true), min{3.4028235e38f, 3.4028235e38f},
          max{-3.4028235e38f, -3.4028235e38f}, field_0x40(0) {}
    ~UnkTileOutline() { points.Free(); }
};
static_assert(sizeof(UnkTileOutline) == 0x48, "UnkTileOutline placeholder layout");

struct UnkNavGenerator_ovb0229;
struct UnkTileOutlines;
struct UnkObj_l289ec0;
struct UnkList_l289ec0;
struct UnkNameRef_l289ec0;
UnkObj_l289ec0* fn_7100289ec0(UnkList_l289ec0* self, void* owner, const UnkNameRef_l289ec0* ref);
void fn_7100d46804(UnkManager_7100863a8c* self);
void fn_7100d62e04(UnkTileOutlines* self, UnkTileOutline* outline);

// The collider made for the region.
struct UnkNavCollider_2916e8 {
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
    virtual void vf_0x60(bool flag);
};
void fn_7100d36da4(UnkNavCollider_2916e8* collider, UnkTileOutlines* shape, bool flag);

// The region's outlines.
struct UnkNavRegion_2916e8 {
    UnkTileOutline* outlines;
    u32 count;
};

static inline UnkManager_7100863a8c* ColliderManager() {
    UnkManager_7100863a8c* instance = g_unk_71726a4788;
    if (!instance) {
        instance = static_cast<UnkManager_7100863a8c*>(fn_71000839a0(0x6b0, 0x3f));
        fn_7100d46804(instance);
        g_unk_71726a4788 = instance;
    }
    return instance;
}

// The manager's new collider for `owner`, named `id`.
static inline UnkNavCollider_2916e8* CreateCollider(UnkManager_7100863a8c* manager, void* owner, CStrId id) {
    return reinterpret_cast<UnkNavCollider_2916e8*>(fn_7100289ec0(
        reinterpret_cast<UnkList_l289ec0*>(manager), owner, reinterpret_cast<const UnkNameRef_l289ec0*>(&id)));
}

// A new, empty polygon collection shape.
static inline UnkTileOutlines* NewShape() {
    UnkTileOutlines* shape = static_cast<UnkTileOutlines*>(fn_71000839a0(0x120, 7));
    fn_7100d62b04(shape);
    return shape;
}

static inline u32 EdgeCount(const UnkTileOutline* outline) {
    return outline->points.count - (!outline->field_0x2c & (outline->points.count != 0));
}

static inline const CVector2D& Pos(const UnkTilePoint& point) {
    return reinterpret_cast<const CVector2D&>(point);
}

void fn_71002916e8(UnkNavGenerator_ovb0229* self, u8* region_, u8* part, const CStrId& name, f32 min_size) {
    UnkNavRegion_2916e8* region = reinterpret_cast<UnkNavRegion_2916e8*>(region_);
    UnkNavCollider_2916e8** collider = reinterpret_cast<UnkNavCollider_2916e8**>(part);
    UnkManager_7100863a8c* manager = ColliderManager();
    *collider = CreateCollider(manager, self, name);

    UnkTileOutlines* shape;
    fn_7100d36da4(*collider, shape = NewShape(), true);

    UnkTileOutline* end = region->outlines + region->count;
    for (UnkTileOutline* source = region->outlines; source != end; ++source) {
        if (source->field_0x2d)
            fn_7100119b88(reinterpret_cast<UnkPolygon2D*>(source));
        if (!(source->max.x - source->min.x > min_size && source->max.y - source->min.y > min_size))
            continue;
        if (EdgeCount(source) < 3)
            continue;

        UnkTileOutline outline;
        for (u64 i = 0; i < EdgeCount(source); ++i) {
            const UnkTilePoint* points = source->points.data;
            UnkSegment2D edge(Pos(points[i]), Pos(i == source->points.count - 1 ? points[0] : points[i + 1]));
            UnkTilePoint point = {edge.end.x, edge.end.y, 0.0f};
            fn_7100118b94(&outline, reinterpret_cast<UnkVec3*>(&point));
        }
        fn_7100119f98(&outline, true);
        fn_7100d62e04(shape, &outline);
    }

    (*collider)->vf_0x60(true);
    fn_7100d35a98(reinterpret_cast<UnkEntry*>(*collider), true);
}
