// CWaterTriggerChangeComponent's unit (tools/tu_map.py unit 1579): the update of the component's two
// water parts: each part's linked water pool updates and gives its level, which switches the linked
// object's component on or off, moves the part's polygon's surface and resizes the pool and the part's model.
// Names and layouts are placeholders.
#include <math.h>

#include "base/UnkDestructors.h"
#include "base/UnkRefHandle.h"
#include "base/math/CVector.h"
#include "game/CActorComponent.h"
#include "game/CCharClassSteamJetComponent.h"
#include "game/CGameLink.h"
#include "game/UnkTU108.h"
#include "game/UnkTU127.h"
#include "game/UnkTU437.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector2D;

struct CActor;
struct UnkWaterPool1396;
struct UnkLiquidPoolSet_ovb0075;

void* fn_71008a1ed0(void* target);
f32 fn_7100909ca4(UnkWaterPool1396* self);
f32 fn_7100909e08(UnkWaterPool1396* self);
UnkLiquidPoolSet_ovb0075* fn_71009c7480(void* target);
void fn_7100a48de8(UnkLiquidPoolSet_ovb0075* self, const UnkVec3f* size, const UnkVec3f* center);
void fn_71001435f0(UnkModel* model, const UnkVec3f* scale);

// The water pool a link leads to: it updates through its vf_0x140.
struct UnkWaterPool_la66354 {
    void** vtable;

    void vf_0x140(void* arg) { reinterpret_cast<void (*)(UnkWaterPool_la66354*, void*)>(vtable[0x140 / 8])(this, arg); }
};

struct UnkWaterVertex_la66354 {
    CVector2D pos;
    f32 field_0x8;
};

// A 2D polygon with a lazily updated bounding box (fn_7100119b88 updates it).
struct UnkWaterPolygon_la66354 {
    UnkWaterVertex_la66354* vertices;
    u32 count;
    u8 pad_0xc[0x21];
    bool box_dirty;  // 0x2d
    u8 pad_0x2e[0x2];
    CVector2D box_min;  // 0x30
    CVector2D box_max;  // 0x38

    void UpdateBox() { fn_7100119b88(reinterpret_cast<UnkPolygon2D*>(this)); }
    CVector2D Size() {
        if (box_dirty)
            UpdateBox();
        return CVector2D{box_max.x - box_min.x, box_max.y - box_min.y};
    }
    CVector2D Center() {
        if (box_dirty)
            UpdateBox();
        return CVector2D{(box_min.x + box_max.x) * 0.5f, (box_min.y + box_max.y) * 0.5f};
    }
};

struct UnkWaterModel_la66354 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
};

struct UnkWaterPartComponent_la66354 {
    u8 pad_0x0[0x50];
    UnkWaterModel_la66354* model;  // 0x50
};

struct UnkWaterTriggerChangeComponent_la66354 {
    u8 pad_0x0[0x48];
    UnkWaterPolygon_la66354* polygons[2];          // 0x48
    UnkWaterPartComponent_la66354* components[2];  // 0x58
    u8 pad_0x68[0x40];
    CGameLink<CActor> pools[2];  // 0xa8
    u8 pad_0x128[0x48];
    f32 levels[2];      // 0x170
    f32 field_0x178[2];
    f32 surfaces[2];    // 0x180
    f32 depths[2];      // 0x188
    u32 surface_vertices[2][2];  // 0x190
};
static_assert(__builtin_offsetof(UnkWaterTriggerChangeComponent_la66354, levels) == 0x170, "layout");

// Switches the pool's activatable component and the part's own component on or off.
static inline void Enable(UnkWaterTriggerChangeComponent_la66354* self, UnkStruct_710024fa0c* activatable, u32 part,
                          bool on) {
    fn_710024fa0c(activatable, on);
    if (self->components[part])
        fn_710024fa0c(reinterpret_cast<UnkStruct_710024fa0c*>(self->components[part]), on);
}

void fn_7100a669ac(UnkWaterTriggerChangeComponent_la66354* self);

static inline CActor* Target(CGameLink<CActor>& link) {
    if (link.state == 1) {
        {
            base::UnkRefPtrMember handle = fn_7100353be8(&link.callbacks);
            fn_710031d208(&link.handle, reinterpret_cast<base::UnkRefHandle*>(&handle));
        }
        link.state = 2;
    }
    return link.Target();
}

