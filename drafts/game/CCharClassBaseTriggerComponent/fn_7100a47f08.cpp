// CCharClassBaseTriggerComponent unit (tools/tu_map.py): collects what touches the trigger. Without the
// world flag only the player is tested; with it, every entity of the scene. An entity counts when one
// of its collision shapes in the trigger's mask overlaps the trigger's collider (box first, then the
// shape tester), and is appended with the contact point. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2509.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU365.h"
#include "game/logic/collision/CCollider.h"
#include "game/logic/collision/UnkColliderResult.h"
#include "types.h"

using base::reflection::CClass;

// As src/game/CCollisionComponent/CCollisionComponent_Collect_l864374.cpp defines them.
struct UnkCollisionShape_l864374 {
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
    virtual u32 vf_0x58();
    u8 unk_0x8[0x62];
    bool enabled;  // 0x6a
};

struct UnkCollisionRecord_l864374 {
    u8 unk_0x0[0x18];
    bool dirty;  // 0x18
    u8 unk_0x19[0xf];
    UnkCollisionShape_l864374* object;  // 0x28
};
static_assert(sizeof(UnkCollisionRecord_l864374) == 0x30, "record size");

struct UnkCollisionContainer_l864374;
void fn_7100864374(UnkCollisionContainer_l864374* self, u32 mask,
                   base::global::CRntVector<UnkCollisionRecord_l864374*>* out, bool all);

// The records fn_7100864374 fills, shared.
extern base::global::CRntVector<UnkCollisionRecord_l864374*> g_unk_717274ac30;
extern void* g_unk_7101a22c60[];

// A box in the plane.
struct UnkBox_la47f08 {
    f32 min_x;
    f32 min_y;
    f32 max_x;
    f32 max_y;
};

// A collision body's box, returned in memory.
struct UnkBodyBox_la47f08 {
    f32 min_x;
    f32 min_y;
    f32 max_x;
    f32 max_y;
    UnkBodyBox_la47f08(const UnkBodyBox_la47f08& o) : min_x(o.min_x), min_y(o.min_y), max_x(o.max_x), max_y(o.max_y) {}
};

// The box of a body's shapes on one layer, and of those in a mask.
UnkBodyBox_la47f08 fn_71008653c0(UnkCollisionContainer_l864374* body, u32 layer, bool all);
UnkBodyBox_la47f08 fn_7100865548(UnkCollisionContainer_l864374* body, u32 mask, bool all);

// A collider as seen here: its mask, its box (brought up to date by vf_0x98) and its shape.
struct UnkTriggerCollider_la47f08 {
    void** vtable;
    u8 pad_0x8[0x38];
    UnkBox_la47f08 box;  // 0x40
    u8 pad_0x50[0x10];
    bool box_dirty;  // 0x60
    u8 pad_0x61[0x158 - 0x61];
    void* shape;  // 0x158

    u32 Mask() { return reinterpret_cast<u32 (*)(UnkTriggerCollider_la47f08*)>(vtable[0x58 / 8])(this); }
    const UnkBox_la47f08& Bounds() {
        if (box_dirty)
            reinterpret_cast<void (*)(UnkTriggerCollider_la47f08*)>(vtable[0x98 / 8])(this);
        return box;
    }
};

struct UnkPoint_la47f08 {
    f32 x;
    f32 y;
    f32 z;
};

// The shape tester's contact sink: it keeps the contact point.
struct UnkTriggerSink_la47f08 {
    void** vtable;
    s32 field_0x8;
    UnkPoint_la47f08 point;  // 0xc
    u8 field_0x18[0x1c];

    UnkTriggerSink_la47f08() : vtable(g_unk_7101a22c60 + 2), field_0x8(-1), point(), field_0x18() {}
};

// One entry of the result: the entity and where it touches.
struct __attribute__((packed, aligned(4))) UnkTriggerHit_la47f08 {
    void* entity;
    UnkPoint_la47f08 point;  // 0x8
    u32 field_0x14;
};

