// Unit 365 ("navmesh::CNavMeshGenerator"; no reliable name): array, list and table helpers of the
// generator's data: edge cursors and polygon segments, node lists with an allocator, growable arrays
// of records and their copies, and the hash of the edge-kind names. Names and layouts are
// placeholders.
#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))
#include <new>

#include "base/UnkEmptyDtorElem.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/CGameObject.h"
#include "game/UnkTU109.h"
#include "game/UnkTU365.h"
#include "game/navmesh/CNavMesh.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;

// ---- fn_7100289128 ----
using base::math::CVector2D;
using base::math::CVector3D;

struct UnkPolygon_b0228 {
    CVector3D* points;  // 0x0
    u32 count;  // 0x8
};

struct UnkEdgeCursor_b0228 {
    UnkPolygon_b0228* first;  // 0x0
    u32 first_index;          // 0x8
    u8 pad_0xc[4];
    UnkPolygon_b0228* second;  // 0x10
    u32 second_index;          // 0x18
    CVector2D first_fallback;  // 0x1c
    CVector2D second_fallback;  // 0x24
};

// Two words copied one by one, as the original does.
struct UnkPoint2_b0228 {
    u32 x_bits, y_bits;
    UnkPoint2_b0228() = default;
    UnkPoint2_b0228(const CVector2D* v) {
        const u32* w = reinterpret_cast<const u32*>(v);
        x_bits = w[0];
        y_bits = w[1];
    }
    operator const CVector2D&() const { return *reinterpret_cast<const CVector2D*>(this); }
};

static inline void EdgeOf(UnkSegment2D* seg, UnkPolygon_b0228* poly, u32 index) {
    CVector3D* start = &poly->points[index];
    CVector3D* next = poly->count - 1 == index ? poly->points : &poly->points[index + 1];
    new (seg) UnkSegment2D(*reinterpret_cast<CVector2D*>(start), *reinterpret_cast<CVector2D*>(next));
}

UnkSegment2D fn_7100289128(UnkEdgeCursor_b0228* self) {
    UnkSegment2D seg;
    const CVector2D* src;
    if (self->first) {
        EdgeOf(&seg, self->first, self->first_index);
        src = &seg.end;
    } else {
        src = &self->first_fallback;
    }
    UnkPoint2_b0228 a(src);
    if (self->second) {
        EdgeOf(&seg, self->second, self->second_index);
        src = &seg.end;
    } else {
        src = &self->second_fallback;
    }
    UnkPoint2_b0228 b(src);
    return UnkSegment2D(a, b);
}

// ---- fn_71002a6488 ----
using base::global::CStrId;

struct UnkEntry_b0228 {
    CStrId name;  // 0x0
    u8 pad_0x8[8];
    base::UnkVector<void*> list0;  // 0x10
    base::UnkVector<void*> list1;  // 0x38

    void Destroy() {
        list1.Free();
        list0.Free();
        name.Reset();
    }
};
static_assert(sizeof(UnkEntry_b0228) == 0x60, "UnkEntry_b0228 layout");

struct UnkEntryArray_b0228 {
    UnkEntry_b0228* data;  // 0x0
    u32 count;             // 0x8
};

void fn_71002a6488(UnkEntryArray_b0228* self, u32 new_count) {
    u64 i = new_count;
    if (self->count > new_count) {
        for (; i < self->count; ++i)
            self->data[i].Destroy();
    }
    self->count = new_count;
}

// ---- fn_7100289554 ----
extern char g_unk_7101a20e98[];
extern char g_unk_7101a20f00[];

struct UnkSubNode_b0228 {
    u8 pad_0x0[0x10];
    UnkSubNode_b0228* next;  // 0x10
};

struct UnkSubList_b0228 {
    void** vtable;                // 0x0
    u32 count;                    // 0x8
    UnkSubNode_b0228* head;       // 0x10
    UnkSubNode_b0228* tail;       // 0x18
    base::UnkAllocator* allocator;  // 0x20

