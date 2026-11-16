// Unit 2566 (tools/tu_map.py; hint CMinimapManager::CTunableMinimapManager): rebuilds the minimap's scene
// objects, one per shape of the map data: the old ones come off layer 6, a new one per shape is created
// in the render pool, put on layer 6 and handed the shape's lists. Names and layouts are placeholders.
#include <string.h>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkRenderManager.h"
#include "game/UnkSceneObject.h"
#include "game/UnkTU140.h"
#include "game/UnkTU252.h"
#include "types.h"

using base::global::CRntVector;

// Restarts a scene object with a mode, a count and three lists (not defined by hand).
void fn_7100148128(UnkSceneObject* self, u32 mode, u32 count, const UnkSceneSpan& positions,
                   const UnkSceneSpan& colors, const UnkSceneSpan& indices);

// The render manager's node of a scene object, after the object's vtable.
static inline UnkSceneNode* NodeOf(UnkSceneObject* object) {
    return object ? reinterpret_cast<UnkSceneNode*>(reinterpret_cast<u8*>(object) + 8) : nullptr;
}

// Creates a scene object in the render pool and puts it on `layer`.
static inline UnkSceneObject* CreateOnLayer(u32 layer) {
    UnkRenderManager* render = g_unk_7101d47300;
    UnkSceneObject* created = nullptr;
    fn_7100149930(render->field_0x218, nullptr, &created, &g_unk_7101d470e0);
    fn_7100219ae0(render, NodeOf(created), layer);
    return created;
}

// A colour (red, green, blue, alpha).
struct UnkColor_le90c28 {
    f32 r;
    f32 g;
    f32 b;
    f32 a;

    UnkColor_le90c28(f32 r, f32 g, f32 b, f32 a) : r(r), g(g), b(b), a(a) {}
};

// The start of the scene object's sub-object: a word the rebuild clears.
struct UnkSceneObjectSubHead_le90c28 {
    u8 pad_0x0[0x18];
    u32 field_0x18;
};

// A shape of the map: its points and its indices (two arrays with their counts).
struct UnkMinimapShape_le90c28 {
    CRntVector<u8> points;
    CRntVector<u8> indices;
    u8 pad_0x50[0x10];
};
static_assert(sizeof(UnkMinimapShape_le90c28) == 0x60, "UnkMinimapShape_le90c28 layout");

struct UnkMinimapShapes_le90c28 {
    u8 pad_0x0[0x5fd8];
    UnkMinimapShape_le90c28* shapes;  // 0x5fd8
    u32 shape_count;                  // 0x5fe0
};

struct UnkMinimapScene_le90c28 {
    u8 pad_0x0[0x68];
    UnkMinimapShapes_le90c28* map;  // 0x68
    u8 pad_0x70[0xd63f58 - 0x70];
    CRntVector<UnkSceneObject*> objects;  // 0xd63f58
};

// Appends a null object, growing the array to exactly the new count, and returns its slot.
static inline UnkSceneObject*& AddObject(CRntVector<UnkSceneObject*>& objects) {
    u32 n = objects.count + 1;
    UnkSceneObject** data;
    if (objects.capacity < n) {
        data = static_cast<UnkSceneObject**>(objects.allocator->vf_0x40((u64)n * sizeof(UnkSceneObject*), 8, objects.flags));
        if (objects.data) {
            memcpy(data, objects.data, (u64)objects.count * sizeof(UnkSceneObject*));
            objects.allocator->vf_0x48(objects.data);
        }
        objects.data = data;
        objects.capacity = n;
    } else {
        data = objects.data;
    }
    memset(&data[objects.count], 0, (u64)(n - objects.count) * sizeof(UnkSceneObject*));
    objects.count = n;
    UnkSceneObject** end = &objects.data[n];
    return n ? end[-1] : *end;
}

void fn_7100e90c28(UnkMinimapScene_le90c28* self) {
    CRntVector<UnkSceneObject*>& objects = self->objects;
    for (UnkSceneObject** it = objects.data, **end = it + objects.count; it != end; ++it) {
        UnkItemBase_71011a91c4* item = reinterpret_cast<UnkItemBase_71011a91c4*>(NodeOf(*it));
        fn_71002199d8(g_unk_7101d47300, item, 6);
    }
    objects.count = 0;

    UnkMinimapShapes_le90c28* map = self->map;
    if (!map)
        return;
    UnkColor_le90c28 color(0.0f, 0.0f, 0.0f, 0.5f);
    for (UnkMinimapShape_le90c28 *shape = map->shapes, *end = shape + map->shape_count; shape != end; ++shape) {
        UnkSceneObject*& object = AddObject(objects);
        object = CreateOnLayer(6);
        UnkSceneSpan points = {shape->points.data, shape->points.count};
        UnkSceneSpan colors = {&color, 1};
        UnkSceneSpan indices = {shape->indices.data, shape->indices.count};
        fn_7100148128(object, 6, 1, points, colors, indices);
        fn_7100149490(object, false);
        fn_7100149490(object, true);
        fn_71001beee4(object->field_0x130, false);
        reinterpret_cast<UnkSceneObjectSubHead_le90c28*>(object->field_0x130)->field_0x18 = 0;
    }
}
