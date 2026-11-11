// CElectrifyingAreaComponent's unit (tools/tu_map.py unit 1236): the area's shape test. Names and
// layouts are placeholders.
#include "base/UnkVector.h"
#include "base/math/CVector.h"
#include "game/CLifeComponent.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU108.h"
#include "game/UnkTU365.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkPolygon2D;
struct UnkPolyline108_b67a1;
struct UnkPolygon109;
struct UnkHolder;
struct UnkBreakablePart;
struct UnkBreakableTarget;
struct UnkElectrifyingAreaA5_b61;

bool fn_7100118be8(const UnkPolyline108_b67a1* self);

struct UnkAreaVertex_ov110 {
    f32 x, y, z;
};

// A 2D polygon: its vertices, whether it is closed, and a bounding box kept up to date lazily
// (the 0x48-byte layout of src/base/spatial/UnkPolygon2D.cpp).
struct UnkAreaPolygon_ov110 {
    base::UnkVector<UnkAreaVertex_ov110> vertices{7};
    u32 field_0x28 = 0;
    bool closed = false;    // 0x2c
    bool box_dirty = true;  // 0x2d
    f32 box_min_x = 3.4028235e38f;
    f32 box_min_y = 3.4028235e38f;
    f32 box_max_x = -3.4028235e38f;
    f32 box_max_y = -3.4028235e38f;
    u32 field_0x40 = 0;

    ~UnkAreaPolygon_ov110() { vertices.Free(); }
};
static_assert(sizeof(UnkAreaPolygon_ov110) == 0x48, "UnkAreaPolygon_ov110 layout");

// The area's shape: an outer polygon and an inner one.
struct UnkAreaShape_ov110 {
    UnkAreaPolygon_ov110 outer;
    UnkAreaPolygon_ov110 inner;
    u32 field_0x90 = 0;
};

struct UnkAreaConfig_ov110 {
    u8 pad_0x0[0x59];
    bool use_fallback;  // 0x59
    u8 pad_0x5a[0x2];
    f32 size_x;  // 0x5c
    f32 size_y;  // 0x60
    f32 size_z;  // 0x64
    u8 pad_0x68[0xc];
    f32 distance;  // 0x74
};

struct UnkAreaObject_ov110 {
    u8 pad_0x0[0x30];
    u64 key;  // 0x30
};

struct UnkAreaTarget_ov110 {
    u8 pad_0x0[0x10];
    UnkAreaObject_ov110* object;  // 0x10
};

struct UnkAreaPart_ov110 {
    u8 pad_0x0[0x10];
    u8* part;  // 0x10, the breakable part at its +0x1b8
};

struct UnkAreaHolder_ov110 {
    u8 pad_0x0[0x48];
    UnkAreaPart_ov110* child;  // 0x48
};

struct UnkAreaOwner_ov110 {
    u8 pad_0x0[0x1b8];
    UnkAreaHolder_ov110* holder;  // 0x1b8
};

struct UnkAreaEntry_ov110 {
    u8 pad_0x0[0x28];
};

// The navigation mesh's table of 0x28-byte entries keyed by object id.
struct UnkAreaNavMesh_ov110 {
    u8 pad_0x0[0x2e8];
    UnkAreaEntry_ov110* values;  // 0x2e8
    u64* keys;                   // 0x2f0
    u32 count;                   // 0x2f8
};

struct UnkElectrifyingArea_ov110 {
    u8 pad_0x0[0x30];
    UnkAreaOwner_ov110* owner;  // 0x30
    u8 pad_0x38[0x74];
    bool use_box;  // 0xac
    u8 pad_0xad[0x6b];
    void* field_0x118;
    UnkAreaConfig_ov110* config;  // 0x120
};

bool fn_71007d1b80(UnkElectrifyingArea_ov110* self, UnkAreaShape_ov110* shape, void* arg, void* data, f32 x, f32 y,
                   f32 z);

static inline UnkAreaEntry_ov110* Find_ov110(UnkAreaNavMesh_ov110* t, const u64& key) {
    for (u32 i = 0; i < t->count; ++i)
        if (t->keys[i] == key)
            return &t->values[i];
    return &t->values[t->count];
}

void* fn_71007d191c(UnkElectrifyingAreaA5_b61* area) {
    UnkElectrifyingArea_ov110* self = reinterpret_cast<UnkElectrifyingArea_ov110*>(area);
    UnkAreaHolder_ov110* holder = self->owner->holder;
    if (!holder)
        return nullptr;
    if (!holder->child || !holder->child->part) {
        fn_710115fe20(reinterpret_cast<UnkHolder*>(holder));
        if (!holder->child || !holder->child->part)
            return nullptr;
    }
    UnkAreaTarget_ov110* target = reinterpret_cast<UnkAreaTarget_ov110*>(
        *fn_71002a76a4(reinterpret_cast<UnkBreakablePart*>(holder->child->part + 0x1b8)));
    if (!target)
        return nullptr;
    UnkAreaObject_ov110* object = target->object;
    if (!object)
        return nullptr;
    UnkNavMeshManager* manager = g_unk_71726a4510;
    if (!manager) {
        manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(manager);
        g_unk_71726a4510 = manager;
    }
    UnkAreaNavMesh_ov110* navmesh = reinterpret_cast<UnkAreaNavMesh_ov110*>(manager->navmesh);
    if (!navmesh)
        return nullptr;
    UnkAreaEntry_ov110* entry = Find_ov110(navmesh, object->key);
    return entry == &navmesh->values[navmesh->count] ? nullptr : entry;
}

void fn_71007d1a74(UnkElectrifyingArea_ov110* self, void* arg, bool* inside, UnkAreaShape_ov110* shape) {
    *inside = false;
    UnkAreaConfig_ov110* config = self->config;
    f32 x = config->size_x;
    f32 y = config->size_y;
    f32 z = config->size_z;
    if (config->use_fallback) {
        *inside = fn_71007d1b80(self, shape, arg, self->field_0x118, -1.0f, -1.0f, -1.0f);
        if (shape->outer.box_dirty)
            fn_7100119b88(reinterpret_cast<UnkPolygon2D*>(shape));
        f32 size_x = shape->outer.box_max_x - shape->outer.box_min_x;
        f32 size_y = shape->outer.box_max_y - shape->outer.box_min_y;
        f32 half_x = size_x * 0.5f;
        f32 half_y = size_y * 0.5f;
        // A small enough area that winds the right way counts as inside.
        if (*inside && self->use_box && fn_7100118be8(reinterpret_cast<const UnkPolyline108_b67a1*>(shape)) &&
            half_x < y && half_y < y) {
            *inside = true;
            return;
        }
    }
    *inside = fn_71007d1b80(self, shape, arg, self->field_0x118, x, y, z);
}

bool fn_71007d2378(UnkElectrifyingArea_ov110* self, const CVector3D* position, void* arg) {
    bool inside = false;
    UnkAreaShape_ov110 shape;
    fn_71007d1a74(self, arg, &inside, &shape);
    bool result;
    if (inside) {
        CVector2D point;
        fn_710007b430(&point, position);
        result = fn_7100117aa8(reinterpret_cast<UnkPolygon109*>(&shape), &point, self->config->distance);
    } else {
        result = false;
    }
    return result;
}