struct UnkTriggerHits_la47f08 {
    u32 count;
    UnkTriggerHit_la47f08 entries[16];  // capacity not known

    void Add(void* entity, const UnkPoint_la47f08& point) {
        UnkTriggerHit_la47f08& hit = entries[count];
        hit.entity = entity;
        hit.point = point;
        ++count;
    }
};

// An entity as seen here.
struct UnkTriggerEntity_la47f08 {
    void** vtable;
    u8 pad_0x8[0x1a0];
    void* field_0x1a8;
    bool field_0x1b0;
    u8 pad_0x1b1[3];
    bool field_0x1b4;
    u8 pad_0x1b5[0x2a0 - 0x1b5];
    UnkCollisionContainer_l864374* body;  // 0x2a0
};

struct UnkTriggerNode_la47f08 {
    UnkTriggerEntity_la47f08* entity;
    UnkTriggerNode_la47f08* next;
};

struct UnkTriggerScene_la47f08 {
    u8 pad_0x0[0x2b0];
    UnkTriggerNode_la47f08* entities;  // 0x2b0
};

struct UnkTriggerWorld_la47f08 {
    u8 pad_0x0[0x608];
    UnkTriggerScene_la47f08* scene;  // 0x608
};

struct UnkTrigger_la47f08 {
    void** vtable;
    u8 pad_0x8[0x80];
    UnkTriggerCollider_la47f08* collider;  // 0x88
    u8 pad_0x90[8];
    u32 layer;  // 0x98
    u8 pad_0x9c[0x9bc - 0x9c];
    bool world;  // 0x9bc

    // The point the trigger reports for an entity it has no shape to test against.
    bool DefaultPoint(UnkPoint_la47f08* point) {
        return reinterpret_cast<bool (*)(UnkTrigger_la47f08*, const base::global::CStrId*, UnkPoint_la47f08*)>(
            vtable[0x188 / 8])(this, &g_unk_7101d470e0, point);
    }
    // Whether the entity may trigger it.
    bool Accepts(UnkTriggerEntity_la47f08* entity) {
        return reinterpret_cast<bool (*)(UnkTrigger_la47f08*, UnkTriggerEntity_la47f08*)>(vtable[0x1d0 / 8])(this,
                                                                                                         entity);
    }
};

inline UnkShapeTester_A5_b68* Tester() {
    UnkShapeTester_A5_b68* tester = g_unk_71726a47a0;
    if (!tester) {
        tester = static_cast<UnkShapeTester_A5_b68*>(fn_71000839a0(8, 0x3f));
        fn_7100d5134c(tester);
        g_unk_71726a47a0 = tester;
    }
    return tester;
}

inline UnkColliderResult Test(UnkShapeTester_A5_b68* tester, void* a, const UnkMatrix44& ta, void* b,
                              const UnkMatrix44& tb, UnkTriggerSink_la47f08* sink) {
    return fn_7100d51368(tester, a, const_cast<UnkMatrix44*>(&ta), b, tb,
                         reinterpret_cast<UnkShapeFilter_ovb0107*>(sink), 0);
}

inline const UnkMatrix44 Transform(UnkTriggerCollider_la47f08* collider) {
    return fn_7100d36a2c(reinterpret_cast<UnkShape*>(collider));
}

// Whether `b` overlaps `a`.
inline bool Overlaps(const UnkBox_la47f08& a, f32 min_x, f32 min_y, f32 max_x, f32 max_y) {
    return max_x >= a.min_x && min_y <= a.max_y && max_y >= a.min_y && min_x <= a.max_x;
}

