// Unit 270 (tools/tu_map.py: 0x71001ea860-0x71001ec8bc), beside src/game/UnkSpherePool.cpp: the pool of 25000 placed
// entries (0xe0 bytes each: a transform, a sphere and a box) and its bookkeeping: the slot table of the
// entries in use (+0x8), two tables of them by kind (+0x588048, +0x5b8d88), a tree of boxes (+0x5e9b88) and the
// buffer of the entries seen from a view (fn_71001ec09c lists them), the tree's growth (fn_71001ec680), a sphere around
// the points of a list of nodes (fn_71001eb6b4) and the sort of the seen entries (fn_71001ea860). Names and layouts
// are placeholders.
#include <new>

#include "base/UnkMatrix44.h"
#include "base/UnkVector.h"
#include "game/UnkSphere.h"
#include "types.h"

// A box with virtual accessors (0x20 bytes): its minimum and maximum corners.
struct UnkPoolBox {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void SetMin(const UnkVec3f* v);  // vf_0x28
    virtual void SetMax(const UnkVec3f* v);  // vf_0x30
    virtual UnkVec3f GetMin();               // vf_0x38
    virtual UnkVec3f GetMax();               // vf_0x40
    UnkVec3f min;
    UnkVec3f max;
};

void fn_7100215dcc(UnkPoolBox* self, const UnkVec3f* min, const UnkVec3f* max);  // the box's base constructor
void fn_7100215e1c(UnkPoolBox* self);                                           // its destructor
extern const char g_unk_7101a20090[];                                           // the box class's vtable

// A box and its corners quantised to tenths (0x40 bytes): a node of the tree. The box is kept as raw storage so
// that the node's constructor does not set the box's vtable (the box constructor does).
struct UnkTreeNode_ovb0254 {
    alignas(8) u8 boxStorage[sizeof(UnkPoolBox)];
    s32 cellMin[3];  // 0x20
    s32 cellMax[3];  // 0x2c
    s32 link;        // 0x38: negative for a leaf (its low 27 bits index the first table), else the next node to visit
    u32 pad_0x3c;

    UnkPoolBox* Box() { return reinterpret_cast<UnkPoolBox*>(boxStorage); }

    UnkTreeNode_ovb0254(UnkTreeNode_ovb0254&& other) {
        UnkPoolBox* box = Box();
        {
            UnkPoolBox* source = other.Box();
            UnkVec3f min = source->GetMin();
            UnkVec3f max = source->GetMax();
            fn_7100215dcc(box, &min, &max);
        }
        *reinterpret_cast<const char**>(box) = g_unk_7101a20090 + 0x10;
        UnkVec3f min = box->GetMin();
        UnkVec3f max = box->GetMax();
        cellMin[0] = (s32)(min.x * 10.0f);
        cellMin[1] = (s32)(min.y * 10.0f);
        cellMin[2] = (s32)(min.z * 10.0f);
        cellMax[0] = (s32)(max.x * 10.0f);
        cellMax[1] = (s32)(max.y * 10.0f);
        cellMax[2] = (s32)(max.z * 10.0f);
        link = other.link;
    }
    ~UnkTreeNode_ovb0254() { fn_7100215e1c(Box()); }
};
static_assert(sizeof(UnkTreeNode_ovb0254) == 0x40, "node layout");

// One pool entry (0xe0 bytes): its first word is the object it stands for (and the sort key).
struct UnkPoolEntry_ovb0254 {
    u64 field_0x0;
    UnkMatrix44 matrix;  // 0x8
    UnkSphere sphere;    // 0x48
    UnkPoolBox box;      // 0x98
    s32 cellMin[3];      // 0xb8
    s32 cellMax[3];      // 0xc4
    s32 slot;            // 0xd0: index in the slot table, or -1
    s32 kindSlot;        // 0xd4: index in its kind's table, or -1
    s32 kind;            // 0xd8
    u32 field_0xdc;
};
static_assert(sizeof(UnkPoolEntry_ovb0254) == 0xe0, "entry layout");

