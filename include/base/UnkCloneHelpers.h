#pragma once

// Templates for repeated functions matched one family at a time (tools/gen_clones.py reference
// kinds): each is instantiated once per identical copy with an UnkClone_fn_<addr> tag. Layouts
// and names are placeholders inferred from the code.

#include <new>

#include "UnkClone.h"
#include "base/UnkCollectionIter.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

void* fn_71000839a0(u64 size, u32 flags);

// A constructor of an object holding a string id at +8 and an array-like block (step 10,
// flags 0xe, allocator) (fn_71004f112c and its identical copies).
namespace base {
template <class Tag>
void refl_StrIdListCtor(char* self) {
    using namespace global;
    self[0] = 0;
    UnkStringData** id = reinterpret_cast<UnkStringData**>(self + 8);
    *id = nullptr;
    if (!g_unk_7101d47108)
        UnkEnsureStringPool();
    *id = &g_unk_7101d47108->empty;
    *reinterpret_cast<u64*>(self + 0x10) = 0;
    *reinterpret_cast<u64*>(self + 0x18) = 0;
    *reinterpret_cast<u64*>(self + 0x20) = 0;
    *reinterpret_cast<u32*>(self + 0x28) = 10;
    *reinterpret_cast<u32*>(self + 0x2c) = 14;
    *reinterpret_cast<UnkAllocator**>(self + 0x30) = fn_7100083990();
}
}

// The destructor of two adjacent slot pools at 0 and +0x80 (fn_71002880d4 and its identical
// copies).
namespace base {
// Three arrays: pointers to slots that are cleared on reset, and two more of unknown elements.
template <class Tag>
struct UnkSlotPool {
    UnkVector<void**> slots;
    UnkVector<UnkCloneWord<Tag>> field_0x28;
    UnkVector<UnkCloneWord<Tag>> field_0x50;

    ~UnkSlotPool() {
        for (void** *p = slots.data, ***end = slots.data + slots.count; p != end; ++p)
            **p = nullptr;
        slots.count = 0;
        field_0x28.count = 0;
        field_0x50.count = 0;
        field_0x50.Free();
        field_0x28.Free();
        slots.Free();
    }
};
static_assert(sizeof(UnkSlotPool<void>) == 0x78);

template <class Tag>
void refl_SlotPoolPairDtor(char* self) {
    reinterpret_cast<UnkSlotPool<Tag>*>(self + 0x80)->~UnkSlotPool();
    reinterpret_cast<UnkSlotPool<Tag>*>(self)->~UnkSlotPool();
}
}

namespace base {
// Compares two arrays of 8-byte elements by count and contents (fn_71000cddd8 and its identical
// copies).
template <class Tag>
bool refl_VectorEqual(const UnkVector<UnkCloneWord<Tag>>* a, const UnkVector<UnkCloneWord<Tag>>* b) {
    if (a->count != b->count)
        return false;
    const UnkCloneWord<Tag>* pa = a->data;
    const UnkCloneWord<Tag>* pb = b->data;
    for (u64 n = a->count; n != 0; --n, ++pa, ++pb)
        if (pb->value != pa->value)
            return false;
    return true;
}
}  // namespace base

// Two parallel arrays (8-byte values, string-id keys) sharing a count, capacity and allocator;
// Reserve(n) reallocates both (fn_710021d58c and its identical copies). The moved-from key is
// cleared through a union so its pool is reloaded, as in the original.
namespace base {
template <class Tag>
struct UnkCloneKeyedArray {
    UnkCloneWord<Tag>* values;
    UnkStrIdMember* keys;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 flags;
    UnkAllocator* allocator;

    void Reserve(u32 n) {
        if (capacity >= n)
            return;
        auto* v = static_cast<UnkCloneWord<Tag>*>(allocator->vf_0x40((u64)n * 8, 8, flags));
        auto* k = static_cast<UnkStrIdMember*>(allocator->vf_0x40((u64)n * 8, 8, flags));
        if (values) {
            __builtin_memcpy(v, values, (u64)count * 8);
            allocator->vf_0x48(values);
        }
        if (keys) {
            UnkStrIdMember* src = keys;
            UnkStrIdMember* dst = k;
            for (u32 i = 0; i < count; ++i, ++src, ++dst) {
                union Slot { global::UnkStringData* p; };
                dst->data = src->data;
                reinterpret_cast<Slot*>(&src->data)->p = &g_unk_7101d47108->empty;
                src->~UnkStrIdMember();
            }
            allocator->vf_0x48(keys);
        }
        values = v;
        keys = k;
        capacity = n;
    }
};
}

// Move-assigns an iterator value keyed by a string id (fn_7100120984 and its identical copies;
// 8-byte elements). The key's stores go through a union so the state is reloaded after them, as
// in the original.
namespace base {
// A string-id key whose move-assignment releases the old id and takes the other's.
template <class Tag>
struct UnkStrIdKey : UnkStrIdMember {
    UnkStrIdKey& operator=(UnkStrIdKey&& other) {
        this->~UnkStrIdKey();
        union Slot { global::UnkStringData* p; };
        reinterpret_cast<Slot*>(&data)->p = other.data;
        reinterpret_cast<Slot*>(&other.data)->p = &g_unk_7101d47108->empty;
        return *this;
    }
};
template <class T, class K>
UnkIterValue<T, K>& refl_IterValueMoveAssign(UnkIterValue<T, K>* self, UnkIterValue<T, K>* other) {
    if (other->state == 1) {
        self->Index() = static_cast<K&&>(other->value.index);
        T v = static_cast<T&&>(other->value.element);
        self->Element() = v;
    } else if (other->state == 2) {
        self->Index() = static_cast<K&&>(*const_cast<K*>(other->ref.index));
        T v = static_cast<T&&>(*other->ref.element);
        self->Element() = v;
    }
    return *self;
}
}

