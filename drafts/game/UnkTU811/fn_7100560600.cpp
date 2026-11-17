// CEmmyAIComponent's unit (tools/tu_map.py unit 811): starts a path search. Clears the open and closed
// lists and the node pool, copies the request, seeds the pool with the origin and the target node, pushes
// the target onto the open heap and records the direction from origin to target. Names and layouts are
// placeholders.
#include <algorithm>
#include <cfloat>
#include <math.h>

#include "game/UnkTU811.h"
#include "types.h"

struct UnkPathPos_l560600 {
    f32 x;
    f32 y;
    UnkPathPos_l560600(const UnkPathPos_l560600& other) : x(other.x), y(other.y) {}
};

struct UnkPathItem_l560600 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual UnkPathPos_l560600 GetPos() const;
    u8 pad_0x8[8];
    void* owner;  // 0x10
};

struct UnkPathNode_l560600;

struct UnkPathNodeVtbl_l560600 {
    void (*Destroy)(UnkPathNode_l560600* self);
    void (*vf_0x8)(UnkPathNode_l560600* self);
    UnkPathPos_l560600 (*GetPos)(const UnkPathNode_l560600* self);
};

struct UnkPathNode_l560600 {
    const UnkPathNodeVtbl_l560600* vtable;
    UnkPathItem_l560600* item;      // 0x8
    UnkPathNode_l560600* parent;    // 0x10
    f32 cost;                       // 0x18
    f32 estimate;                   // 0x1c
};
static_assert(sizeof(UnkPathNode_l560600) == 0x20, "UnkPathNode_l560600 layout");

#pragma pack(push, 4)
struct UnkPathSet_l560600 {
    u32 count;
    void* items[1];
};

struct UnkPathRequest_l560600 {
    u64 field_0x0;
    bool flag;                       // 0x8
    u32 mode;                        // 0xc
    UnkPathItem_l560600* target;     // 0x10
    UnkPathItem_l560600* origin;     // 0x18
    UnkPathSet_l560600* blocked;     // 0x20
};

struct UnkPathSearch_l560600 {
    u32 openCount;                          // 0x0
    UnkPathNode_l560600* open[100];         // 0x4
    u32 closedCount;                        // 0x324
    UnkPathNode_l560600* closed[100];       // 0x328
    u32 nodeCount;                          // 0x648
    UnkPathNode_l560600 nodes[100];         // 0x64c
    u32 pathCount;                          // 0x12cc
    UnkPathNode_l560600* path[100];         // 0x12d0
    s32 state;                              // 0x15f0
    u8 pad_0x15f4[0x1600 - 0x15f4];
    UnkPathNode_l560600* current;           // 0x1600
    UnkPathNode_l560600* start;             // 0x1608
    f32 distance;                           // 0x1610
    f32 dirX;                               // 0x1614
    f32 dirY;                               // 0x1618
    u32 pad_0x161c;
    UnkPathRequest_l560600 request;         // 0x1620
};
#pragma pack(pop)
static_assert(__builtin_offsetof(UnkPathSearch_l560600, request) == 0x1620, "UnkPathSearch_l560600 layout");

static inline UnkPathNode_l560600* NewNode(UnkPathSearch_l560600* self) {
    UnkPathNode_l560600* node = &self->nodes[self->nodeCount++];
    node->vtable = reinterpret_cast<const UnkPathNodeVtbl_l560600*>(g_unk_7101a48c20 + 2);
    node->item = nullptr;
    node->parent = nullptr;
    node->cost = FLT_MAX;
    node->estimate = FLT_MAX;
    return node;
}

void fn_7100560600(UnkPathSearch_l560600* self, const UnkPathRequest_l560600* req) {
    self->openCount = 0;
    self->closedCount = 0;
    for (u32 i = 0; i < self->nodeCount; i++)
        self->nodes[i].vtable->Destroy(&self->nodes[i]);
    self->nodeCount = 0;
    self->pathCount = 0;
    self->current = nullptr;
    self->start = nullptr;
    self->distance = 0.0f;
    self->dirX = 0.0f;
    self->dirY = 0.0f;
    self->state = 2;
    self->request = *req;

    UnkPathNode_l560600* start = NewNode(self);
    start->item = self->request.origin;
    start->parent = nullptr;
    self->start = start;

    UnkPathNode_l560600* goal = NewNode(self);
    goal->item = self->request.target;
    goal->cost = 0.0f;
    {
        UnkPathPos_l560600 from = self->request.origin->GetPos();
        UnkPathPos_l560600 to = goal->vtable->GetPos(goal);
        f32 dx = from.x - to.x;
        f32 dy = from.y - to.y;
        goal->estimate = sqrtf(dx * dx + dy * dy);
    }
    self->current = goal;

    UnkPathNode_l560600** open = self->open;
    open[self->openCount] = goal;
    self->openCount++;
    std::push_heap(open, open + self->openCount,
                   [](const UnkPathNode_l560600* a, const UnkPathNode_l560600* b) {
                       return a->cost + a->estimate > b->cost + b->estimate;
                   });

    if (req->blocked != nullptr && req->blocked->count != 0) {
        UnkPathSet_l560600* blocked = self->request.blocked;
        void** end = blocked->items + blocked->count;
        if (std::find(blocked->items, end, self->start->item->owner) != end)
            self->state = 4;
    }

    {
        UnkPathPos_l560600 a = self->request.target->GetPos();
        UnkPathPos_l560600 b = self->request.origin->GetPos();
        self->dirY = a.y - b.y;
        self->dirX = a.x - b.x;
    }
    self->distance = sqrtf(self->dirX * self->dirX + self->dirY * self->dirY);
    self->dirX /= self->distance;
    self->dirY /= self->distance;
}
