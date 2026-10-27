// The navigation-mesh query state (tools/tu_map.py unit 356, fn_71002810b0 to fn_710028659c): the
// 0x5ddf0-byte object of game/UnkNavMeshManager.h that runs an A* search over the cells of the
// current mesh. Its open list is a fixed array of up to 1200 node pointers (count at +0x8) kept as
// a binary heap on the estimated total cost, and the nodes themselves are 0x128-byte records in a
// fixed array of 1200 (count at +0x7094, storage at +0x7098). A node names a cell and a side of it,
// carries a 0x100-byte payload (game/UnkObject_0x100.h), its position, the cost so far, the
// estimate to the goal and its parent; its constructor, destructor and setters live in the node's
// own unit (fn_71002871cc and after). This file holds the step that opens or improves a node
// (fn_710028340c) and the cost update of a node from two points (fn_7100286298).
#include <algorithm>
#include <new>

#include "base/math/CVector.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkObject_0x100.h"
#include "types.h"

using base::math::CVector2D;

extern "C" f32 sqrtf(f32);

// The payload's default constructor and assignment (its copy constructor and destructor are in
// game/UnkObject_0x100.h).
void fn_71002b57bc(UnkObject_0x100* self);
UnkObject_0x100* fn_71002b5c90(UnkObject_0x100* self, const UnkObject_0x100* other);

struct UnkNavPathNode;
struct UnkNavPathPoint;
void fn_71002871cc(UnkNavPathNode* self);  // the node's constructor
void fn_71002872f4(UnkNavPathNode* self);  // the node's destructor (destroys the payload)
// Sets the cell, side, kind and position (and, with a mesh and a cell, +0x108 from the cell).
void fn_7100287298(UnkNavPathNode* self, UnkNavMesh* mesh, u16 cell, u8 side, u32 kind, const UnkNavPathPoint* pos);
// Sets the three costs (+0x11c, +0x114, +0x118).
void fn_71002872f8(UnkNavPathNode* self, f32 distance, f32 cost, f32 estimate);

// The payload as a node member: copied with fn_71002b58c0, destroyed by the node's destructor.
// Only the fields read here are spelled out (src/game_lo/fn_71002b58c0.cpp has more).
struct alignas(8) UnkNavPathNodeData {
    u8 field_0x0;
    u8 field_0x1;
    s16 field_0x2;
    u8 pad_0x4[0x1c - 0x4];
    u8 field_0x1c;
    u8 pad_0x1d;
    s16 field_0x1e;
    u8 pad_0x20[0x4c - 0x20];
    u8 field_0x4c;
    u8 pad_0x4d[0x100 - 0x4d];

    UnkNavPathNodeData() {}
    UnkNavPathNodeData(const UnkNavPathNodeData& other) { fn_71002b58c0(Object(), other.Object()); }
    UnkObject_0x100* Object() { return reinterpret_cast<UnkObject_0x100*>(this); }
    const UnkObject_0x100* Object() const { return reinterpret_cast<const UnkObject_0x100*>(this); }
};

// A point of a node, copied member by member.
struct UnkNavPathPoint {
    f32 x;
    f32 y;

    UnkNavPathPoint() {}
    UnkNavPathPoint(f32 x, f32 y) : x(x), y(y) {}
    UnkNavPathPoint(const UnkNavPathPoint& other) : x(other.x), y(other.y) {}
};

// A search node (0x128 bytes). The constructor, destructor and the two setters are out of line in
// the node's unit; the copy constructor is the implicit one.
struct UnkNavPathNode {
    UnkNavPathNodeData data;
    u16 cell;
    u8 side;
    u32 kind;
    u32 field_0x108;
    UnkNavPathPoint pos;
    // The costs, also seen as an array (a union: the original reloads them after every move in the
    // open list, as accesses through a union may alias anything).
    union {
        struct {
            f32 cost;      // the cost so far
            f32 estimate;  // the estimate to the goal
            f32 field_0x11c;
        };
        f32 costs[3];
    };
    UnkNavPathNode* parent;