// An element holding an array of owning pointers, freed when destroyed.
template <class Tag>
struct UnkCloneOwnerVec {
    base::UnkVector<UnkCloneOwner<Tag>> v;
    ~UnkCloneOwnerVec() { v.Free(); }
};

// Assigns the handle a refcounted object's source hands out (created on first use with the
// traits' vtable), or null (fn_7100234f78 and its identical copies).
namespace base {
template <class Tag> struct UnkRefSlotTraits;  // vtable(): the vtable of the handles created
// A refcounted pointer; a union member, so the old pointer is reloaded after the increment.
struct UnkRefHandle {
    union {
        UnkRefCounted* ptr;
    };
    void Set(UnkRefCounted* p) {
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            if (UnkRefCounted* old = ptr) {
                if (--old->ref_count == 0)
                    delete old;
            }
            ptr = p;
        }
    }
};
struct UnkRefSource {
    virtual void vf_0x0(); virtual void vf_0x8(); virtual void vf_0x10(); virtual void vf_0x18(); virtual void vf_0x20();
    virtual UnkRefHandle* vf_0x28();
};
template <class Tag>
UnkRefHandle* refl_AssignHandle(UnkRefHandle* self, const UnkRefHandle* other) {
    if (self == other)
        return self;
    UnkRefCounted* h = nullptr;
    if (UnkRefCounted* obj = other->ptr) {
        if (UnkRefSource* src = *reinterpret_cast<UnkRefSource**>(reinterpret_cast<char*>(obj) + 0x10)) {
            if (UnkRefHandle* slot = src->vf_0x28()) {
                h = slot->ptr;
                if (!h) {
                    char* n = static_cast<char*>(fn_71000839a0(0x18, 0x3e));
                    *reinterpret_cast<s32*>(n + 8) = 0;
                    *reinterpret_cast<char**>(n) = UnkRefSlotTraits<Tag>::vtable() + 0x10;
                    *reinterpret_cast<UnkRefSource**>(n + 0x10) = src;
                    h = reinterpret_cast<UnkRefCounted*>(n);
                    slot->Set(h);
                }
            }
        }
    }
    self->Set(h);
    return self;
}
}

// A deferred pointer set with a signed lock count at +0x78 and three arrays (fn_71008415d8 and
// its identical copies). With the count at or below zero, the item is moved to the back of the
// third array; otherwise it is appended to the first and dropped from the second. Names and
// roles are placeholders inferred from the code.
namespace base {

template <class Tag>
struct UnkCountedDeferredSet {
    UnkVector<Tag*> items;
    UnkVector<Tag*> pending_a;
    UnkVector<Tag*> pending_b;
    s32 lock;
};

template <class Tag>
void refl_CountedDeferredAdd(UnkCountedDeferredSet<Tag>* s, Tag* item) {
    if (s->lock <= 0) {
        s->pending_b.Remove(item);
        s->pending_b.PushBack(item);
    } else {
        s->items.PushBack(item);
        s->pending_a.Remove(item);
    }
}

}  // namespace base

// A hash map keyed by string ids whose nodes are also kept in a doubly linked list (fn_7100121358
// and its identical copies). Erase unlinks the node for `key` from its bucket chain and the list,
// releases the key and frees the node, returning the next node in list order. Bucket index =
// (hash words at +0x30/+0x34 of the key's string data, xor'd) modulo the bucket count. Names,
// roles and fields are placeholders inferred from the code.
namespace base {

template <class Tag>
struct UnkStrIdHashNode {
    UnkStrIdMember key;
    u8 unk_0x8[0x8];
    // Union members: the original reloads them after each link store.
    union {
        UnkStrIdHashNode* next;  // list order
    };
    union {
        UnkStrIdHashNode* prev;
    };
    UnkStrIdHashNode* chain;  // bucket chain
};

template <class Tag>
struct UnkStrIdHashMap {
    UnkAllocator* allocator;
    u32 field_0x8;
    u32 count;
    UnkStrIdHashNode<Tag>** buckets;
    u32 field_0x18;
    u32 bucket_count;
    u8 unk_0x20[0x18];
    UnkStrIdHashNode<Tag>* tail;
    UnkStrIdHashNode<Tag>* head;
};

template <class Tag>
UnkStrIdHashNode<Tag>* refl_StrIdHashErase(UnkStrIdHashMap<Tag>* m, const UnkStrIdMember* key) {
    using Node = UnkStrIdHashNode<Tag>;
    global::UnkStringData* k = key->data;
    const char* raw = reinterpret_cast<const char*>(k);
    u32 h = *reinterpret_cast<const u32*>(raw + 0x30) ^ *reinterpret_cast<const u32*>(raw + 0x34);
    u32 i = h % m->bucket_count;
    Node** slot = m->buckets + i;
    Node* n = *slot;
    if (!n)
        return nullptr;
    Node* prev = nullptr;
    if (n->key.data != k) {
        do {
            prev = n;
            n = n->chain;
            if (!n)
                return nullptr;
        } while (n->key.data != k);
    }
    *(prev ? &prev->chain : slot) = n->chain;
    if (m->head == n)
        m->head = n->next;
    if (m->tail == n)
        m->tail = n->prev;
    if (n->prev)
        n->prev->next = n->next;
    if (n->next)
        n->next->prev = n->prev;
    Node* next = n->next;
    n->key.~UnkStrIdMember();
    m->allocator->vf_0x48(n);
    --m->count;
    return next;
}

}  // namespace base
