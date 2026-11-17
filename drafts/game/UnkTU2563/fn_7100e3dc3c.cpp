// UnkTU2563 (CMinimapData's unit): welds a mesh's vertices. Vertices within 0.001 of one already kept
// are dropped, the indices are pointed at the kept vertices, and the vertex array is rebuilt from them
// in the order they were first seen. Names and layouts are placeholders.
#include <new>

#include "base/UnkBlockFreeList.h"
#include "base/UnkHashStream.h"
#include "base/global/CRntVector.h"
#include "game/CGoliathXBurstProjectionBombMovement.h"
#include "game/UnkTU83.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::UnkAllocator;
using base::global::CRntVector;

struct UnkWeldMap_le3dc3c;
void fn_7100e63120(void* map, u32 buckets, u32 nodes);
void fn_7100e3e0b0(UnkWeldMap_le3dc3c* self);

struct UnkWeldMesh_le3dc3c {
    CRntVector<UnkVec3f> vertices;  // 0x0
    CRntVector<u16> indices;        // 0x28
};

// A kept vertex: its position and new index, linked in the order kept and in its bucket.
struct UnkWeldNode_le3dc3c {
    UnkVec3f pos;
    u16 index;                   // 0xc
    UnkWeldNode_le3dc3c* next;   // 0x10
    UnkWeldNode_le3dc3c* prev;   // 0x18
    UnkWeldNode_le3dc3c* chain;  // 0x20

    UnkWeldNode_le3dc3c(const UnkVec3f& pos, u16 index)
        : pos(pos), index(index), next(nullptr), prev(nullptr), chain(nullptr) {}
};

// A chained hash map of the kept vertices (base/UnkHashMap.h layout), its nodes from a block pool;
// fn_7100e63120 sets its storage up and fn_7100e3e0b0 destroys it.
struct UnkWeldMap_le3dc3c {
    UnkAllocator* allocator;
    u32 flags;
    u32 count;  // 0xc
    UnkWeldNode_le3dc3c** buckets;  // 0x10
    u32 bucket_count;               // 0x18
    u32 bucket_capacity;            // 0x1c
    u32 bucket_step;                // 0x20
    bool field_0x24;
    u32 bucket_flags;               // 0x28
    UnkAllocator* bucket_allocator;  // 0x30
    UnkWeldNode_le3dc3c* tail;      // 0x38
    UnkWeldNode_le3dc3c* head;      // 0x40
    char* pool_vtable;              // 0x48
    UnkAllocator* pool_parent;      // 0x50
    UnkBlockFreeList pool_blocks;   // 0x58
    u32 field_0x80;
    bool field_0x84;

    UnkWeldMap_le3dc3c()
        : allocator(fn_7100083990()), flags(0xe), count(0), buckets(nullptr), bucket_count(0), bucket_capacity(0),
          bucket_step(10), field_0x24(true), bucket_flags(0xe), bucket_allocator(fn_7100083990()), tail(nullptr),
          head(nullptr), pool_vtable(g_unk_7101a1a4c8 + 0x10), pool_parent(nullptr) {
        fn_710008377c(&pool_blocks);
        field_0x80 = 0;
        field_0x84 = false;
    }

    UnkWeldNode_le3dc3c** Bucket(UnkVec3f* pos) {
        if (!bucket_count)
            return nullptr;
        return &buckets[fn_710007b750(pos, sizeof(UnkVec3f), -1) % bucket_capacity];
    }
};

// Walks the kept vertices in the order kept, each as its position and index.
struct UnkWeldIter_le3dc3c {
    UnkWeldNode_le3dc3c* node;
    UnkVec3f* pos;
    u16* index;
    s32 state;  // 2 while pos and index are set

    explicit UnkWeldIter_le3dc3c(UnkWeldNode_le3dc3c* first) : node(first), state(0) { Load(); }
    ~UnkWeldIter_le3dc3c() {
        if (state == 2 || state == 1)
            state = 0;
    }
    void Load() {
        if (node) {
            state = 2;
            pos = &node->pos;
            index = &node->index;
        }
    }
    void Next() {
        node = node->next;
        Load();
    }
};

static inline bool Near(f32 kept, f32 value) {
    return kept - 0.001f <= value && value <= kept + 0.001f;
}

static inline bool Near(const UnkVec3f& kept, const UnkVec3f& value) {
    return Near(kept.x, value.x) && Near(kept.y, value.y) && Near(kept.z, value.z);
}

void fn_7100e3dc3c(UnkWeldMesh_le3dc3c* self) {
    u32 vertex_count = self->vertices.count;
    u32 bucket_count = vertex_count / 4;
    UnkWeldMap_le3dc3c map;
    fn_7100e63120(&map, bucket_count, vertex_count);

    u16 kept = 0;
    for (UnkVec3f *v = self->vertices.data, *end = v + self->vertices.count; v != end; ++v) {
        bool found = false;
        if (UnkWeldNode_le3dc3c** bucket = map.Bucket(v)) {
            for (UnkWeldNode_le3dc3c* node = *bucket; node; node = node->chain) {
                if (Near(node->pos, *v)) {
                    found = true;
                    break;
                }
            }
        }
        if (found)
            continue;

        UnkWeldNode_le3dc3c** bucket = map.Bucket(v);
        UnkWeldNode_le3dc3c* first = *bucket;
        void* mem = map.allocator->vf_0x40(sizeof(UnkWeldNode_le3dc3c), 8, map.flags);
        UnkWeldNode_le3dc3c* node = new (mem) UnkWeldNode_le3dc3c(*v, kept);
        node->chain = first;
        *bucket = node;
        if (!map.head) {
            map.head = node;
        } else if (map.tail) {
            node->prev = map.tail;
            map.tail->next = node;
        }
        map.tail = node;
        map.count++;
        kept++;
    }

    for (u16 *it = self->indices.data, *end = it + self->indices.count; it != end; ++it) {
        UnkVec3f* v = reinterpret_cast<UnkVec3f*>(fn_71000cc374(self, *it));
        UnkWeldNode_le3dc3c* node = map.buckets[fn_710007b750(v, sizeof(UnkVec3f), -1) % map.bucket_capacity];
        while (!Near(node->pos, *v))
            node = node->chain;
        *it = node->index;
    }

    self->vertices.Clear();
    self->vertices.Reserve(map.count);
    for (UnkWeldIter_le3dc3c it(map.head); it.node; it.Next())
        fn_710058c768(reinterpret_cast<UnkPushedVector*>(self), reinterpret_cast<const UnkValues3*>(it.pos));
    fn_7100e3e0b0(&map);
}