    __attribute__((always_inline)) void Destroy(char* list_vtable) {
        vtable = reinterpret_cast<void**>(list_vtable + 0x10);
        for (UnkSubNode_b0228* n = head; n;) {
            UnkSubNode_b0228* next = n->next;
            allocator->vf_0x48(n);
            n = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

struct UnkPayload_b0228 {
    base::UnkVector<void*> items;  // 0x0
    u8 pad_0x28[0x20];
    UnkSubList_b0228 sub;  // 0x48
};
static_assert(__builtin_offsetof(UnkPayload_b0228, sub) == 0x48, "UnkPayload_b0228 layout");

struct UnkOuterNode_b0228 {
    UnkPayload_b0228* payload;  // 0x0
    UnkOuterNode_b0228* next;   // 0x8
};

struct UnkOuter_b0228 {
    void** vtable;  // 0x0
    u8 pad_0x8[8];
    void** sub_vtable;  // 0x10
    u32 count;          // 0x18
    u8 pad_0x1c[4];
    UnkOuterNode_b0228* head;       // 0x20
    UnkOuterNode_b0228* tail;       // 0x28
    base::UnkAllocator* allocator;  // 0x30
};

struct UnkCameraCtrl_ovb0049;

void fn_7100289554(UnkOuter_b0228* self) {
    self->vtable = reinterpret_cast<void**>(g_unk_7101a20e98 + 0x10);
    for (UnkOuterNode_b0228* n = self->head; n; n = n->next) {
        UnkPayload_b0228* p = n->payload;
        if (!p)
            continue;
        p->sub.Destroy(g_unk_7101a20f00);
        if (p->items.data) {
            p->items.count = 0;
            p->items.allocator->vf_0x48(p->items.data);
        }
        __builtin_operator_delete(p);
    }
    for (UnkOuterNode_b0228* n = self->head; n;) {
        UnkOuterNode_b0228* next = n->next;
        self->allocator->vf_0x48(n);
        n = next;
    }
    self->count = 0;
    self->sub_vtable = reinterpret_cast<void**>(g_unk_7101a20f88 + 0x10);
    self->head = nullptr;
    self->tail = nullptr;
    fn_7101043934(reinterpret_cast<UnkCameraCtrl_ovb0049*>(self));
}

// ---- fn_71002a0680 ----
struct UnkKey_b0228 {
    u32 value;
    ~UnkKey_b0228() { fn_71002874dc(this); }
};

struct UnkKeyList_b0228 {
    u8 pad_0x0[8];
    base::UnkVector<UnkKey_b0228> keys;  // 0x8
    ~UnkKeyList_b0228() { keys.Free(); }
};
static_assert(sizeof(UnkKeyList_b0228) == 0x30, "UnkKeyList_b0228 layout");

struct UnkKeyListOwner_b0228 {
    u8 pad_0x0[8];
    base::UnkVector<void*> field_0x8;
    base::UnkVector<UnkKeyList_b0228> lists;  // 0x30
};

void fn_71002a0680(UnkKeyListOwner_b0228* self) {
    self->lists.Free();
    self->field_0x8.Free();
}

// ---- fn_71002a6124, fn_71002a93e0, fn_71002a5fe8 ----
// A singly linked list of 4- or 8-byte values (count at +8, head and tail at +0x10/+0x18, nodes of
// {value, next} from an allocator at +0x20 with flags at +0x28); base::UnkSList's layout. The count
// is a union member (no type-based alias info), as the lists of the other users of these inserts.
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
};

struct UnkPoint_ovb0229 {
    u32 x;
    u32 y;
    UnkPoint_ovb0229(const UnkPoint_ovb0229& other) : x(other.x), y(other.y) {}
};

using UnkPoints_ovb0229 = UnkSortedList_ovb0229<UnkPoint_ovb0229>;
using UnkKeys_ovb0229 = UnkSortedList_ovb0229<u32>;
using UnkIds_ovb0229 = UnkSortedList_ovb0229<s32>;

// The same lists as the inserts see them: a plain count (stores of the 32-bit values may alias it) and
// nodes of one or two 32-bit words. The insert of the point list is written out below (as a template it
// copies the pair of words with a load/store pair).
struct UnkWordNode_b0228 {
    u32 value;
    UnkWordNode_b0228* next;  // 0x8
};
template <class Node>
struct UnkPlainList_b0228 {
    using Node_t = Node;
    u8 pad_0x0[8];
    u32 count;  // 0x8
    Node* head;  // 0x10
    Node* tail;  // 0x18
    base::UnkAllocator* allocator;  // 0x20
    u32 flags;                      // 0x28
};
static inline void StoreValue(UnkWordNode_b0228* node, const u32* value) { node->value = *value; }
struct UnkPairValue_b0228 {
    u32 first, second;
};

// Inserts a new node holding `value` before `pos` (nothing when `pos` is not in the list).
template <class Node, class V>
__attribute__((always_inline)) static inline void InsertBefore(UnkPlainList_b0228<Node>* list, const V* value, Node* pos) {
    if (!list->head)
        return;
    Node* prev = nullptr;
    Node* cur = list->head;
    while (cur != pos) {
        prev = cur;
        cur = cur->next;
        if (!cur)
            return;
    }
    // Keeps the predecessor test after the search instead of being threaded into it.
    asm("" : "+r"(prev));
    if (prev) {
        u32 n = list->count;
        Node* node = static_cast<Node*>(list->allocator->vf_0x40(16, 8, list->flags));
        if (n == 0) {
            if (!node)
                return;
            StoreValue(node, value);
            node->next = nullptr;
            if (list->count == 0) {
                list->head = node;
                list->tail = node;
                list->count = 1;
                return;
            }
            list->tail->next = node;
            node->next = nullptr;
            goto set_tail;
        }
        if (!node)
            return;
        StoreValue(node, value);
        node->next = nullptr;
        {
            Node* after = prev->next;
            prev->next = node;
            node->next = after;
            if (after)
                goto bump;
        }
    set_tail:
        list->tail = node;
    bump:
        list->count = list->count + 1;
    } else {
        Node* node = static_cast<Node*>(list->allocator->vf_0x40(16, 8, list->flags));
        if (!node)
            return;
        StoreValue(node, value);
        node->next = nullptr;
        if (list->count == 0) {
            list->head = node;
            list->tail = node;
            list->count = 1;
            return;
        }
        node->next = list->head;
        list->head = node;
        // Keeps the count reloaded after the links are stored.
        asm volatile("" ::: "memory");
        list->count = list->count + 1;
    }
}



struct UnkPairNodeK_b0228 {
    u32 first;                 // 0x0
    u32 second;                // 0x4
    UnkPairNodeK_b0228* next;   // 0x8
};

struct UnkPairListK_b0228 {
    u8 pad_0x0[8];
    u32 count;                      // 0x8
    UnkPairNodeK_b0228* head;        // 0x10
    UnkPairNodeK_b0228* tail;        // 0x18
    base::UnkAllocator* allocator;  // 0x20
    u32 flags;                      // 0x28

    UnkPairNodeK_b0228* Alloc() { return static_cast<UnkPairNodeK_b0228*>(allocator->vf_0x40(16, 8, flags)); }
};


void fn_71002a5fe8(UnkPoints_ovb0229* list, const UnkPoint_ovb0229* point, UnkPoints_ovb0229::Node* node_pos) {
    auto* self = reinterpret_cast<UnkPairListK_b0228*>(list);
    auto* value = reinterpret_cast<const UnkPairValue_b0228*>(point);
    auto* pos = reinterpret_cast<UnkPairNodeK_b0228*>(node_pos);
    if (!self->head)
        return;
    UnkPairNodeK_b0228* prev = nullptr;
    UnkPairNodeK_b0228* cur = self->head;
    while (cur != pos) {
        prev = cur;
        cur = cur->next;
        if (!cur)
            return;
    }
    // Keeps the predecessor test after the search instead of being threaded into it.
    asm("" : "+r"(prev));
    if (prev) {
        u32 n = self->count;
        UnkPairNodeK_b0228* node = self->Alloc();
        if (n == 0) {
            if (!node)
                return;
            node->first = value->first;
            node->second = value->second;
            node->next = nullptr;
            if (self->count == 0) {
                self->head = node;
                self->tail = node;
                self->count = 1;
                return;
            }
            self->tail->next = node;
            node->next = nullptr;
            goto set_tail;
        }
        if (!node)
            return;
        node->first = value->first;
            node->second = value->second;
        node->next = nullptr;
        {
            UnkPairNodeK_b0228* after = prev->next;
            prev->next = node;
            node->next = after;
            if (after)
                goto bump;
        }
    set_tail:
        self->tail = node;
    bump:
        self->count = self->count + 1;
    } else {
        UnkPairNodeK_b0228* node = self->Alloc();
        if (!node)
            return;
        node->first = value->first;
            node->second = value->second;
        node->next = nullptr;
        if (self->count == 0) {
            self->head = node;
            self->tail = node;
            self->count = 1;
            return;
        }
        node->next = self->head;
        self->head = node;
        // Keeps the count reloaded after the links are stored.
        asm volatile("" ::: "memory");
        self->count = self->count + 1;
    }
}

void fn_71002a6124(UnkKeys_ovb0229* list, const u32* value, UnkKeys_ovb0229::Node* pos) {
    InsertBefore(reinterpret_cast<UnkPlainList_b0228<UnkWordNode_b0228>*>(list), value,
                 reinterpret_cast<UnkWordNode_b0228*>(pos));
}
void fn_71002a93e0(UnkIds_ovb0229* list, const s32* value, UnkIds_ovb0229::Node* pos) {
    InsertBefore(reinterpret_cast<UnkPlainList_b0228<UnkWordNode_b0228>*>(list), reinterpret_cast<const u32*>(value),
                 reinterpret_cast<UnkWordNode_b0228*>(pos));
}

// ---- fn_71002a6bdc ----
u64 fn_71002a6bdc(u32 kind) {
    switch (kind) {
    case 1:
        return fn_7100001570("water", 5);
    case 2:
        return fn_7100001570("speedbooster", 12);
    case 3:
        return fn_7100001570("speedbooster45up", 16);
    case 4:
        return fn_7100001570("left", 4);
    case 5:
        return fn_7100001570("right", 5);
    case 6:
        return fn_7100001570("patrol", 6);
    case 7:
        return fn_7100001570("search", 6);
    case 8:
        return fn_7100001570("chase", 5);
    case 9:
        return fn_7100001570("chase2", 6);
    case 10:
        return fn_7100001570("chasereachable", 14);
    case 11:
        return fn_7100001570("combat", 6);
    case 12:
        return fn_7100001570("flee", 4);
    case 13:
        return fn_7100001570("brokenshield", 12);
    case 14:
        return fn_7100001570("grab2", 5);
    case 15:
        return fn_7100001570("grabwater", 9);
    case 16:
        return fn_7100001570("protoemmytuto", 13);
    case 17:
        return fn_7100001570("preseta", 7);
    case 18:
        return fn_7100001570("presetb", 7);
    case 19:
        return fn_7100001570("presetc", 7);
    default:
        return ~0ul;
    }
}

// ---- fn_71002a7bfc ----
// std::function's callable holder: slot 0x18 clones it into the given buffer, slot 0x20 destroys it in place.
struct UnkCallable_b0228 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18(UnkCallable_b0228* dst);
    virtual void vf_0x20();
};

struct UnkFunction_b0228 {
    alignas(16) u8 buffer[0x20];
    UnkCallable_b0228* callable;  // 0x20