void fn_7100a66354(UnkWaterTriggerChangeComponent_la66354* self, void* arg) {
    fn_7100a669ac(self);
    // Part 0.
    if (Target(self->pools[0])) {
        if (UnkWaterPool_la66354* pool =
                static_cast<UnkWaterPool_la66354*>(fn_71008a1ed0(self->pools[0].Target()))) {
            pool->vf_0x140(arg);
            self->levels[0] = fn_7100909ca4(reinterpret_cast<UnkWaterPool1396*>(pool));
            self->field_0x178[0] = fn_7100909e08(reinterpret_cast<UnkWaterPool1396*>(pool));
            f32 level = self->levels[0];
            UnkStruct_710024fa0c* activatable =
                reinterpret_cast<UnkStruct_710024fa0c*>(fn_7100899610(Target(self->pools[0])));
            // Off while the pool is empty.
            if (level < 1.1920929e-07f)
                Enable(self, activatable, 0, false);
            else
                Enable(self, activatable, 0, true);
        }

        f32 surface = self->surfaces[0] - self->depths[0] * fabsf(1.0f - self->levels[0]);
        self->polygons[0]->vertices[self->surface_vertices[0][1]].pos.y = surface;
        self->polygons[0]->vertices[self->surface_vertices[0][0]].pos.y = surface;
        self->polygons[0]->UpdateBox();

        {
            UnkLiquidPoolSet_ovb0075* set = fn_71009c7480(Target(self->pools[0]));
            CVector2D size2 = self->polygons[0]->Size();
            UnkVec3f size(size2.x, size2.y, 0.0f);
            CVector2D center2 = self->polygons[0]->Center();
            UnkVec3f center(center2.x, center2.y, 0.0f);
            fn_7100a48de8(set, &size, &center);
        }

        if (self->components[0]) {
            UnkVec3f scale = self->components[0]->model->xform.GetScale();
            scale.y = self->levels[0];
            fn_71001435f0(reinterpret_cast<UnkModel*>(self->components[0]->model), &scale);
            fn_71001436a0(reinterpret_cast<UnkModel*>(self->components[0]->model), scale.y > 0.001f);
        }
    }
    // Part 1.
    if (Target(self->pools[1])) {
        if (UnkWaterPool_la66354* pool =
                static_cast<UnkWaterPool_la66354*>(fn_71008a1ed0(self->pools[1].Target()))) {
            pool->vf_0x140(arg);
            self->levels[1] = fn_7100909ca4(reinterpret_cast<UnkWaterPool1396*>(pool));
            self->field_0x178[1] = fn_7100909e08(reinterpret_cast<UnkWaterPool1396*>(pool));
            f32 level = self->levels[1];
            UnkStruct_710024fa0c* activatable =
                reinterpret_cast<UnkStruct_710024fa0c*>(fn_7100899610(Target(self->pools[1])));
            // Off while the pool is empty.
            if (level < 1.1920929e-07f)
                Enable(self, activatable, 1, false);
            else
                Enable(self, activatable, 1, true);
        }

        f32 surface = self->surfaces[1] - self->depths[1] * fabsf(1.0f - self->levels[1]);
        self->polygons[1]->vertices[self->surface_vertices[1][1]].pos.y = surface;
        self->polygons[1]->vertices[self->surface_vertices[1][0]].pos.y = surface;
        self->polygons[1]->UpdateBox();

        {
            UnkLiquidPoolSet_ovb0075* set = fn_71009c7480(Target(self->pools[1]));
            CVector2D size2 = self->polygons[1]->Size();
            UnkVec3f size(size2.x, size2.y, 0.0f);
            CVector2D center2 = self->polygons[1]->Center();
            UnkVec3f center(center2.x, center2.y, 0.0f);
            fn_7100a48de8(set, &size, &center);
        }

        if (self->components[1]) {
            UnkVec3f scale = self->components[1]->model->xform.GetScale();
            scale.y = self->levels[1];
            fn_71001435f0(reinterpret_cast<UnkModel*>(self->components[1]->model), &scale);
            fn_71001436a0(reinterpret_cast<UnkModel*>(self->components[1]->model), scale.y > 0.001f);
        }
    }
}
