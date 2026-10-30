// The CLogicDroppedItemManager unit (tools/tu_map.py: 0x7100e1e35c-0x7100e31c18, whose reflected
// class is CLogicDroppedItemManager::CTunableDroppableBillboard): the dropped-item manager's
// update. Positions reported during the frame are summed; the update hands their mean to another
// manager with one of three entries chosen by how many there were (the tunable's thresholds), then
// updates the dropped items and erases those that are done. The names and layouts are
// placeholders.
// Holds fn_7100e2d39c (the update) and fn_7100e2d93c (a segment test against the collision
// world).
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "base/math/UnkZeroVector3.h"
#include "game/UnkGameManager.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// The tunable (CTunableDroppableBillboard's instance): the counts above which the second and the
// third entry are used.
struct UnkDroppableTunable {
    u8 pad_0x0[0x90];
    u32 field_0x90;
    u32 field_0x94;
};
extern UnkDroppableTunable g_unk_71727c10c8;

// The manager the mean position goes to (a singleton reached through the GOT).
struct UnkDropTarget;
extern UnkDropTarget* g_unk_7101d47318;

struct UnkDropEntry {
    u8 data[0x18];
};

struct UnkDroppedItem {
    u8 pad_0x0[0xc];
    u32 field_0xc;  // zero once the item is done
    u8 pad_0x10[0x68];
};
static_assert(sizeof(UnkDroppedItem) == 0x78, "UnkDroppedItem placeholder layout");

struct UnkDroppedItems {
    UnkDroppedItem* data;
    u64 field_0x8;
    u32 count;

    UnkDroppedItem* begin() { return data; }
    UnkDroppedItem* end() { return data + count; }
};

struct UnkDroppedItemManager {
    u8 pad_0x0[0xa010];
    base::math::CVector3D field_0xa010;  // the sum of this frame's positions
    u32 field_0xa01c;                    // how many there were
    UnkDropEntry field_0xa020[3];
    u8 pad_0xa068[0x60];
    UnkDroppedItems field_0xa0c8;
};
static_assert(__builtin_offsetof(UnkDroppedItemManager, field_0xa0c8) == 0xa0c8,
              "UnkDroppedItemManager placeholder layout");

struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager* manager);
void fn_71010247e4(UnkDropTarget* self, UnkDropEntry* entry, UnkPlayer* player, const base::math::CVector3D* pos,
                   void* arg4, u32 arg5);
void fn_7100e2ae2c(UnkDroppedItem* self);
UnkDroppedItem* fn_7100e2f398(UnkDroppedItems* self, UnkDroppedItem* item);

// NON_MATCHING(minor): scheduling at the item loop's entry: the original loads the items' data
// pointer (through a pre-indexed load from the manager) before testing the count; ours tests the
// count first.
void fn_7100e2d39c(UnkDroppedItemManager* self) {
    base::math::CVector3D& sum = self->field_0xa010;
    if (self->field_0xa01c != 0) {
        u64 tier;
        if (self->field_0xa01c > g_unk_71727c10c8.field_0x90)
            tier = self->field_0xa01c > g_unk_71727c10c8.field_0x94 ? 2 : 1;
        else
            tier = 0;
        UnkPlayer* player = fn_7100dec788(g_unk_7101d47310);
        f32 inv = 1.0f / (f32)self->field_0xa01c;
        sum.x = sum.x * inv;
        sum.y = inv * sum.y;
        sum.z = inv * sum.z;
        fn_71010247e4(g_unk_7101d47318, &self->field_0xa020[tier], player, &sum, nullptr, 0);
        self->field_0xa01c = 0;
        sum.x = g_unk_710164779c.x;
        sum.y = g_unk_710164779c.y;
        sum.z = g_unk_710164779c.z;
    }
    UnkDroppedItems& items = self->field_0xa0c8;
    for (UnkDroppedItem* it = items.begin(); items.end() != it;) {
        fn_7100e2ae2c(it);
        if (it->field_0xc == 0)
            it = fn_7100e2f398(&items, it);
        else
            ++it;
    }
}

