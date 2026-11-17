// CMovableGrapplePointComponent's unit (tools/tu_map.py unit 1292): whether the owner can be pulled towards
// `other`: each of the owner's "collision" colliders, shrunk a little, is swept 100 units that way from 5
// units behind the owner; it may move unless something hit is not one of the owner's own colliders.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/CAbilityShinespark.h"
#include "game/UnkActor.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "game/logic/collision/CAABoxShape2D.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector2D;

struct UnkMatrix44_7100078c9c {
    f32 m[16];
};

struct UnkBox2D {
    f32 min_x, min_y, max_x, max_y;

    UnkBox2D(f32 min_x_, f32 min_y_, f32 max_x_, f32 max_y_)
        : min_x(min_x_), min_y(min_y_), max_x(max_x_), max_y(max_y_) {}
};

struct UnkStruct_71008651c8_Entry;
struct UnkStruct_71008651c8;
using UnkOut_71008651c8 = base::global::CRntVector<UnkStruct_71008651c8_Entry*>;
void fn_71008651c8(UnkStruct_71008651c8*, void* const*, UnkOut_71008651c8*);

// A collider's shape: slot 0x18 answers its kind (4 for an axis-aligned box), with its transform at +0x8
// and, for a box, its bounds at +0xa8.
struct UnkShape_l82ac54 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 vf_0x18();
    UnkMatrix44 matrix;  // 0x8
    u8 pad_0x48[0x60];
    f32 min_x, min_y, max_x, max_y;  // 0xa8
};

// The box shape built on the stack (0xc0 bytes): slot 0x80 brings it up to date with its transform.
struct UnkBoxShape_l82ac54 {
    void** vtable;
    UnkMatrix44 matrix;  // 0x8
    u8 pad_0x48[0x78];

    void vf_0x80() { reinterpret_cast<void (*)(UnkBoxShape_l82ac54*)>(vtable[0x80 / 8])(this); }
};
static_assert(sizeof(UnkBoxShape_l82ac54) == 0xc0, "layout");

struct UnkCollider_l82ac54 {
    u8 pad_0x0[0x158];
    UnkShape_l82ac54* shape;  // 0x158
};

struct UnkColliderEntry_l82ac54 {
    u8 pad_0x0[0x28];
    UnkCollider_l82ac54* collider;  // 0x28
};

// A hit: its entity's name (+0x20) is looked up among the owner's colliders.
struct UnkHitEntity_l82ac54 {
    u8 pad_0x0[0x20];
    CStrId name;  // 0x20
};
struct UnkHitHandle_l82ac54 : UnkRefObject {
    UnkHitEntity_l82ac54* entity;  // 0x10
};
struct __attribute__((packed)) UnkHitRecord_l82ac54 {
    UnkHitHandle_l82ac54* handle;
    UnkRefObject* field_0x8;
    u8 pad_0x10[0x38];
};

static inline void ReleaseRef(UnkRefObject* p) {
    if (p && --p->ref_count == 0)
        p->vf_0x10();
}

// The hits of a collision query: up to eight records inline after the count.
struct __attribute__((packed)) UnkHitList_l82ac54 {
    void** vtable;
    u32 count;
    u8 storage[8 * sizeof(UnkHitRecord_l82ac54)];

    UnkHitRecord_l82ac54* items() { return reinterpret_cast<UnkHitRecord_l82ac54*>(storage); }

    UnkHitList_l82ac54() {
        vtable = g_unk_7101a23478 + 2;
        count = 0;
    }
    ~UnkHitList_l82ac54() {
        vtable = g_unk_7101a23478 + 2;
        for (u64 i = 0; i < count; i++) {
            ReleaseRef(items()[i].field_0x8);
            ReleaseRef(items()[i].handle);
        }
        count = 0;
    }
    UnkHitRecord_l82ac54* begin() { return items(); }
    UnkHitRecord_l82ac54* end() {
        return reinterpret_cast<UnkHitRecord_l82ac54*>(storage + count * sizeof(UnkHitRecord_l82ac54));
    }
};