constexpr u32 kPoolSize = 25000;

struct UnkPool_ovb0254 {
    void* vtable;                                        // 0x0
    UnkPoolEntry_ovb0254* slots[kPoolSize];              // 0x8
    UnkPoolEntry_ovb0254 entries[kPoolSize];             // 0x30d48
    UnkPoolEntry_ovb0254* tableA[kPoolSize];             // 0x588048: the entries of kind 0
    UnkPoolEntry_ovb0254* tableB[kPoolSize];             // 0x5b8d88
    UnkPoolEntry_ovb0254** seen;                         // 0x5e9ac8: buffer of the entries seen
    void* seenObjects[20];                               // 0x5e9ad0
    u32 seenObjectCount;                                 // 0x5e9b70
    u32 slotCount;                                       // 0x5e9b74
    u32 seenCount;                                       // 0x5e9b78
    u32 countA;                                          // 0x5e9b7c
    u32 countB;                                          // 0x5e9b80
    bool disabled;                                       // 0x5e9b84
    bool field_0x5e9b85;
    bool field_0x5e9b86;
    u8 changed;                                          // 0x5e9b87
    base::UnkVector<UnkTreeNode_ovb0254> tree;           // 0x5e9b88
};
static_assert(__builtin_offsetof(UnkPool_ovb0254, entries) == 0x30d48, "pool layout");
static_assert(__builtin_offsetof(UnkPool_ovb0254, tableA) == 0x588048, "pool layout");
static_assert(__builtin_offsetof(UnkPool_ovb0254, tableB) == 0x5b8d88, "pool layout");
static_assert(__builtin_offsetof(UnkPool_ovb0254, seen) == 0x5e9ac8, "pool layout");
static_assert(__builtin_offsetof(UnkPool_ovb0254, slotCount) == 0x5e9b74, "pool layout");
static_assert(__builtin_offsetof(UnkPool_ovb0254, tree) == 0x5e9b88, "pool layout");



// An object an entry stands for: flagged when it wants to be told it was seen, and the frame it was last told.
struct UnkSeenObject_ovb0254 {
    u8 unk_0x0[0x354];
    u32 lastFrame;  // 0x354
    u8 unk_0x358[9];
    u8 wantsSeen;   // 0x361
};

struct UnkFrustum_ovb0254;
bool fn_71001ba188(UnkFrustum_ovb0254* view, UnkSphere* sphere);        // whether the sphere is inside the view
extern u32 g_unk_7101d58e48;  // the current frame

// The entries of both tables whose sphere is in the view: appended to `out1`, and their objects (when they want
// to be told, once a frame) to `out2`. The fourth and sixth parameters are not read.
void fn_71001ec09c(UnkPool_ovb0254* self, UnkFrustum_ovb0254* view, UnkPoolEntry_ovb0254** out1, u32, void** out2, u64,
                   u32* count1, u32* count2) {
    u32 frame = g_unk_7101d58e48;
    u32 n = self->countA;
    UnkPoolEntry_ovb0254** it = self->tableA;
    for (; n != 0; --n, ++it) {
        UnkPoolEntry_ovb0254* entry = *it;
        if (!fn_71001ba188(view, &entry->sphere))
            continue;
        UnkSeenObject_ovb0254* object = reinterpret_cast<UnkSeenObject_ovb0254*>(entry->field_0x0);
        if (object->wantsSeen) {
            u32 old = object->lastFrame;
            object->lastFrame = frame;
            if (old != frame) {
                u32 k = *count2;
                *count2 = k + 1;
                out2[k] = object;
            }
        }
        u32 k = *count1;
        *count1 = k + 1;
        out1[k] = entry;
    }
    n = self->countB;
    it = self->tableB;
    for (; n != 0; --n, ++it) {
        UnkPoolEntry_ovb0254* entry = *it;
        if (!fn_71001ba188(view, &entry->sphere))
            continue;
        UnkSeenObject_ovb0254* object = reinterpret_cast<UnkSeenObject_ovb0254*>(entry->field_0x0);
        if (object->wantsSeen) {
            u32 old = object->lastFrame;
            object->lastFrame = frame;
            if (old != frame && self->seenObjectCount <= 19) {
                u32 k = *count2;
                *count2 = k + 1;
                out2[k] = object;
            }
        }
        u32 k = *count1;
        *count1 = k + 1;
        out1[k] = entry;
    }
}