// A collision shape (0xc0 bytes) built from a segment (fn_7100d693ac) and destroyed by
// fn_7100d69400.
struct UnkSegmentShape {
    u8 data[0xc0];
};

// The hits of a collision query: up to 32 records of 0x48 bytes after the count, each starting
// with two reference-counted pointers, which the destructor (vtable g_unk_7101a70eb0) releases.
struct __attribute__((packed)) UnkDropHitRecord {
    UnkRefObject* field_0x0;
    UnkRefObject* field_0x8;
    u8 pad_0x10[0x38];
};
extern void* g_unk_7101a70eb0[];

static inline void ReleaseRef(UnkRefObject* p) {
    if (p && --p->ref_count == 0)
        p->vf_0x10();
}

struct __attribute__((packed)) UnkDropHitList {
    void** vtable;
    u32 count;
    u8 storage[32 * sizeof(UnkDropHitRecord)];

    UnkDropHitRecord* items() { return reinterpret_cast<UnkDropHitRecord*>(storage); }

    UnkDropHitList() {
        vtable = g_unk_7101a70eb0 + 2;
        count = 0;
    }
    ~UnkDropHitList() {
        vtable = g_unk_7101a70eb0 + 2;
        for (u64 i = 0; i < count; i++) {
            ReleaseRef(items()[i].field_0x8);
            ReleaseRef(items()[i].field_0x0);
        }
        count = 0;
    }
};

// The collision query (src/game_hi/fn_7100d5b484.cpp, whose names for the tree, the shape and the
// matrix these are).
struct UnkTree;
struct UnkShape;
struct UnkMtx44;
bool fn_7100d5b484(UnkTree* self, u32 mask, UnkShape* shape, const UnkMtx44* xf, void* arg, u16 arg2);
void fn_7100d693ac(UnkSegmentShape* self, const UnkSegment2D* seg);
void fn_7100d69400(UnkSegmentShape* self);

static inline void SetIdentity(UnkMatrix44* m) {
    *m = UnkMatrix44{};
    m->m[15] = 1.0f;
    m->m[10] = 1.0f;
    m->m[5] = 1.0f;
    m->m[0] = 1.0f;
}

// Queries layer 1 of the collision world along the segment from `a` to `b` (false for a
// degenerate one). The result is the low bit of the hit count, as the original computes it.
bool fn_7100e2d93c(void* self, const base::math::CVector2D* a, const base::math::CVector2D* b) {
    if (a->x == b->x && a->y == b->y)
        return false;
    UnkSegmentShape shape;
    {
        base::math::CVector2D start;
        start.x = a->x;
        start.y = a->y;
        base::math::CVector2D end;
        end.x = b->x;
        end.y = b->y;
        UnkSegment2D seg(start, end);
        fn_7100d693ac(&shape, &seg);
    }
    bool result;
    {
        UnkDropHitList hits;
        UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);
        UnkMatrix44 identity;
        SetIdentity(&identity);
        fn_7100d5b484(tree, 1, reinterpret_cast<UnkShape*>(&shape), reinterpret_cast<const UnkMtx44*>(&identity),
                      &hits, 0);
        result = (hits.count & 1) != 0;
    }
    fn_7100d69400(&shape);
    return result;
}

// An identical copy (fn_71007bae10).
bool fn_71007bae10(void* self, const base::math::CVector2D* a, const base::math::CVector2D* b) {
    if (a->x == b->x && a->y == b->y)
        return false;
    UnkSegmentShape shape;
    {
        base::math::CVector2D start;
        start.x = a->x;
        start.y = a->y;
        base::math::CVector2D end;
        end.x = b->x;
        end.y = b->y;
        UnkSegment2D seg(start, end);
        fn_7100d693ac(&shape, &seg);
    }
    bool result;
    {
        UnkDropHitList hits;
        UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);
        UnkMatrix44 identity;
        SetIdentity(&identity);
        fn_7100d5b484(tree, 1, reinterpret_cast<UnkShape*>(&shape), reinterpret_cast<const UnkMtx44*>(&identity),
                      &hits, 0);
        result = (hits.count & 1) != 0;
    }
    fn_7100d69400(&shape);
    return result;
}