// The collision query: the shape at `matrix`, swept by `sweep`, against the tree.
bool fn_7100d5babc(void* tree, u32 mask, UnkCollisionShape* shape, const UnkMatrix44_7100078c9c* matrix,
                   const CVector2D* sweep, UnkHitList_l82ac54* hits, u32 flags);

struct UnkGrappleOwner_l82ac54 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x238];
    UnkCollisionContainer* collision;  // 0x2a0
};

struct UnkMovableGrapplePoint_l82ac54 {
    u8 pad_0x0[0x30];
    UnkGrappleOwner_l82ac54* owner;  // 0x30
};

inline f32 SignOf(f32 v) {
    return (v > 0.0f) - (v < 0.0f);
}

inline UnkHitEntity_l82ac54* EntityOf(UnkHitHandle_l82ac54* handle) {
    return handle ? handle->entity : nullptr;
}

bool fn_710082ac54(UnkMovableGrapplePoint_l82ac54* self, UnkActor* other) {
    UnkGameManager* gm = g_unk_7101d47310;
    if (!gm->field_0x608)
        return true;
    void* tree = gm->field_0x608->field_0x820;
    if (!tree)
        return true;
    UnkCollisionContainer* container = self->owner->collision;
    if (!container)
        return true;

    UnkOut_71008651c8 colliders(0xf);
    {
        CStrId name("collision", true);
        fn_71008651c8(reinterpret_cast<UnkStruct_71008651c8*>(container), reinterpret_cast<void* const*>(&name),
                      &colliders);
    }

    bool clear = true;
    for (u64 i = 0; i < colliders.count; ++i) {
        UnkCollider_l82ac54* collider = reinterpret_cast<UnkColliderEntry_l82ac54*>(colliders.data[i])->collider;
        if (!collider)
            continue;

        UnkGrappleOwner_l82ac54* owner = self->owner;
        f32 dir = SignOf(other->field_0x44 - owner->x);
        UnkVec3f start(owner->x + dir * -5.0f, owner->y, owner->z);
        CVector2D sweep = {dir * 100.0f, 0.0f};

        UnkShape_l82ac54* source = collider->shape;
        UnkShape_l82ac54* box_shape = source->vf_0x18() == 4 ? source : nullptr;
        f32 center_x = (box_shape->min_x + box_shape->max_x) * 0.5f;
        f32 center_y = (box_shape->min_y + box_shape->max_y) * 0.5f;
        UnkBox2D box(center_x + (box_shape->min_x - center_x) * 0.99f, center_y + (box_shape->min_y - center_y) * 0.99f,
                     center_x + (box_shape->max_x - center_x) * 0.99f, center_y + (box_shape->max_y - center_y) * 0.99f);
        UnkBoxShape_l82ac54 shape;
        fn_7100d5e1f8(reinterpret_cast<UnkCollisionShape*>(&shape), &box);
        UnkCopyMatrix(&shape.matrix, box_shape->matrix);
        shape.vf_0x80();

        bool blocked = false;
        {
            UnkHitList_l82ac54 hits;
            UnkMatrix44_7100078c9c at = fn_7100078cd8(start);
            if (fn_7100d5babc(tree, 0x11, reinterpret_cast<UnkCollisionShape*>(&shape), &at, &sweep, &hits, 0)) {
                for (UnkHitRecord_l82ac54* it = hits.begin(); it != hits.end(); ++it) {
                    if (!fn_71008640a4(container, &EntityOf(it->handle)->name)) {
                        blocked = true;
                        break;
                    }
                }
            }
        }
        fn_7100d5e2c8(reinterpret_cast<UnkCollisionShape*>(&shape));
        if (blocked) {
            clear = false;
            break;
        }
    }
    colliders.Free();
    return clear;
}