    UnkCallable_b0228* Local() { return reinterpret_cast<UnkCallable_b0228*>(this); }
};

__attribute__((always_inline)) static inline void Swap(UnkFunction_b0228& self_ref, UnkFunction_b0228& f) {
    UnkFunction_b0228* self = &self_ref;
    UnkFunction_b0228* other = &f;
    if (other == self)
        return;
    if ((void*)self->callable == self->buffer && (void*)other->callable == other->buffer) {
        alignas(16) u8 temp[0x20];
        UnkCallable_b0228* t = reinterpret_cast<UnkCallable_b0228*>(temp);
        self->callable->vf_0x18(t);
        self->callable->vf_0x20();
        self->callable = nullptr;
        other->callable->vf_0x18(self->Local());
        other->callable->vf_0x20();
        other->callable = nullptr;
        self->callable = self->Local();
        t->vf_0x18(other->Local());
        t->vf_0x20();
        other->callable = other->Local();
    } else if ((void*)self->callable == self->buffer) {
        self->callable->vf_0x18(other->Local());
        self->callable->vf_0x20();
        self->callable = other->callable;
        other->callable = other->Local();
    } else if ((void*)other->callable == other->buffer) {
        other->callable->vf_0x18(self->Local());
        other->callable->vf_0x20();
        other->callable = self->callable;
        self->callable = self->Local();
    } else {
        UnkCallable_b0228* tmp = self->callable;
        self->callable = other->callable;
        other->callable = tmp;
    }
}

void fn_71002a7bfc(UnkFunction_b0228* self, UnkFunction_b0228* other) { Swap(*self, *other); }

// ---- fn_71002a1ff8 ----
// A copy of a growable array of 16-bit values: same settings, own storage.
struct UnkU16Array_b0228 {
    u16* data;                      // 0x0
    u32 count;                      // 0x8
    u32 capacity;                   // 0xc
    u32 step;                       // 0x10
    bool field_0x14;                // 0x14
    u32 flags;                      // 0x18
    base::UnkAllocator* allocator;  // 0x20