    UnkNavPathNode() { fn_71002871cc(this); }
    ~UnkNavPathNode() { fn_71002872f4(this); }
    void Init(UnkNavMesh* mesh, u16 cell, u8 side, u32 kind, const UnkNavPathPoint& pos) {
        fn_7100287298(this, mesh, cell, side, kind, &pos);
    }
    void SetCosts(f32 distance, f32 cost, f32 estimate) { fn_71002872f8(this, distance, cost, estimate); }

    f32 Total() const { return cost + estimate; }
};
static_assert(sizeof(UnkNavPathNode) == 0x128, "UnkNavPathNode layout");

// A fixed-capacity array with byte storage right after its count.
template <class T, u32 N>
struct UnkNavFixedArray {
    u32 count;
    u8 storage[N * sizeof(T)];

    T* begin() { return reinterpret_cast<T*>(storage); }
    T* end() { return begin() + count; }
    bool IsFull() const { return count == N; }
    void PushBack(const T& value) {
        new (begin() + count) T(value);
        ++count;
    }
    T& Back() { return *(end() - 1); }
};

// The cost callback: a function and the context it is called with.
struct UnkNavPathCostFn {
    typedef f32 (*Fn)(f32 distance, void* context, UnkNavPathNode* parent, u16 cell, u8 side,
                      const UnkNavPathPoint* pos, void* unused, void* state);
    void* context;
    Fn fn;

    f32 operator()(f32 distance, void* state, UnkNavPathNode* parent, u16 cell, u8 side, const UnkNavPathPoint* pos,
                   void* unused) const {
        return fn(distance, context, parent, cell, side, pos, unused, state);
    }
};

struct UnkNavMeshQueryState {
    UnkNavMesh* mesh;
    UnkNavFixedArray<UnkNavPathNode*, 1200> open;  // +0x8
    u8 pad_0x258c[0x7094 - 0x258c];
    UnkNavFixedArray<UnkNavPathNode, 1200> nodes;  // +0x7094
    u32 status;  // +0x5dc18
    u8 pad_0x5dc1c[4];
    u8 field_0x5dc20[0x20];
    UnkNavPathCostFn cost_fn;   // +0x5dc40
    u8 pad_0x5dc50[0x50];
    bool use_goal;              // +0x5dca0
    CVector2D goal;             // +0x5dca4
};
static_assert(__builtin_offsetof(UnkNavMeshQueryState, nodes) == 0x7094, "UnkNavMeshQueryState layout");
static_assert(__builtin_offsetof(UnkNavMeshQueryState, status) == 0x5dc18, "UnkNavMeshQueryState layout");
static_assert(__builtin_offsetof(UnkNavMeshQueryState, cost_fn) == 0x5dc40, "UnkNavMeshQueryState layout");
static_assert(__builtin_offsetof(UnkNavMeshQueryState, goal) == 0x5dca4, "UnkNavMeshQueryState layout");

// fn_7100284cac is in this unit: the original sees that it keeps no pointer to the cost and the
// estimate (noescape), so it holds them in registers across the node calls.
bool fn_71002847a4(UnkNavMeshQueryState* self, u16 cell, u8 side, UnkNavPathNode* parent,
                   const UnkObject_0x100* data, u32* result);
void fn_7100284cac(UnkNavMeshQueryState* self, UnkNavPathNode* parent, u16 cell, u8 side, UnkNavPathPoint* pos,
                   const UnkObject_0x100* data, __attribute__((noescape)) f32* cost,
                   __attribute__((noescape)) f32* estimate, f32* distance);

static inline const UnkNavPathNodeData* AsNodeData(const UnkObject_0x100* data) {
    return reinterpret_cast<const UnkNavPathNodeData*>(data);
}

