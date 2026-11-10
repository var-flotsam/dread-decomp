// game::logic::collision::CCollider unit (tools/tu_map.py): the deleting form of the destructor
// fn_7100d3b4c8 (src/game/logic/collision/CCollider/CCollider_Dtor.cpp), a hash map clear whose
// nodes hold a string id and a refcounted value, and the copy-assignment of an array of pointer
// arrays (elements appended by fn_7100d43e70). Names and layouts are placeholders.
#include <new>
#include <string.h>

#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"

struct UnkColliderDtor;

struct UnkColliderListNode_ovb0034 {
    void* value;
    UnkColliderListNode_ovb0034* next;
};

struct UnkColliderReleasable_ovb0034 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
};

struct UnkCollider_ovb0034 {
    void** vtable;
    u8 pad_0x8[0x128 - 0x8];
    void** list_vtable;                       // 0x128
    u32 count;                                // 0x130
    UnkColliderListNode_ovb0034* head;        // 0x138
    UnkColliderListNode_ovb0034* tail;        // 0x140
    base::UnkAllocator* allocator;            // 0x148
    u8 pad_0x150[0x8];
    UnkColliderReleasable_ovb0034* field_0x158;
    bool field_0x160;
    u32 field_0x164;

    void vf_0x38() { reinterpret_cast<void (*)(UnkCollider_ovb0034*)>(vtable[7])(this); }
};
static_assert(__builtin_offsetof(UnkCollider_ovb0034, field_0x164) == 0x164, "UnkCollider_ovb0034 layout");

extern char g_unk_7101aa85b0[];
extern char g_unk_7101aa8660[];
void fn_7100d35a74(UnkColliderDtor* self);
void fn_7100d35944(UnkColliderDtor* self);

void fn_7100d3b58c(UnkCollider_ovb0034* self) {
    self->vtable = reinterpret_cast<void**>(g_unk_7101aa85b0 + 0x10);
    if (self->field_0x160) {
        if (self->field_0x158) {
            self->field_0x158->vf_0x10();
            self->field_0x158 = nullptr;
            self->field_0x160 = false;
            self->vf_0x38();
        }
    } else if (self->field_0x158) {
        self->field_0x158 = nullptr;
        fn_7100d35a74(reinterpret_cast<UnkColliderDtor*>(self));
    }
    self->field_0x164 = 0;
    self->list_vtable = reinterpret_cast<void**>(g_unk_7101aa8660 + 0x10);
    for (UnkColliderListNode_ovb0034* n = self->head; n;) {
        UnkColliderListNode_ovb0034* next = n->next;
        self->allocator->vf_0x48(n);
        n = next;
    }
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
    fn_7100d35944(reinterpret_cast<UnkColliderDtor*>(self));
    fn_7100083c00(self);
}

// The map's layout is base/UnkHashMap.h's: allocator, entry count (+0xc), bucket array (+0x10,
// its capacity at +0x1c) and the list of all nodes (+0x38/+0x40, walked from +0x40).
struct UnkColliderMapNode_ovb0034 {
    base::UnkStrIdMember key;
    base::UnkRefPtrMember value;
    UnkColliderMapNode_ovb0034* next;
};

struct UnkColliderMap_ovb0034 {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    UnkColliderMapNode_ovb0034** buckets;
    u32 bucket_count;
    u32 bucket_capacity;
    u8 pad_0x20[0x18];
    UnkColliderMapNode_ovb0034* first;
    UnkColliderMapNode_ovb0034* last;
};

void fn_7100d418c8(UnkColliderMap_ovb0034* self) {
    if (!self->count)
        return;
    for (UnkColliderMapNode_ovb0034* node = self->last; node;) {
        UnkColliderMapNode_ovb0034* next = node->next;
        node->value.~UnkRefPtrMember();
        node->key.~UnkStrIdMemberOf();
        self->allocator->vf_0x48(node);
        node = next;
    }
    memset(self->buckets, 0, self->bucket_capacity * u64(8));
    self->count = 0;
    self->first = nullptr;
    self->last = nullptr;
}

// An array of pointers that owns its storage (0x28 bytes, laid out as base::UnkVector).
struct UnkPtrArray_ovb0034 {
    void** data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkPtrArray_ovb0034(UnkPtrArray_ovb0034&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
    ~UnkPtrArray_ovb0034() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};
static_assert(sizeof(UnkPtrArray_ovb0034) == 0x28, "UnkPtrArray_ovb0034 layout");

struct UnkPtrArrays_ovb0034 {
    UnkPtrArray_ovb0034* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_7100d43e70(UnkPtrArrays_ovb0034* self, const UnkPtrArray_ovb0034* value);

// Destroys the elements, grows the storage to the other array's capacity (moving the elements
// over), then appends a copy of each of the other's elements.
UnkPtrArrays_ovb0034* fn_7100d44054(UnkPtrArrays_ovb0034* self, const UnkPtrArrays_ovb0034* other) {
    if (self == other)
        return self;
    for (u32 i = 0; i < self->count; ++i)
        self->data[i].~UnkPtrArray_ovb0034();
    self->count = 0;
    u32 n = other->capacity;
    if (self->capacity < n) {
        auto* p = static_cast<UnkPtrArray_ovb0034*>(
            self->allocator->vf_0x40((u64)n * sizeof(UnkPtrArray_ovb0034), 8, self->flags));
        if (self->data) {
            UnkPtrArray_ovb0034* src = self->data;
            UnkPtrArray_ovb0034* dst = p;
            for (u32 i = 0; i < self->count; ++i, ++src, ++dst)
                new (dst) UnkPtrArray_ovb0034(static_cast<UnkPtrArray_ovb0034&&>(*src));
            self->allocator->vf_0x48(self->data);
        }
        self->data = p;
        self->capacity = n;
    }
    for (const UnkPtrArray_ovb0034 *it = other->data, *end = it + other->count; it != end; ++it)
        fn_7100d43e70(self, it);
    return self;
}
