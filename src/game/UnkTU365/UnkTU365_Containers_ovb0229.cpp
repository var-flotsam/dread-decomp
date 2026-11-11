// Unit 365 (tools/tu_map.py, hint navmesh::CNavMeshGenerator; no reliable name): array and list helpers
// next to the navmesh generator. In order:
// - the copy constructor of an array of 0x108-byte records (fn_71002aa29c) and the erase of a 0x60-byte record
//   from an array (fn_71002a6248);
// - the sorted insert of a point into three parallel lists (fn_71002a8f58);
// - the generator pass naming and handing out the regions (fn_7100297250);
// - the copy constructor of an object holding three linked lists (fn_71002a9710).
// Names and layouts are placeholders.
#include <new>
#include <stdio.h>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkObject_0x100.h"
#include "game/UnkTU109.h"
#include "game/UnkTU365.h"
#include "types.h"

// ---- The record array copy ----

// the copy constructor of an array of 0x108-byte
// records (a word, then a UnkObject_0x100): the other's step, flags and allocator, its capacity reserved,
// then each record appended. Names are placeholders.
struct UnkObjectRecord_ovb0229 {
    u32 field_0x0;
    UnkObject_0x100 object;  // 0x8

    UnkObjectRecord_ovb0229(const UnkObjectRecord_ovb0229& other) : field_0x0(other.field_0x0), object(other.object) {}
};
static_assert(sizeof(UnkObjectRecord_ovb0229) == 0x108, "UnkObjectRecord_ovb0229 layout");

using UnkObjectRecords_ovb0229 = base::UnkVector<UnkObjectRecord_ovb0229>;

void fn_71002aa29c(UnkObjectRecords_ovb0229* self, const UnkObjectRecords_ovb0229* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    self->Reserve(other->capacity);
    for (const UnkObjectRecord_ovb0229 *it = other->data, *end = other->data + other->count; it != end; ++it)
        self->PushBackCopy(*it);
}

// ---- The record erase ----

// erases the 0x60-byte record at an index from an
// array: destroys it and moves the later ones down by one (UnkVector::EraseAt, inlined; the moved-from record
// is destroyed again, its storage reloaded through the array). A record is a string id, a few small fields and
// two growable arrays. Names and layouts are placeholders.
using base::global::CStrId;

// base::UnkVector's layout with plain members.
struct UnkRawArray_ovb0229 {
    u32* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    void Free() { reinterpret_cast<base::UnkVector<u32>*>(this)->Free(); }
};

struct UnkNavRecord_ovb0229 {
    base::global::UnkStringData* id;  // 0x0, a CStrId
    u16 field_0x8;
    u8 field_0xa;
    UnkRawArray_ovb0229 first;  // 0x10
    UnkRawArray_ovb0229 second;  // 0x38

    // Moves the other's contents in; its id becomes the empty one and its arrays empty.
    UnkNavRecord_ovb0229(UnkNavRecord_ovb0229&& other) {
        id = other.id;
        other.id = &g_unk_7101d47108->empty;
        field_0xa = other.field_0xa;
        field_0x8 = other.field_0x8;
        __builtin_memcpy(&first, &other.first, 16);
        __builtin_memset(&other.first, 0, 16);
        first.step = other.first.step;
        first.field_0x14 = other.first.field_0x14;
        first.flags = other.first.flags;
        first.allocator = other.first.allocator;
        second.data = other.second.data;
        second.count = other.second.count;
        second.capacity = other.second.capacity;
        second.step = other.second.step;
        second.field_0x14 = other.second.field_0x14;
        second.flags = other.second.flags;
        second.allocator = other.second.allocator;
        other.second.data = nullptr;
        other.second.count = 0;
        other.second.capacity = 0;
    }
    ~UnkNavRecord_ovb0229() {
        second.Free();
        first.Free();
        reinterpret_cast<CStrId*>(&id)->Reset();
    }
};
static_assert(sizeof(UnkNavRecord_ovb0229) == 0x60, "UnkNavRecord_ovb0229 layout");

