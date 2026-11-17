// CObsydomithonAIComponent unit (tools/tu_map.py, the name from reflection). fn_710062ece4: casts the two
// diagonals of the oriented box of the AI's collider through the collision tree, both ways, and adds the point
// and normal of each hit to the list (up to eight); true when either cast hits. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/spatial/COBox2D.h"
#include "game/CAbilityShinespark.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU437.h"
#include "game/UnkXformNode.h"
#include "game/logic/collision/CCollider.h"
#include "types.h"

#include <new>

using base::math::CVector2D;

// The box copied member by member (as UnkPlacement2D in src/game/UnkCollisionContacts.cpp).
struct UnkOBoxPlacement : base::spatial::COBox2D {
    UnkOBoxPlacement(const UnkOBoxPlacement& o)
        : COBox2D{{o.center.x, o.center.y}, {o.axis_x.x, o.axis_x.y}, {o.axis_y.x, o.axis_y.y},
                  {o.size.x, o.size.y}, o.offset, o.angle} {}
};

struct UnkOBoxShape2D {
    u8 pad_0x0[0xa8];
    UnkOBoxPlacement placement;  // 0xa8
};

UnkOBoxPlacement fn_7100d685c0(const UnkOBoxShape2D* self, const UnkXform* xf);

// A collision shape: its kind (0x40 is the oriented 2D box).
struct UnkObsyShape_l62ece4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 GetKind();  // 0x18
};

struct UnkObsyCollider_l62ece4 {
    u8 pad_0x0[0x6a];
    bool enabled;  // 0x6a
    u8 pad_0x6b[0x158 - 0x6b];
    UnkObsyShape_l62ece4* shape;  // 0x158
};

struct UnkObsyConfig_l62ece4 {
    u8 pad_0x0[0x3c8];
    u32 mask;  // 0x3c8
};

struct UnkObsyOwner_l62ece4 {
    u8 pad_0x0[0x218];
    UnkObsyConfig_l62ece4* config;  // 0x218
};

struct UnkObsydomithonAI_l62ece4 {
    u8 pad_0x0[0x30];
    UnkObsyOwner_l62ece4* owner;  // 0x30
    u8 pad_0x38[0x147a8 - 0x38];
    UnkObsyCollider_l62ece4* collider;  // 0x147a8
};

struct UnkObsyVec3_l62ece4 {
    f32 x, y, z;
};

// A point and normal of a hit.
struct UnkObsyContact_l62ece4 {
    UnkObsyVec3_l62ece4 point;
    UnkObsyVec3_l62ece4 normal;
};

struct UnkObsyContacts_l62ece4 {
    u32 count;
    UnkObsyContact_l62ece4 items[8];
};

#pragma pack(push, 4)
// A collision hit (0x48 bytes): two counted references, an id, the point, a vector, the normal and flags.
struct UnkObsyHit_l62ece4 {
    base::UnkRefPtrMember object;
    base::UnkRefPtrMember field_0x8;
    s32 id;                         // 0x10
    UnkObsyVec3_l62ece4 point;      // 0x14
    UnkObsyVec3_l62ece4 field_0x20;  // 0x20
    UnkObsyVec3_l62ece4 normal;     // 0x2c
    u32 field_0x38[4];

    UnkObsyHit_l62ece4(const UnkObsyHit_l62ece4& other) { fn_7100331c7c(this, &other); }
};

// Up to 64 hits, kept from +4.
struct UnkObsyHits_l62ece4 {
    u32 count;
    u8 storage[64 * sizeof(UnkObsyHit_l62ece4)];  // the hits, built in place

    UnkObsyHit_l62ece4* items() { return reinterpret_cast<UnkObsyHit_l62ece4*>(storage); }
    const UnkObsyHit_l62ece4* items() const { return reinterpret_cast<const UnkObsyHit_l62ece4*>(storage); }

