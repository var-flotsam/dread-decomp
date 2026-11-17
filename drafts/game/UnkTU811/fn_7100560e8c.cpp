// CEmmyAIComponent's unit (tools/tu_map.py unit 811): offers a neighbour node to the path search.
// Skips it when it is closed; when it is already open, takes the cheaper cost and reorders the open
// heap; otherwise copies it into the node pool and pushes it, unless a list is full (state 4) or its
// owner is blocked. Names and layouts are placeholders.
#include <algorithm>
#include <math.h>
#include <new>

#include "game/UnkTU811.h"
#include "types.h"

struct UnkPathPos_l560e8c {
    f32 x;
    f32 y;
    UnkPathPos_l560e8c(const UnkPathPos_l560e8c& other) : x(other.x), y(other.y) {}
};

struct UnkPathScale_l560e8c {
    u8 pad_0x0[0xc4];
    f32 scale;  // 0xc4
};

struct UnkPathItem_l560e8c {
    u8 pad_0x0[0x10];
    void* owner;  // 0x10
    u8 pad_0x18[0x68 - 0x18];
    UnkPathScale_l560e8c* metrics;  // 0x68
};

struct UnkPathNode_l560e8c;

struct UnkPathNodeVtbl_l560e8c {
    void (*Destroy)(UnkPathNode_l560e8c* self);
    void (*vf_0x8)(UnkPathNode_l560e8c* self);
    UnkPathPos_l560e8c (*GetPos)(const UnkPathNode_l560e8c* self);
};

struct UnkPathNode_l560e8c {
    const UnkPathNodeVtbl_l560e8c* vtable;
    UnkPathItem_l560e8c* item;     // 0x8
    UnkPathNode_l560e8c* parent;   // 0x10
    f32 cost;                      // 0x18
    f32 estimate;                  // 0x1c

    // The copy takes the node class's own vtable (g_unk_7101a48c20).
    UnkPathNode_l560e8c(const UnkPathNode_l560e8c& other)
        : vtable(reinterpret_cast<const UnkPathNodeVtbl_l560e8c*>(g_unk_7101a48c20 + 2)), item(other.item),
          parent(other.parent), cost(other.cost), estimate(other.estimate) {}

    UnkPathPos_l560e8c GetPos() const { return vtable->GetPos(this); }
    f32 Total() const { return cost + estimate; }
};
static_assert(sizeof(UnkPathNode_l560e8c) == 0x20, "UnkPathNode_l560e8c layout");

#pragma pack(push, 4)
struct UnkPathSet_l560e8c {
    u32 count;
    void* items[1];
};

struct UnkPathSearch_l560e8c {
    u32 openCount;                          // 0x0
    UnkPathNode_l560e8c* open[100];         // 0x4
    u32 closedCount;                        // 0x324
    UnkPathNode_l560e8c* closed[100];       // 0x328
    u32 nodeCount;                          // 0x648
    UnkPathNode_l560e8c nodes[100];         // 0x64c
    u8 pad_0x12cc[0x15f0 - 0x12cc];
    s32 state;                              // 0x15f0
    u8 pad_0x15f4[0x1608 - 0x15f4];
    UnkPathNode_l560e8c* start;             // 0x1608
    u8 pad_0x1610[0x1640 - 0x1610];
    UnkPathSet_l560e8c* blocked;            // 0x1640
};
#pragma pack(pop)
static_assert(__builtin_offsetof(UnkPathSearch_l560e8c, blocked) == 0x1640, "UnkPathSearch_l560e8c layout");

static inline bool HigherTotal(const UnkPathNode_l560e8c* a, const UnkPathNode_l560e8c* b) {
    return a->Total() > b->Total();
}

bool fn_7100560e8c(UnkPathSearch_l560e8c* self, UnkPathNode_l560e8c* node) {
    if (!node)
        return false;
    for (UnkPathNode_l560e8c** it = self->closed; it != self->closed + self->closedCount; ++it) {
        if (node->item == (*it)->item)
            return false;
    }

    f32 cost = node->cost + 1.0f;
    UnkPathPos_l560e8c a = self->start->GetPos();
    UnkPathPos_l560e8c b = node->GetPos();
    f32 dx = a.x - b.x;
    f32 dy = a.y - b.y;
    f32 estimate = sqrtf(dx * dx + dy * dy) * node->item->metrics->scale;

    UnkPathNode_l560e8c** openEnd = self->open + self->openCount;
    UnkPathNode_l560e8c** it = std::find(self->open, openEnd, node);
    if (it != openEnd) {
        UnkPathNode_l560e8c* open = *it;
        if (!(open->Total() > cost + estimate))
            return false;
        open->parent = node;
        open->cost = cost;
        open->estimate = estimate;
        std::make_heap(self->open, self->open + self->openCount, HigherTotal);
        return true;
    }

    if (self->openCount == 100 || self->nodeCount == 100) {
        self->state = 4;
        return false;
    }
    if (UnkPathSet_l560e8c* blocked = self->blocked) {
        void** end = blocked->items + blocked->count;
        if (std::find(blocked->items, end, node->item->owner) != end)
            return false;
    }

    UnkPathNode_l560e8c* added = new (&self->nodes[self->nodeCount++]) UnkPathNode_l560e8c(*node);
    added->cost = cost;
    added->estimate = estimate;
    self->open[self->openCount++] = added;
    std::push_heap(self->open, self->open + self->openCount, HigherTotal);
    return true;
}