void fn_71002a6248(base::UnkVector<UnkNavRecord_ovb0229>* self, u32 index) {
    if (index < self->count) {
        u64 i = index;
        self->data[i].~UnkNavRecord_ovb0229();
        for (; i < (u32)(self->count - 1); ++i) {
            UnkNavRecord_ovb0229* cur = &self->data[i];
            new (cur) UnkNavRecord_ovb0229(static_cast<UnkNavRecord_ovb0229&&>(cur[1]));
            (&self->data[i])[1].~UnkNavRecord_ovb0229();
        }
        self->count = self->count - 1;
    }
}

// ---- The sorted point insert ----

// inserts a point into a sorted set of three
// parallel singly linked lists (points, their keys along a direction, and ids), unless a point with the same
// key (within a thousandth) is already there. The key is the point's offset from an origin projected on a
// direction. Inserting before a larger key goes through each list's out-of-line insert; past the last one the
// three values are pushed back inline. Names and layouts are placeholders.
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
using UnkIds_ovb0229 = UnkSortedList_ovb0229<s32>;

struct UnkProjection_ovb0229 {
    UnkPoints_ovb0229* points;  // 0x0
    UnkKeys_ovb0229* keys;  // 0x8
    UnkIds_ovb0229* ids;  // 0x10
    UnkVec2_ovb0229* direction;  // 0x18
    UnkVec2_ovb0229* origin;  // 0x20
};

void fn_71002a5fe8(UnkPoints_ovb0229* list, const UnkPoint_ovb0229* value, UnkPoints_ovb0229::Node* pos);
void fn_71002a6124(UnkKeys_ovb0229* list, const u32* value, UnkKeys_ovb0229::Node* pos);
void fn_71002a93e0(UnkIds_ovb0229* list, const s32* value, UnkIds_ovb0229::Node* pos);

void fn_71002a8f58(UnkProjection_ovb0229* self, const UnkPoint_ovb0229* point, s32 id) {
    const UnkVec2_ovb0229* p = reinterpret_cast<const UnkVec2_ovb0229*>(point);
    f32 dx = p->x - self->origin->x;
    f32 dy = p->y - self->origin->y;
    f32 key = dx * self->direction->x + dy * self->direction->y;
    UnkPoints_ovb0229* points = self->points;
    UnkPoints_ovb0229::Node* node = points->head;
    if (node) {
        UnkIds_ovb0229::Node** id_it = &self->ids->head;
        UnkKeys_ovb0229::Node** key_it = &self->keys->head;
        f32 low = key - 0.001f;
        f32 high = key + 0.001f;
        do {
            UnkKeys_ovb0229::Node* key_node = *key_it;
            f32 other;
            __builtin_memcpy(&other, &key_node->value, sizeof(other));
            if (high >= other && low <= other)
                return;
            UnkIds_ovb0229::Node* id_node = *id_it;
            if (key < other) {
                fn_71002a5fe8(points, point, node);
                fn_71002a6124(self->keys, reinterpret_cast<const u32*>(&key), key_node);
                fn_71002a93e0(self->ids, &id, id_node);
                return;
            }
            node = node->next;
            key_it = &key_node->next;
            id_it = &id_node->next;
        } while (node);
    }
    points->PushBack(*point);
    self->keys->PushBack(*reinterpret_cast<const u32*>(&key));
    self->ids->PushBack(id);
}

// ---- The region pass ----

// after fn_7100295d28, walks the generator's
// array of regions four times: names each "Isolated<index>" (string id) and hands it to fn_71002916e8,
// then runs the three later passes over each region's parts. Names and layouts are placeholders.
using base::global::CStrId;

struct UnkNavGenerator_ovb0229;
struct UnkNavRegions_ovb0229;

void fn_7100295d28(UnkNavGenerator_ovb0229* self, void* arg);
u8* fn_71002970ec(UnkNavRegions_ovb0229* regions, u32 index);
void fn_71002916e8(UnkNavGenerator_ovb0229* self, u8* region, u8* part, const CStrId& name, f32 value);
void fn_7100297468(u8* region, u64 unused, u8* part_a, u8* part_b);
void fn_71002978c8(UnkNavGenerator_ovb0229* self, u8* region, u8* part_a, u8* part_b);
void fn_710028e928(UnkNavGenerator_ovb0229* self, u8* region, u8* part_a, u8* part_b);