    UnkObsyHits_l62ece4() : count(0) {}
    UnkObsyHits_l62ece4(const UnkObsyHits_l62ece4& other) {
        for (u64 i = 0; i < other.count; ++i)
            new (&items()[i]) UnkObsyHit_l62ece4(other.items()[i]);
        count = other.count;
    }
    ~UnkObsyHits_l62ece4() {
        for (u64 i = 0; i < count; ++i)
            items()[i].~UnkObsyHit_l62ece4();
    }
    const UnkObsyHit_l62ece4* begin() const { return &items()[0]; }
    const UnkObsyHit_l62ece4* end() const { return &items()[count]; }
};
#pragma pack(pop)
static_assert(sizeof(UnkObsyHit_l62ece4) == 0x48, "UnkObsyHit_l62ece4 layout");

// The collector fn_7100d5c518 fills.
struct UnkObsyHitCollector_l62ece4 {
    void** vtable;
    UnkObsyHits_l62ece4 hits;

    UnkObsyHitCollector_l62ece4() : vtable(g_unk_7101a23c58 + 2) {}
    ~UnkObsyHitCollector_l62ece4() { vtable = g_unk_7101a23c58 + 2; }
};

// The segment fn_7100d5c518 casts: its two ends and the vector between them.
struct UnkObsySegment_l62ece4 {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkObsySegment_l62ece4(const CVector2D& s, const CVector2D& e) {
        start.x = s.x;
        start.y = s.y;
        end.x = e.x;
        end.y = e.y;
        delta.x = e.x - s.x;
        delta.y = e.y - s.y;
    }
};

static inline bool Cast(UnkTree* tree, u32 mask, const UnkObsySegment_l62ece4& segment,
                        UnkObsyHitCollector_l62ece4* collector) {
    return fn_7100d5c518(tree, mask, reinterpret_cast<const UnkBox*>(&segment), collector, 2);
}

// Adds the point and normal of each hit to the list while it has room.
static inline void AddContacts(const UnkObsyHits_l62ece4& hits, UnkObsyContacts_l62ece4* out) {
    for (const UnkObsyHit_l62ece4& hit : hits) {
        if (out->count < 8) {
            UnkObsyContact_l62ece4& contact = out->items[out->count++];
            contact.point = hit.point;
            contact.normal = hit.normal;
        }
    }
}

// The shape when it is an oriented 2D box.
static inline UnkObsyShape_l62ece4* AsOBox(UnkObsyShape_l62ece4* shape) {
    return shape && shape->GetKind() == 0x40 ? shape : nullptr;
}

bool fn_710062ece4(UnkObsydomithonAI_l62ece4* self, UnkObsyContacts_l62ece4* out) {
    UnkObsyCollider_l62ece4* collider = self->collider;
    UnkObsyShape_l62ece4* shape = collider ? AsOBox(collider->shape) : nullptr;
    UnkGameManagerSub_0x608* sub = g_unk_7101d47310->field_0x608;
    if (!sub)
        return false;
    UnkTree* tree = shape ? static_cast<UnkTree*>(sub->field_0x820) : nullptr;
    if (!tree)
        return false;
    if (!self->owner->config || !self->collider->enabled)
        return false;

    UnkMatrix44 transform = fn_7100d36a2c(reinterpret_cast<UnkShape*>(self->collider));
    UnkOBoxPlacement box = fn_7100d685c0(reinterpret_cast<UnkOBoxShape2D*>(shape),
                                         reinterpret_cast<const UnkXform*>(&transform));
    f32 half = box.size.x * 0.5f;
    CVector2D extent = {box.axis_x.x * half, box.axis_x.y * half};
    CVector2D min = {box.center.x - extent.x, box.center.y - extent.y};
    CVector2D max = {box.center.x + extent.x, box.center.y + extent.y};
    u32 mask = self->owner->config->mask;

    UnkObsyHitCollector_l62ece4 forward;
    UnkObsyHitCollector_l62ece4 backward;
    bool found = false;
    if (Cast(tree, mask, UnkObsySegment_l62ece4(min, max), &forward)) {
        UnkObsyHits_l62ece4 hits = forward.hits;
        AddContacts(hits, out);
        found = true;
    }
    if (Cast(tree, mask, UnkObsySegment_l62ece4(max, min), &backward)) {
        UnkObsyHits_l62ece4 hits = backward.hits;
        AddContacts(hits, out);
        found = true;
    }
    return found;
}
