// Unit 365 (tools/tu_map.py, hint navmesh::CNavMeshGenerator): inserts a point into a sorted pair of
// parallel singly linked lists (points and their keys along a direction), unless a point with the same key
// (within a thousandth) is already there. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "types.h"

// A singly linked list of 4- or 8-byte values (count at +8, head and tail at +0x10/+0x18, nodes of
// {value, next} from an allocator at +0x20 with flags at +0x28); base::UnkSList's layout.
template <class T>
struct UnkSortedList_ovb0229 {
    struct Node {
        T value;
        Node* next;
    };
    u64 field_0x0;
    union {
        u32 count;
    };
    Node* head;
    Node* tail;
    base::UnkAllocator* allocator;
    u32 flags;

    __attribute__((always_inline)) void PushBack(const T& v) {
        Node* n = static_cast<Node*>(allocator->vf_0x40(sizeof(Node), 8, flags));
        if (!n)
            return;
        new (&n->value) T(v);
        n->next = nullptr;
        if (count == 0) {
            head = tail = n;
            count = 1;
        } else {
            tail->next = n;
            n->next = nullptr;
            tail = n;
            ++count;
        }
    }
};

struct UnkPoint_ovb0229 {
    u32 x;
    u32 y;
    UnkPoint_ovb0229(const UnkPoint_ovb0229& other) : x(other.x), y(other.y) {}
};

struct UnkVec2_ovb0229 {
    f32 x;
    f32 y;
};

using UnkPoints_ovb0229 = UnkSortedList_ovb0229<UnkPoint_ovb0229>;
using UnkKeys_ovb0229 = UnkSortedList_ovb0229<u32>;

struct UnkProjection_l2a5e50 {
    UnkPoints_ovb0229* points;  // 0x0
    UnkKeys_ovb0229* keys;  // 0x8
    UnkVec2_ovb0229* direction;  // 0x10
    UnkVec2_ovb0229* origin;  // 0x18
};

void fn_71002a5fe8(UnkPoints_ovb0229* list, const UnkPoint_ovb0229* value, UnkPoints_ovb0229::Node* pos);
void fn_71002a6124(UnkKeys_ovb0229* list, const u32* value, UnkKeys_ovb0229::Node* pos);

void fn_71002a5e50(UnkProjection_l2a5e50* self, const UnkPoint_ovb0229* point) {
    const UnkVec2_ovb0229* p = reinterpret_cast<const UnkVec2_ovb0229*>(point);
    f32 dx = p->x - self->origin->x;
    f32 dy = p->y - self->origin->y;
    f32 key = dx * self->direction->x + dy * self->direction->y;
    UnkPoints_ovb0229* points = self->points;
    UnkPoints_ovb0229::Node* node = points->head;
    if (node) {
        UnkKeys_ovb0229::Node** key_it = &self->keys->head;
        f32 low = key - 0.001f;
        f32 high = key + 0.001f;
        do {
            UnkKeys_ovb0229::Node* key_node = *key_it;
            f32 other;
            __builtin_memcpy(&other, &key_node->value, sizeof(other));
            if (high >= other && low <= other)
                return;
            if (key < other) {
                fn_71002a5fe8(points, point, node);
                fn_71002a6124(self->keys, reinterpret_cast<const u32*>(&key), key_node);
                return;
            }
            node = node->next;
            key_it = &key_node->next;
        } while (node);
    }
    points->PushBack(*point);
    self->keys->PushBack(*reinterpret_cast<const u32*>(&key));
}