    UnkU16Array_b0228(const UnkU16Array_b0228& o) {
        data = nullptr;
        count = 0;
        capacity = 0;
        step = o.step;
        field_0x14 = o.field_0x14;
        flags = o.flags;
        allocator = o.allocator;
        u32 cap = o.capacity;
        u16* p = nullptr;
        if (cap) {
            p = static_cast<u16*>(allocator->vf_0x40((u64)cap * 2, 8, flags));
            if (data) {
                __builtin_memcpy(p, data, (u64)count * 2);
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = cap;
        }
        __builtin_memcpy(p, o.data, (u64)o.count * 2);
        count = o.count;
    }
};

// The sub-object copied by fn_710027ebe4.
struct UnkNavMeshEdge;

struct UnkSubObject_b0228 {
    alignas(8) u8 data[0x28];
    UnkSubObject_b0228(const UnkSubObject_b0228& o) {
        fn_710027ebe4(reinterpret_cast<base::UnkVector<UnkNavMeshEdge>*>(this),
                      reinterpret_cast<const base::UnkVector<UnkNavMeshEdge>*>(&o));
    }
};

struct UnkRecord_b0228 {
    u16 id;                  // 0x0
    UnkU16Array_b0228 list;  // 0x8
    UnkSubObject_b0228 sub;  // 0x30
    u64 words[3];            // 0x58