// Grows the tree's array to `n` nodes (the nodes are copied from their corners).
void fn_71001ec680(base::UnkVector<UnkTreeNode_ovb0254>* self, u32 n) { self->Reserve(n); }


void fn_7100215dac(UnkPoolBox* self);                                  // its default constructor
struct UnkBounded;
void fn_7100215f60(UnkBounded* self, const void* point);                  // grows it to hold a point
UnkVec3f fn_7100215ef4(UnkPoolBox* self);                              // its centre
UnkVec3f fn_7100215e38(UnkPoolBox* self);                              // its size

// A node whose bounds are eight points.
struct UnkBoundsNode {
    u8 unk_0x0[0xa4];
    UnkVec3f points[8];  // 0xa4
};

struct UnkBoundsItem {
    UnkBoundsNode* node;
    u8 unk_0x8[0x18];
};

struct UnkBoundsList {
    u8 unk_0x0[0x34];
    u32 count;
    UnkBoundsItem* items;
};

// Sets the sphere to the one around every point of every node in the list (the first parameter is unused).
void fn_71001eb6b4(void*, UnkBoundsList* list, UnkSphere* sphere) {
    fn_710021693c(sphere, 0.0f);
    u32 count = list->count;
    alignas(8) u8 storage[sizeof(UnkPoolBox)];
    UnkPoolBox* box = reinterpret_cast<UnkPoolBox*>(storage);
    fn_7100215dac(box);
    for (u32 i = 0; i < count; ++i) {
        UnkBoundsNode* node = list->items[i].node;
        for (u32 k = 0; k < 8; ++k)
            fn_7100215f60(reinterpret_cast<UnkBounded*>(box), &node->points[k]);
    }
    {
        UnkVec3f centre = fn_7100215ef4(box);
        fn_7100216978(sphere, &centre);
    }
    {
        UnkVec3f size = fn_7100215e38(box);
        fn_710021693c(sphere, __builtin_sqrtf(size.x * size.x + size.y * size.y + size.z * size.z) * 0.5f);
    }
    fn_7100215e1c(box);
}

// Sorts pointers to keyed entries, largest key first (a quicksort that ends in a bubble sort for five or fewer).
struct UnkKeyed_ovb0254 {
    u64 key;
};

void fn_71001ea860(s32 left, s32 right, UnkKeyed_ovb0254** items) {
    for (;;) {
        if (right - left <= 4) {
            s32 n = right - left;
            for (s32 i = 0; i < n; ++i) {
                for (s32 k = 0; k < n - i; ++k) {
                    if (items[left + k]->key < items[left + k + 1]->key) {
                        UnkKeyed_ovb0254* t = items[left + k];
                        items[left + k] = items[left + k + 1];
                        items[left + k + 1] = t;
                    }
                }
            }
            return;
        }
        if (left >= right)
            return;
        u64 pivot = items[(left + right) / 2]->key;
        s32 j = right;
        s32 i = left;
        do {
            UnkKeyed_ovb0254* x;
            UnkKeyed_ovb0254* y;
            do {
                x = items[i++];
            } while (x->key > pivot);
            do {
                y = items[j--];
            } while (y->key < pivot);
            s32 found_i = i - 1;
            s32 found_j = j + 1;
            if (found_i <= found_j) {
                items[found_i] = y;
                items[found_j] = x;
            } else {
                j = found_j;
                i = found_i;
            }
        } while (i <= j);
        if (j > left)
            fn_71001ea860(left, j, items);
        left = i;
        if (left >= right)
            return;
    }
}