// Opens the node for a cell side reached from parent, or lowers the costs of the open node that
// already has it. Returns 1 for a new node, 2 for an improved one, 6 when the open node is no
// worse, 7 when the arrays are full (status 4), else what fn_71002847a4 reported.
// NON_MATCHING(minor): the two capacity checks are tested in the other order (open list first), the
// push path takes the heap length from the open count instead of shifting the byte length back,
// the epilogue sits after the update path instead of before it, and the float registers and the
// order of two cost loads in the heap loops differ.
u32 fn_710028340c(UnkNavMeshQueryState* self, u16 cell, u8 side, UnkNavPathNode* parent,
                  const UnkObject_0x100* data) {
    // The open list's order: a min-heap on the total, ties going to the larger cost so far.
    auto greater = [](const UnkNavPathNode* a, const UnkNavPathNode* b) {
        f32 ta = a->Total();
        f32 tb = b->Total();
        return ta > tb || (ta == tb && a->cost < b->cost);
    };

    u32 result = 0;
    if (!fn_71002847a4(self, cell, side, parent, data, &result))
        return result;

    u32 kind;
    if (data == nullptr)
        kind = parent->kind;
    else
        kind = AsNodeData(data)->field_0x1c != 0xff ? 2 : 1;

    UnkNavPathPoint pos(0.0f, 0.0f);
    f32 cost = 0.0f;
    f32 estimate = 0.0f;
    f32 distance = 0.0f;
    fn_7100284cac(self, parent, cell, side, &pos, data, &cost, &estimate, &distance);

    UnkNavPathNode* found = nullptr;
    for (UnkNavPathNode* n : self->open) {
        if (n->cell == cell && (kind == 1 || n->side == side)) {
            found = n;
            break;
        }
    }

    if (found != nullptr) {
        if (!(found->Total() > cost + estimate))
            return 6;
        found->parent = parent;
        found->Init(self->mesh, cell, side, kind, pos);
        found->SetCosts(distance, cost, estimate);
        // found->data = data ? *data : a default payload, through a temporary.
        alignas(8) u8 storage[sizeof(UnkObject_0x100)];
        UnkObject_0x100* copy = reinterpret_cast<UnkObject_0x100*>(storage);
        if (data != nullptr)
            new (copy) UnkObject_0x100(*data);
        else
            fn_71002b57bc(copy);
        fn_71002b5c90(found->data.Object(), copy);
        copy->~UnkObject_0x100();
        std::make_heap(self->open.begin(), self->open.end(), greater);
        return 2;
    }

    if (self->nodes.IsFull() || self->open.IsFull()) {
        self->status = 4;
        return 7;
    }
    self->nodes.PushBack(UnkNavPathNode());
    UnkNavPathNode* node = &self->nodes.Back();
    node->Init(self->mesh, cell, side, kind, pos);
    node->parent = parent;
    node->SetCosts(distance, cost, estimate);
    if (data != nullptr)
        fn_71002b5c90(node->data.Object(), data);
    self->open.PushBack(node);
    std::push_heap(self->open.begin(), self->open.end(), greater);
    return 1;
}

static inline bool IsNearZero(f32 v) { return v + -0.001f <= 0.0f && v + 0.001f >= 0.0f; }

// Sets a node's costs for the step from `from` to `to`: the distance, the cost the callback at
// +0x5dc40 makes of it (the distance itself without one) and, when a goal is set and the node's
// payload asks for it, the distance from `to` to the goal.
void fn_7100286298(UnkNavMeshQueryState* self, UnkNavPathNode* node, const CVector2D* from, const CVector2D* to) {
    f32 dx = to->x - from->x;
    f32 dy = to->y - from->y;
    f32 distance;
    if (IsNearZero(dx) && IsNearZero(dy))
        distance = 0.0f;
    else
        distance = sqrtf(dx * dx + dy * dy);

    f32 cost = distance;
    if (self->cost_fn.fn != nullptr)
        cost = self->cost_fn(distance, self->field_0x5dc20, node->parent, node->cell, node->side, &node->pos, nullptr);

    f32 estimate = 0.0f;
    if (self->use_goal && (node->data.field_0x2 == -1 || node->data.field_0x1e == -1 || node->data.field_0x4c == 0)) {
        f32 gx = to->x - self->goal.x;
        f32 gy = to->y - self->goal.y;
        estimate = sqrtf(gx * gx + gy * gy);
    }
    node->SetCosts(distance, cost, estimate);
}