    UnkRecord_b0228(const UnkRecord_b0228& o) : id(o.id), list(o.list), sub(o.sub) {
        __builtin_memcpy(words, o.words, sizeof(words));
    }
};
static_assert(sizeof(UnkRecord_b0228) == 0x70, "UnkRecord_b0228 layout");

struct UnkRecordArray_b0228 {
    UnkRecord_b0228* data;  // 0x0
    u32 count;              // 0x8
    u32 capacity;           // 0xc
    u32 step;               // 0x10
};

struct UnkNavLoadCell;

void fn_71002a1ff8(UnkRecordArray_b0228* self, const UnkRecord_b0228* value) {
    u32 n = self->count;
    u32 cap = self->capacity;
    if (cap <= n && self->step != 0)
        cap = (n / self->step + 1) * self->step;
    if ((s32)n >= (s32)cap)
        for (;;) {
        }
    fn_7100279600(reinterpret_cast<base::UnkVector<UnkNavLoadCell>*>(self), cap);
    new (&self->data[self->count]) UnkRecord_b0228(*value);
    self->count = self->count + 1;
}

// ---- fn_71002a9df0 ----
// A 16-byte element copied word by word.
struct UnkPair16_b0228 {
    u64 first;
    u64 second;
    UnkPair16_b0228(const UnkPair16_b0228& o) { __builtin_memcpy(this, &o, sizeof(*this)); }
};

using UnkPair16Vector_b0228 = base::UnkVector<UnkPair16_b0228>;

// The array before its elements exist: raw storage the copy fills in.
struct UnkPair16Storage_b0228 {
    alignas(8) u8 raw[sizeof(UnkPair16Vector_b0228)];
};

void fn_71002a9df0(UnkPair16Storage_b0228* self, const UnkPair16Storage_b0228* other) {
    auto* dst = reinterpret_cast<UnkPair16Vector_b0228*>(self);
    auto* src = reinterpret_cast<const UnkPair16Vector_b0228*>(other);
    dst->data = nullptr;
    dst->count = 0;
    dst->capacity = 0;
    dst->step = src->step;
    dst->field_0x14 = src->field_0x14;
    dst->flags = src->flags;
    dst->allocator = src->allocator;
    dst->Reserve(src->capacity);
    for (const UnkPair16_b0228 *it = src->data, *end = src->data + src->count; it != end; ++it)
        dst->PushBackCopy(*it);
}

// ---- fn_71002a9558 ----

// An object built and destroyed out of line (fn_7100123eb4, fn_7100123fb0).
struct UnkTileOutlineList;

struct UnkPart_b0228 {
    u8 data[0x40];
    UnkPart_b0228() { fn_7100123eb4(reinterpret_cast<UnkTileOutlineList*>(this)); }
    ~UnkPart_b0228() { fn_7100123fb0(reinterpret_cast<UnkTileOutlineList*>(this)); }
};

struct UnkNodeP_b0228 {
    u8 pad_0x0[0x18];
    UnkNodeP_b0228* next;  // 0x18
};
struct UnkNodeQ_b0228 {
    u8 pad_0x0[0x30];
    UnkNodeQ_b0228* next;  // 0x30
};
struct UnkNodeR_b0228 {
    u8 pad_0x0[8];
    UnkNodeR_b0228* next;  // 0x8
};

struct UnkListP_b0228 {
    void** vtable;
    u32 count = 0;
    UnkNodeP_b0228* head = nullptr;
    UnkNodeP_b0228* tail = nullptr;
    u32 flags = 0x10;
    base::UnkAllocator* allocator;
    UnkListP_b0228() : vtable(reinterpret_cast<void**>(g_unk_7101a20f48 + 0x10)), allocator(fn_7100083990()) {}
    ~UnkListP_b0228() {
        vtable = reinterpret_cast<void**>(g_unk_7101a20f48 + 0x10);
        for (UnkNodeP_b0228* n = head; n;) {
            UnkNodeP_b0228* next = n->next;
            allocator->vf_0x48(n);
            n = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

struct UnkListQ_b0228 {
    void** vtable;
    u32 count = 0;
    UnkNodeQ_b0228* head = nullptr;
    UnkNodeQ_b0228* tail = nullptr;
    base::UnkAllocator* allocator;
    u32 flags = 0x13;
    UnkListQ_b0228() : vtable(reinterpret_cast<void**>(g_unk_7101a20f68 + 0x10)), allocator(fn_7100083990()) {}
    ~UnkListQ_b0228() {
        vtable = reinterpret_cast<void**>(g_unk_7101a20f68 + 0x10);
        for (UnkNodeQ_b0228* n = head; n;) {
            UnkNodeQ_b0228* next = n->next;
            allocator->vf_0x48(n);
            n = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

struct UnkListR_b0228 {
    void** vtable;
    u32 count = 0;
    UnkNodeR_b0228* head = nullptr;
    UnkNodeR_b0228* tail = nullptr;
    base::UnkAllocator* allocator;
    u32 flags = 0x13;
    UnkListR_b0228() : vtable(reinterpret_cast<void**>(g_unk_7101a20f88 + 0x10)), allocator(fn_7100083990()) {}
    ~UnkListR_b0228() {
        vtable = reinterpret_cast<void**>(g_unk_7101a20f88 + 0x10);
        for (UnkNodeR_b0228* n = head; n;) {
            UnkNodeR_b0228* next = n->next;
            allocator->vf_0x48(n);
            n = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

// A record of the array: moved by the out-of-line constructor fn_71002a9710 (bound by name, as
// base/math/UnkSegment2D.h binds its constructor), destroyed member by member.
struct UnkBigSlot_b0228 {
    UnkPart_b0228 part0;
    u64 field_0x40 = 0;
    UnkListP_b0228 list_p;  // 0x48
    UnkListQ_b0228 list_q;  // 0x78
    UnkPart_b0228 part1;    // 0xa8
    UnkListR_b0228 list_r;  // 0xe8
    UnkPart_b0228 part2;    // 0x118

    UnkBigSlot_b0228() = default;
    UnkBigSlot_b0228(UnkBigSlot_b0228&&) asm("fn_71002a9710");
};
static_assert(sizeof(UnkBigSlot_b0228) == 0x158, "UnkBigSlot_b0228 layout");

void fn_71002a9558(base::UnkVector<UnkBigSlot_b0228>* self, u32 capacity) { self->Reserve(capacity); }

// ---- fn_71002a0a00 ----
// A 4-byte key built by an out-of-line constructor (fn_71002874bc) and destroyed by fn_71002874dc.
void fn_71002874bc(void* self);

struct UnkKeyCtor_b0228 {
    u32 value;
    UnkKeyCtor_b0228() { fn_71002874bc(this); }
    UnkKeyCtor_b0228(UnkKeyCtor_b0228&& o) { value = o.value; }
    ~UnkKeyCtor_b0228() { fn_71002874dc(this); }
};

UnkKeyCtor_b0228* fn_71002a0a00(base::UnkVector<UnkKeyCtor_b0228>* self, u32 index) { return &self->GrowAt(index); }

// ---- fn_71002aa0a4 ----
using base::global::CStrId;

// An element copied by the out-of-line constructor fn_71002aa29c (bound by name).
struct UnkLeaf_b0228 {
    u8 data[0x28];
    UnkLeaf_b0228(const UnkLeaf_b0228&) asm("fn_71002aa29c");
};

struct UnkInner_b0228;
template <class V>
struct UnkIdMap;
struct UnkOwnedList;
void fn_710027fe8c(UnkIdMap<UnkOwnedList>* self, u32 capacity);

// A table of leaves with a name per leaf.
struct UnkInner_b0228 {
    UnkLeaf_b0228* leaves;  // 0x0
    CStrId* ids;            // 0x8
    u32 count;              // 0x10
    u32 capacity;           // 0x14
    u32 field_0x18;
    u32 field_0x1c;
    u64 field_0x20;

    __attribute__((always_inline)) UnkInner_b0228(const UnkInner_b0228& o) {
        leaves = nullptr;
        ids = nullptr;
        count = 0;
        capacity = 0;
        field_0x18 = o.field_0x18;
        field_0x1c = o.field_0x1c;
        field_0x20 = o.field_0x20;
        fn_710027fe8c(reinterpret_cast<UnkIdMap<UnkOwnedList>*>(this), o.count);
        count = o.count;
        for (u64 i = 0; i < count; i++)
            new (&leaves[i]) UnkLeaf_b0228(o.leaves[i]);
        count = o.count;
        for (u64 i = 0; i < count; i++) {
            base::global::UnkStringData* d = o.ids[i].data;
            __builtin_memcpy(&ids[i].data, &d, sizeof(d));
            if (&g_unk_7101d47108->empty != d)
                ++d->ref_count;
        }
    }
};
static_assert(sizeof(UnkInner_b0228) == 0x28, "UnkInner_b0228 layout");

struct UnkOuterTable_b0228 {
    UnkInner_b0228* inners;  // 0x0
    CStrId* ids;             // 0x8
    u32 count;               // 0x10
    u32 capacity;            // 0x14
    u32 field_0x18;
    u32 field_0x1c;
    u64 field_0x20;
};

struct UnkNavLoadRecordMaps;
void fn_710027fc88(UnkNavLoadRecordMaps* self, u32 capacity);

void fn_71002aa0a4(UnkOuterTable_b0228* self, const UnkOuterTable_b0228* other) {
    self->inners = nullptr;
    self->ids = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->field_0x18 = other->field_0x18;
    self->field_0x1c = other->field_0x1c;
    self->field_0x20 = other->field_0x20;
    fn_710027fc88(reinterpret_cast<UnkNavLoadRecordMaps*>(self), other->count);
    self->count = other->count;
    for (u64 i = 0; i < self->count; i++)
        new (&self->inners[i]) UnkInner_b0228(other->inners[i]);
    self->count = other->count;
    for (u64 i = 0; i < self->count; i++) {
        base::global::UnkStringData* d = other->ids[i].data;
        __builtin_memcpy(&self->ids[i].data, &d, sizeof(d));
        if (&g_unk_7101d47108->empty != d)
            ++d->ref_count;
    }
}