// Whether one of the body's shapes in `mask` touches the trigger's collider, and where.
inline bool FindContact(UnkTrigger_la47f08* self, UnkCollisionContainer_l864374* body, u32 mask,
                        UnkPoint_la47f08* point) {
    base::global::CRntVector<UnkCollisionRecord_l864374*>& records = g_unk_717274ac30;
    records.count = 0;
    fn_7100864374(body, mask, &records, false);
    for (UnkCollisionRecord_l864374** it = records.begin(); it != records.end(); ++it) {
        UnkTriggerCollider_la47f08* other = reinterpret_cast<UnkTriggerCollider_la47f08*>((*it)->object);
        UnkBox_la47f08 mine = self->collider->Bounds();
        const UnkBox_la47f08& box = other->Bounds();
        if (!Overlaps(mine, box.min_x, box.min_y, box.max_x, box.max_y))
            continue;
        UnkTriggerSink_la47f08 sink;
        UnkShapeTester_A5_b68* tester = Tester();
        if (Test(tester, self->collider->shape, Transform(self->collider), other->shape, Transform(other), &sink).bits &
            1) {
            *point = sink.point;
            return true;
        }
    }
    return false;
}

void fn_7100a47f08(UnkTrigger_la47f08* self, UnkTriggerHits_la47f08* hits) {
    if (!self->collider)
        return;
    UnkGameManager* manager = g_unk_7101d47310;

    if (!self->world) {
        UnkTriggerEntity_la47f08* player = reinterpret_cast<UnkTriggerEntity_la47f08*>(fn_7100dec788(manager));
        if (!player)
            return;
        u32 mask = self->collider->Mask();
        UnkPoint_la47f08 point;
        if (!mask) {
            point = UnkPoint_la47f08();
            if (self->DefaultPoint(&point))
                hits->Add(player, point);
        } else if (FindContact(self, player->body, mask, &point)) {
            hits->Add(player, point);
        }
        return;
    }

    UnkTriggerScene_la47f08* scene = reinterpret_cast<UnkTriggerWorld_la47f08*>(manager)->scene;
    UnkTriggerEntity_la47f08* player = reinterpret_cast<UnkTriggerEntity_la47f08*>(fn_7100dec788(manager));
    for (UnkTriggerNode_la47f08* node = scene->entities; node; node = node->next) {
        UnkTriggerEntity_la47f08* object = node->entity;
        if (!object)
            continue;
        bool is_entity = fn_710009d50c(
            reinterpret_cast<const CClass* (*)(UnkTriggerEntity_la47f08*)>(object->vtable[0])(object), fn_7100d70c20());
        UnkTriggerEntity_la47f08* entity = is_entity ? object : nullptr;
        UnkPoint_la47f08 point;
        if (is_entity && entity == player) {
            u32 mask = self->collider->Mask();
            if (!mask) {
                point = UnkPoint_la47f08();
                if (self->DefaultPoint(&point))
                    hits->Add(player, point);
            } else if (FindContact(self, player->body, mask, &point)) {
                hits->Add(player, point);
            }
            continue;
        }
        if (!is_entity || !entity->field_0x1b4)
            continue;
        if (entity->field_0x1b0 && !entity->field_0x1a8)
            continue;
        UnkCollisionContainer_l864374* body = entity->body;
        if (!body || !self->Accepts(entity))
            continue;

        // The trigger's layer when the body has shapes there, else its mask (all of them without one).
        u32 mask = self->collider->Mask() ? self->collider->Mask() : 0x7fffffff;
        f32 min_x;
        f32 min_y;
        f32 max_x;
        f32 max_y;
        bool on_layer = false;
        if (self->layer != 0) {
            UnkBodyBox_la47f08 box = fn_71008653c0(body, self->layer, false);
            min_x = box.min_x;
            min_y = box.min_y;
            max_x = box.max_x;
            max_y = box.max_y;
            on_layer = max_x >= min_x && max_y >= min_y;
        }
        if (on_layer) {
            mask = self->layer;
        } else {
            UnkBodyBox_la47f08 box = fn_7100865548(body, mask, false);
            min_x = box.min_x;
            min_y = box.min_y;
            max_x = box.max_x;
            max_y = box.max_y;
        }
        if (!Overlaps(self->collider->Bounds(), min_x, min_y, max_x, max_y))
            continue;
        if (FindContact(self, body, mask, &point))
            hits->Add(entity, point);
    }
}