struct UnkNavRegions_ovb0229 {
    u8 unk_0x0[8];
    u32 count;  // 0x8
};

u32 fn_7100297250(UnkNavGenerator_ovb0229* self, void* arg, UnkNavRegions_ovb0229* regions) {
    fn_7100295d28(self, arg);
    for (u32 i = 0; i < regions->count; ++i) {
        char name[32];
        sprintf(name, "Isolated%d", i);
        fn_71002916e8(self, fn_71002970ec(regions, i), fn_71002970ec(regions, i) + 0x40, CStrId(name, true), -1.0f);
    }
    for (u32 i = 0; i < regions->count; ++i) {
        u64 unused;
        fn_71002970ec(regions, i);
        u8* b = fn_71002970ec(regions, i);
        u8* c = fn_71002970ec(regions, i);
        fn_7100297468(c, unused, b, c + 0x48);
    }
    for (u32 i = 0; i < regions->count; ++i) {
        fn_71002978c8(self, *reinterpret_cast<u8**>(fn_71002970ec(regions, i) + 0x40),
                      fn_71002970ec(regions, i) + 0x48, fn_71002970ec(regions, i) + 0x78);
    }
    for (u32 i = 0; i < regions->count; ++i) {
        fn_710028e928(self, fn_71002970ec(regions, i), fn_71002970ec(regions, i) + 0x78,
                      fn_71002970ec(regions, i) + 0xa8);
    }
    return 0;
}

// ---- The three-list copy ----

// the copy constructor of an object made of two
// copied sub-objects (fn_7100123f58) at +0 and +0xa8, a third at +0x118, and three reflected linked lists
// (+0x48, +0x78, +0xe8) whose nodes are copied one by one. Names and layouts are placeholders.

// A doubly linked node of 0x28 bytes.
struct UnkNode28_ovb0229 {
    u32 field_0x0;
    u32 field_0x4;
    u8 field_0x8[0x10];
    UnkNode28_ovb0229* next;  // 0x18
    UnkNode28_ovb0229* prev;  // 0x20

    UnkNode28_ovb0229(const UnkNode28_ovb0229& other) : field_0x0(other.field_0x0), field_0x4(other.field_0x4) {
        __builtin_memcpy(field_0x8, other.field_0x8, sizeof(field_0x8));
        next = nullptr;
        prev = nullptr;
    }
};

struct UnkList28_ovb0229 {
    void* vtable;  // 0x0
    union {
        u32 count;  // 0x8
    };
    UnkNode28_ovb0229* head;  // 0x10
    UnkNode28_ovb0229* tail;  // 0x18
    u32 flags;  // 0x20
    base::UnkAllocator* allocator;  // 0x28

    void PushBackCopy(const UnkNode28_ovb0229& value) {
        auto* node = static_cast<UnkNode28_ovb0229*>(allocator->vf_0x40(sizeof(UnkNode28_ovb0229), 8, flags));
        if (!node)
            return;
        new (node) UnkNode28_ovb0229(value);
        if (count == 0) {
            head = tail = node;
            count = 1;
        } else {
            // Stored untyped: the original reloads the tail and the count afterwards.
            __builtin_memcpy(&tail->next, &node, sizeof(node));
            node->prev = tail;
            tail = node;
            ++count;
        }
    }
};

// A singly linked node of 0x38 bytes.
struct UnkNode38_ovb0229 {
    u8 field_0x0[0x1c];
    u32 field_0x1c;
    u32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;
    UnkNode38_ovb0229* next;  // 0x30

    UnkNode38_ovb0229(const UnkNode38_ovb0229& other) {
        __builtin_memcpy(field_0x0, other.field_0x0, sizeof(field_0x0));
        field_0x1c = other.field_0x1c;
        field_0x20 = other.field_0x20;
        field_0x24 = other.field_0x24;
        field_0x28 = other.field_0x28;
        next = nullptr;
    }
};

struct UnkList38_ovb0229 {
    void* vtable;  // 0x0
    union {
        u32 count;  // 0x8
    };
    UnkNode38_ovb0229* head;  // 0x10
    UnkNode38_ovb0229* tail;  // 0x18
    base::UnkAllocator* allocator;  // 0x20
    u32 flags;  // 0x28

    void PushBackCopy(const UnkNode38_ovb0229& value) {
        auto* node = static_cast<UnkNode38_ovb0229*>(allocator->vf_0x40(sizeof(UnkNode38_ovb0229), 8, flags));
        if (!node)
            return;
        new (node) UnkNode38_ovb0229(value);
        if (count == 0) {
            head = tail = node;
            count = 1;
        } else {
            tail->next = node;
            node->next = nullptr;
            tail = node;
            ++count;
        }
    }
};

// A singly linked node of one word.
struct UnkNode10_ovb0229 {
    u64 value;
    UnkNode10_ovb0229* next;  // 0x8

    UnkNode10_ovb0229(const UnkNode10_ovb0229& other) : value(other.value), next(nullptr) {}
};

struct UnkList10_ovb0229 {
    void* vtable;  // 0x0
    union {
        u32 count;  // 0x8
    };
    UnkNode10_ovb0229* head;  // 0x10
    UnkNode10_ovb0229* tail;  // 0x18
    base::UnkAllocator* allocator;  // 0x20
    u32 flags;  // 0x28

    void PushBackCopy(const UnkNode10_ovb0229& value) {
        auto* node = static_cast<UnkNode10_ovb0229*>(allocator->vf_0x40(sizeof(UnkNode10_ovb0229), 8, flags));
        if (!node)
            return;
        new (node) UnkNode10_ovb0229(value);
        if (count == 0) {
            head = tail = node;
            count = 1;
        } else {
            tail->next = node;
            node->next = nullptr;
            tail = node;
            ++count;
        }
    }
};

struct UnkShape_ovb0229 {
    u8 field_0x0[0x40];
    u64 field_0x40;
    UnkList28_ovb0229 first;  // 0x48
    UnkList38_ovb0229 second;  // 0x78
    u8 field_0xa8[0x40];
    UnkList10_ovb0229 third;  // 0xe8
    u8 field_0x118[0x40];
};
static_assert(__builtin_offsetof(UnkShape_ovb0229, second) == 0x78, "UnkShape_ovb0229 layout");
static_assert(__builtin_offsetof(UnkShape_ovb0229, third) == 0xe8, "UnkShape_ovb0229 layout");

void fn_71002a9710(UnkShape_ovb0229* self, UnkShape_ovb0229* other) {
    fn_7100123f58(self, other);
    self->field_0x40 = other->field_0x40;
    self->first.vtable = g_unk_7101a20f48 + 0x10;
    self->first.count = 0;
    self->first.head = self->first.tail = nullptr;
    self->first.flags = other->first.flags;
    self->first.allocator = other->first.allocator;
    for (UnkNode28_ovb0229* n = other->first.head; n; n = n->next)
        self->first.PushBackCopy(*n);
    self->second.vtable = g_unk_7101a20f68 + 0x10;
    self->second.count = 0;
    self->second.head = self->second.tail = nullptr;
    self->second.allocator = other->second.allocator;
    self->second.flags = other->second.flags;
    for (UnkNode38_ovb0229* n = other->second.head; n; n = n->next)
        self->second.PushBackCopy(*n);
    fn_7100123f58(self->field_0xa8, other->field_0xa8);
    self->third.vtable = g_unk_7101a20f88 + 0x10;
    self->third.count = 0;
    self->third.head = self->third.tail = nullptr;
    self->third.allocator = other->third.allocator;
    self->third.flags = other->third.flags;
    for (UnkNode10_ovb0229* n = other->third.head; n; n = n->next)
        self->third.PushBackCopy(*n);
    fn_7100123f58(self->field_0x118, other->field_0x118);
}
