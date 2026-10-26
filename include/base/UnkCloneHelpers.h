#pragma once

// Templates for repeated functions matched one family at a time (tools/gen_clones.py reference
// kinds): each is instantiated once per identical copy with an UnkClone_fn_<addr> tag. Layouts
// and names are placeholders inferred from the code.

#include <new>

#include "UnkClone.h"
#include "base/UnkAtExit.h"
#include "base/UnkBlockFreeList.h"
#include "base/UnkCollectionIter.h"
#include "base/UnkCountedResource.h"
#include "base/UnkDeferredSet.h"
#include "base/UnkDestructors.h"
#include "base/UnkHashStream.h"
#include "base/UnkMatrix44.h"
#include "base/UnkRefHandle.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "types.h"

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
// The slot, UnkRefHandle, is in base/UnkRefHandle.h.
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

// A record with a u32 at +8, a string id (Id, from UnkClone.h) at +0x10 and a T at +0x18, copy-
// assigned past its first word (fn_7100b96394 and its copies).
template <class Tag, class Id, class T>
struct UnkCloneIdRecord {
    void* field_0x0;
    u32 field_0x8;
    Id id;
    T field_0x18;

    UnkCloneIdRecord& operator=(const UnkCloneIdRecord& o) {
        if (this != &o) {
            field_0x8 = o.field_0x8;
            id = o.id;
            field_0x18 = o.field_0x18;
        }
        return *this;
    }
};

namespace base {

template <class Tag>
struct UnkCtorTraits;  // vtable(), base(char*): the class's vtable and base constructor

// A constructor that calls its base, stores its vtable and default-constructs a string id at
// `Off` (fn_7100025398 and its copies).
template <class Tag, u32 Off>
void refl_StrIdMemberCtor(char* self) {
    UnkCtorTraits<Tag>::base(self);
    auto* id = reinterpret_cast<global::UnkStringData**>(self + Off);
    *id = nullptr;
    *reinterpret_cast<char**>(self) = UnkCtorTraits<Tag>::vtable() + 0x10;
    if (!g_unk_7101d47108)
        global::UnkEnsureStringPool();
    *id = &g_unk_7101d47108->empty;
}

}  // namespace base

// Destroys an array's elements with a 64-bit index, keeping the array (fn_71002b9890 and its
// identical copies; owning-pointer elements).
namespace base {
template <class T>
void refl_ClearWide(UnkVector<T>* v) {
    for (u64 i = 0; i < v->count; ++i)
        v->data[i].~T();
    v->count = 0;
}
}  // namespace base

// Destroys a collection iterator (its cached pair) (fn_7100f4adf8 and its identical copies).
namespace base {
template <class T, class K>
void refl_IterDestroy(UnkCollectionIter<T, K>* it) {
    it->~UnkCollectionIter();
}
}  // namespace base

// A record with a u32, a string id and a byte, copy-assigned past its first word
// (fn_7100bbc1d0 and its identical copies).
namespace base {
template <class Tag>
struct UnkStrIdRecord {
    u64 field_0x0;
    u32 field_0x8;
    UnkStrIdMember id;
    u8 field_0x18;

    UnkStrIdRecord& operator=(const UnkStrIdRecord& other) {
        if (this != &other) {
            field_0x8 = other.field_0x8;
            id = other.id;
            field_0x18 = other.field_0x18;
        }
        return *this;
    }
};
}  // namespace base

// An iterator over a keyed map's nodes ({key, value}) with its cached pair; Sync re-points the
// pair at the current node or resets it (fn_710011c448 and its identical copies).
namespace base {
template <class T, class K>
struct UnkMapIter {
    struct Node {
        K key;
        T value;
    };
    Node* node;
    UnkIterValue<T, K> current;
};

template <class T, class K>
void refl_MapIterSync(UnkMapIter<T, K>* it) {
    if (it->node)
        it->current.SetRef(&it->node->key, &it->node->value);
    else
        it->current.Reset();
}
}  // namespace base

// A list of type-erased callables with a refcounted owner, behind two vtables; held by
// collection iterators (refl_IterSync, fn_710090b1c8 and its identical copies).
namespace base {
// vtable(): the list's vtable; base_vtable(): its base's.
template <class Tag>
struct UnkCallbackTraits;
// A type-erased callable: storage and an operations table whose +0x40 entry destroys it.
struct UnkCallable {
    struct Ops {
        void* field_0x0[8];
        void (*destroy)(UnkCallable* self);
    };
    u64 storage;
    const Ops* ops;
    ~UnkCallable() {
        if (ops) {
            ops->destroy(this);
            ops = nullptr;
        }
    }
};
template <class Tag>
struct UnkCallbackListBase {
    char* vtable;
    UnkVector<UnkCallable> callables;
    ~UnkCallbackListBase() {
        vtable = UnkCallbackTraits<Tag>::base_vtable() + 0x10;
        if (callables.data) {
            for (u64 i = 0; i < callables.count; ++i)
                callables.data[i].~UnkCallable();
            callables.count = 0;
            callables.allocator->vf_0x48(callables.data);
            callables.capacity = 0;
        }
    }
};
template <class Tag>
struct UnkCallbackList : UnkCallbackListBase<Tag> {
    u64 field_0x30;
    UnkRefPtrMember owner;
    u64 field_0x40;
    ~UnkCallbackList() { this->vtable = UnkCallbackTraits<Tag>::vtable() + 0x10; }
};

// Its deleting form (fn_710023af18 and its copies).
template <class Tag>
void refl_CallbackListDeletingDtor(UnkCallbackList<Tag>* self) {
    self->~UnkCallbackList();
    __builtin_operator_delete(self);
}
}  // namespace base

// A record of an 8-byte field, a string id and a u32, copy-assigned past its first field
// (fn_71002e4fe8 and its identical copies).
namespace base {
template <class Tag>
struct UnkStrIdRecordB {
    u64 field_0x0;
    global::CStrId id;
    u32 field_0x10;

    UnkStrIdRecordB& operator=(const UnkStrIdRecordB& other) {
        if (this != &other) {
            id = other.id;
            field_0x10 = other.field_0x10;
        }
        return *this;
    }
};
}  // namespace base

// A polymorphic record of plain fields, copy-assigned field by field past its vtable; the u32 at
// +0x34 is not copied (fn_710098e188 and its identical copies).
namespace base {
template <class Tag>
struct UnkPlainRecord {
    virtual ~UnkPlainRecord();
    u64 field_0x8;
    struct Block28 {
        u32 words[7];
    } field_0x10;
    u32 field_0x2c;
    u32 field_0x30;
    u32 field_0x34;
    u32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;
    u32 field_0x44;

    UnkPlainRecord& operator=(const UnkPlainRecord& other) {
        if (this != &other) {
            field_0x8 = other.field_0x8;
            field_0x10 = other.field_0x10;
            field_0x2c = other.field_0x2c;
            field_0x30 = other.field_0x30;
            field_0x38 = other.field_0x38;
            field_0x3c = other.field_0x3c;
            field_0x40 = other.field_0x40;
            field_0x44 = other.field_0x44;
        }
        return *this;
    }
};
}  // namespace base

// Copy-assigns an iterator value keyed by a string id, reading the source element before
// locating the destination (fn_7100120848 and its identical copies; 8-byte elements).
namespace base {
template <class T, class K>
UnkIterValue<T, K>& refl_IterValueCopyAssign(UnkIterValue<T, K>* self, const UnkIterValue<T, K>* other) {
    if (self != other) {
        self->Index() = other->Index();
        T v = other->Element();
        self->Element() = v;
    }
    return *self;
}
}  // namespace base

// Inserts a node with a copy of `key` and an 8-byte value at the head of its bucket chain and
// the tail of the node list of an UnkStrIdHashMap (fn_7100120f0c and its identical copies). The
// node is built with placement new's null check; the links are stored after it either way.
namespace base {
template <class Tag>
void refl_StrIdHashInsert(UnkStrIdHashMap<Tag>* m, const global::CStrId* key, const u64* value) {
    using Node = UnkStrIdHashNode<Tag>;
    Node** slot = nullptr;
    if (m->field_0x18) {
        const char* raw = reinterpret_cast<const char*>(key->data);
        u32 h = *reinterpret_cast<const u32*>(raw + 0x30) ^ *reinterpret_cast<const u32*>(raw + 0x34);
        u32 i = h % m->bucket_count;
        slot = m->buckets + i;
    }
    Node* chain = *slot;
    Node* n = static_cast<Node*>(m->allocator->vf_0x40(0x28, 8, m->field_0x8));
    if (n) {
        new (&n->key) global::CStrId(*key);
        *reinterpret_cast<u64*>(n->unk_0x8) = *value;
        n->next = nullptr;
        n->prev = nullptr;
        n->chain = nullptr;
    }
    n->chain = chain;
    *slot = n;
    if (!m->head) {
        m->head = n;
    } else if (m->tail) {
        n->prev = m->tail;
        m->tail->next = n;
    }
    m->tail = n;
    ++m->count;
}
}  // namespace base

// A refcounted pointer as a collection element (fn_7100237158: UnkCollectionIter's
// copy-assignment over it, and its identical copies).
template <class Tag>
struct UnkCloneRefPtr : base::UnkRefPtrMember {};

// An array as an element of another array, moved field by field (fn_710065b1b0).
namespace base {
template <class Tag>
struct UnkCloneSubVec {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    UnkAllocator* allocator;
};

// Resizes an array of arrays to `n`: reallocates when the capacity is short (moving the
// elements and clearing the moved-from ones with memset, so the count is reloaded each step),
// then default-constructs the new elements (flags 0xf) (fn_710065b1b0 and its identical copies).
template <class Tag>
void refl_ResizeSubVecs(UnkVector<UnkCloneSubVec<Tag>>* v, u32 n) {
    using E = UnkCloneSubVec<Tag>;
    if (v->capacity < n) {
        E* d = static_cast<E*>(v->allocator->vf_0x40((u64)n * sizeof(E), 8, v->flags));
        if (E* s = v->data) {
            E* o = d;
            for (u32 i = 0; i < v->count; ++i, ++s, ++o) {
                o->data = s->data;
                o->count = s->count;
                o->capacity = s->capacity;
                o->step = s->step;
                o->field_0x14 = s->field_0x14;
                o->flags = s->flags;
                o->allocator = s->allocator;
                __builtin_memset(s, 0, 16);
            }
            v->allocator->vf_0x48(v->data);
        }
        v->data = d;
        v->capacity = n;
    }
    for (u32 i = v->count; i < n; ++i) {
        E* e = v->data + i;
        e->step = 10;
        e->data = nullptr;
        e->count = 0;
        e->capacity = 0;
        e->field_0x14 = true;
        e->flags = 0xf;
        e->allocator = fn_7100083990();
    }
    v->count = n;
}
}  // namespace base

// An array of string ids with the data pointer at +8 and the count at +0x10; its move-assignment
// is fn_71000df128 and its identical copies.
namespace base {
template <class Tag>
struct UnkStrIdArray {
    u64 field_0x0;
    UnkStrIdMember* data;
    u32 count;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    u64 field_0x20;

    UnkStrIdArray& operator=(UnkStrIdArray&& other) {
        if (this != &other) {
            if (data) {
                for (u32 i = 0; i < count; ++i)
                    data[i].~UnkStrIdMember();
            }
            count = 0;
            field_0x20 = other.field_0x20;
            data = other.data;
            field_0x0 = other.field_0x0;
            field_0x14 = other.field_0x14;
            count = other.count;
            field_0x1c = other.field_0x1c;
            field_0x18 = other.field_0x18;
            other.count = 0;
            other.field_0x0 = 0;
            other.data = nullptr;
        }
        return *this;
    }
};
}  // namespace base

// A 0x28-byte array header moved field by field; UnkVector::EmplaceBack over it is fn_71004a940c
// and its identical copies.
template <class Tag>
struct UnkCloneVecHandle {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkCloneVecHandle(UnkCloneVecHandle&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
};

// An iterator over keyed nodes (key at +0, value at +8) whose current pair refers into the node;
// refl_KeyedIterSync re-points or resets it (fn_710023c550 and its identical copies; values are
// arrays of 0x40-byte polymorphic elements).
template <class Tag>
struct UnkClonePoly64 {
    virtual ~UnkClonePoly64();
    u64 field_0x8[7];
};
template <class Tag>
struct UnkClonePolyVec {
    base::UnkVector<UnkClonePoly64<Tag>> v;
    ~UnkClonePolyVec() { v.Free(); }
};
// The same followed by 8-byte words up to `Size` (iterator values keyed by a string id at the
// end of a larger key, fn_710075abfc and its copies).
template <class Tag, u64 Size>
struct UnkClonePolyVecPad {
    base::UnkVector<UnkClonePoly64<Tag>> v;
    u64 pad[(Size - 0x28) / 8];
    ~UnkClonePolyVecPad() { v.Free(); }
};
namespace base {
template <class T, class K>
struct UnkKeyedNode {
    K key;
    T value;
};
template <class T, class K>
struct UnkKeyedIter {
    UnkKeyedNode<T, K>* node;
    UnkIterValue<T, K> current;
};
template <class T, class K>
void refl_KeyedIterSync(UnkKeyedIter<T, K>* it) {
    if (it->node)
        it->current.SetRef(&it->node->key, &it->node->value);
    else
        it->current.Reset();
}
}  // namespace base

// Iterator values keyed by a string id with 8-byte elements (fn_71000c9490, fn_71000c93ec and
// their identical copies).
namespace base {

template <class Tag>
using UnkStrKeyWordValue = UnkIterValue<UnkCloneWord<Tag>, UnkCloneStrId<Tag>>;

// Resets this value, then copies the other's state and its pair or reference bitwise
// (fn_71000c9490).
template <class Tag>
UnkStrKeyWordValue<Tag>* refl_IterValueAssignRaw(UnkStrKeyWordValue<Tag>* self, const UnkStrKeyWordValue<Tag>* other) {
    if (self != other) {
        self->Reset();
        self->state = other->state;
        struct Raw {
            u64 a, b;
        };
        *reinterpret_cast<Raw*>(&self->ref) = *reinterpret_cast<const Raw*>(&other->ref);
    }
    return self;
}

// Makes this value refer to the (index, element) a reference holds (fn_71000c93ec).
template <class Tag>
UnkStrKeyWordValue<Tag>* refl_IterValueAssignRef(UnkStrKeyWordValue<Tag>* self,
                                                 const typename UnkStrKeyWordValue<Tag>::Ref* r) {
    if (r != static_cast<const void*>(self))
        self->SetRef(r->index, r->element);
    return self;
}

}  // namespace base

// Copy-assigns the pair an iterator value refers to, copying the element out first
// (fn_71000d3038 and its identical copies; 8-byte elements).
namespace base {

template <class T, class K>
void refl_IterValueAssign(UnkIterValue<T, K>* self, const UnkIterValue<T, K>* other) {
    if (self != other) {
        self->Index() = other->Index();
        T v = other->Element();
        self->Element() = v;
    }
}

}  // namespace base

// Assigns the handle the source at +8 of `holder` hands out, created on first use with the
// traits' vtable, or null (fn_7100234ccc and its identical copies; see refl_AssignHandle).
namespace base {

template <class Tag>
void refl_AssignHandleFromSource(UnkRefHandle* self, const char* holder) {
    UnkRefCounted* h = nullptr;
    if (UnkRefSource* src = *reinterpret_cast<UnkRefSource* const*>(holder + 8)) {
        if (UnkRefHandle* slot = src->vf_0x28()) {
            h = slot->ptr;
            if (!h) {
                char* n = static_cast<char*>(fn_71000839a0(0x18, 0x3e));
                *reinterpret_cast<char**>(n) = UnkRefSlotTraits<Tag>::vtable() + 0x10;
                *reinterpret_cast<s32*>(n + 8) = 0;
                *reinterpret_cast<UnkRefSource**>(n + 0x10) = src;
                h = reinterpret_cast<UnkRefCounted*>(n);
                slot->Set(h);
            }
        }
    }
    self->Set(h);
}

}  // namespace base

// Move-assigns the pair an iterator value owns or refers to, without a self check
// (fn_71004a8728 and its identical copies; elements are arrays of owning pointers).
namespace base {

template <class T, class K>
UnkIterValue<T, K>& refl_IterValueMoveFrom(UnkIterValue<T, K>* self, UnkIterValue<T, K>* other) {
    if (other->state == 1) {
        self->Index() = other->value.index;
        self->Element() = static_cast<T&&>(other->value.element);
    } else if (other->state == 2) {
        self->Index() = *other->ref.index;
        self->Element() = static_cast<T&&>(*other->ref.element);
    }
    return *self;
}

}  // namespace base

// A record with a u32, a string id and a u16, copy-assigned past its first word (fn_7100ba33cc
// and its identical copies).
namespace base {

template <class Tag>
struct UnkStrIdU16Record {
    void* field_0x0;
    u32 field_0x8;
    UnkStrIdMember id;
    u16 field_0x18;

    UnkStrIdU16Record& operator=(const UnkStrIdU16Record& other) {
        if (this != &other) {
            field_0x8 = other.field_0x8;
            id = other.id;
            field_0x18 = other.field_0x18;
        }
        return *this;
    }
};

}  // namespace base

// A copy constructor of a class derived from a polymorphic base of two words: the base's vtable,
// the base's copy, the class's vtable and the copy of the T at 0x18. Both copies are bitwise, so
// the vtable stores stay (fn_7100bf86b4 and its copies).
namespace base {
template <class Tag>
struct UnkCopyCtorTraits;  // base_vtable(), vtable()

template <class Tag, class T>
void refl_DerivedCopyCtor(char* self, const char* other) {
    *reinterpret_cast<char**>(self) = UnkCopyCtorTraits<Tag>::base_vtable() + 0x10;
    __builtin_memcpy(self + 8, other + 8, 16);
    *reinterpret_cast<char**>(self) = UnkCopyCtorTraits<Tag>::vtable() + 0x10;
    __builtin_memcpy(self + 0x18, other + 0x18, sizeof(T));
}

// UnkCallbackList without its last field (0x40 bytes; refl_IterSync, fn_710023ac4c and its
// copies).
template <class Tag>
struct UnkCallbackListShort : UnkCallbackListBase<Tag> {
    u64 field_0x30;
    UnkRefPtrMember owner;
    ~UnkCallbackListShort() { this->vtable = UnkCallbackTraits<Tag>::vtable() + 0x10; }
};
}  // namespace base

// A hasher fed with values and finished into a u32 (fn_71004f0fc8 and its copies): the hash
// stream of base/UnkHashStream.h.
struct UnkHashKeySource {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual u32 vf_0x38();
};

template <class Tag>
struct UnkHashKeyTraits;  // source(): the string id the static is copied from

// The hash of a static string id's hash, a string id's hash at Off, a u32 from the object at
// Off + 8 and the u32 at Off + 0x10.
template <class Tag, u32 Off>
u32 refl_HashKeyFields(char* obj) {
    UnkHashStream_0x38 h;
    fn_710007be8c(&h, 0x16, nullptr);
    static base::global::CStrId s_id(UnkHashKeyTraits<Tag>::source());
    u64 v = *reinterpret_cast<u64*>(reinterpret_cast<char*>(s_id.data) + 0x30);
    fn_710007c3f0(&h, &v, 8);
    v = *reinterpret_cast<u64*>(*reinterpret_cast<char**>(obj + Off) + 0x30);
    fn_710007c3f0(&h, &v, 8);
    *reinterpret_cast<u32*>(&v) = (*reinterpret_cast<UnkHashKeySource**>(obj + Off + 8))->vf_0x38();
    fn_710007c3f0(&h, &v, 4);
    fn_710007c3f0(&h, obj + Off + 0x10, 4);
    void* t = h.dest_0x18;
    u32 r = fn_710007b750(t, fn_710007c1a4(&h), -1);
    fn_710007c0e4(&h);
    return r;
}

// Appends two values to the array at +8 of an object (fn_710007e34c and its copies): the
// results of UnkPushPairTraits<Tag>::first() and second(), typically type getters.
namespace base {
template <class Tag>
struct UnkPushPairTraits;

template <class Tag>
void refl_PushPair(char* self) {
    auto* v = reinterpret_cast<UnkVector<void*>*>(self + 8);
    v->PushBack(UnkPushPairTraits<Tag>::first());
    v->PushBack(UnkPushPairTraits<Tag>::second());
}
}  // namespace base

// Calls each callback in a deferred set with a value, last first, dropping those that return
// non-zero; then applies the pending removals and additions (fn_710003152c and its copies).
namespace base {
template <class Arg>
struct UnkDeferredCallback {
    void* field_0x0;  // cleared when the callback is dropped
    void* context;
    u32 (*fn)(void* context, Arg value);
};

template <class Tag>
struct UnkDeferredInvokeTraits;  // at(items, index): the slot of an item (out of line)

template <class Tag, class Callback, class Call>
__attribute__((always_inline)) inline void refl_DeferredInvokeWith(UnkDeferredSet<Callback>* s, Call call) {
    s->locked = true;
    for (u32 i = s->items.count - 1; (s32)i >= 0; --i) {
        Callback* cb = *UnkDeferredInvokeTraits<Tag>::at(&s->items, i);
        if (call(cb)) {
            cb->field_0x0 = nullptr;
            u32 n = s->items.count;
            if (n > i) {
                u32 last = n - 1;
                if (last > i)
                    s->items.data[i] = s->items.data[last];
                s->items.count = last;
            }
        }
    }
    s->locked = false;
    for (Callback **it = s->pending_remove.data, **end = it + s->pending_remove.count; it != end; ++it) {
        u32 n = s->items.count;
        for (u32 j = 0; j < n; ++j) {
            if (s->items.data[j] == *it) {
                u32 last = n - 1;
                if (j < last)
                    s->items.data[j] = s->items.data[last];
                s->items.count = last;
                break;
            }
        }
    }
    s->pending_remove.count = 0;
    for (Callback **it = s->pending_add.data, **end = it + s->pending_add.count; it != end; ++it) {
        Callback* p = *it;
        s->items.PushBack(p);
    }
    s->pending_add.count = 0;
}

// The value is read from `arg` for each call.
template <class Tag, class Arg>
void refl_DeferredInvoke(UnkDeferredSet<UnkDeferredCallback<Arg>>* s, const Arg* arg) {
    refl_DeferredInvokeWith<Tag>(s, [arg](UnkDeferredCallback<Arg>* cb) {
        Arg value = *arg;
        return cb->fn(cb->context, value);
    });
}

// The same passing `arg` itself (fn_710006ca34 and its copies).
template <class Tag>
void refl_DeferredInvokePtr(UnkDeferredSet<UnkDeferredCallback<const void*>>* s, const void* arg) {
    refl_DeferredInvokeWith<Tag>(s, [arg](UnkDeferredCallback<const void*>* cb) { return cb->fn(cb->context, arg); });
}
}  // namespace base

// A polymorphic element of a string id and a T, copied through a base and a derived class
// (vtables from UnkPolyStrIdTraits<Tag>; iterator values, fn_7100f5ee20 and its copies).
namespace base {
template <class Tag>
struct UnkPolyStrIdTraits;  // vtable(), base_vtable()

template <class Tag, class T>
struct UnkPolyStrIdElem {
    char* vtable;
    global::UnkStringData* id;
    T field_0x10;

    UnkPolyStrIdElem(const UnkPolyStrIdElem& other) {
        vtable = UnkPolyStrIdTraits<Tag>::base_vtable() + 0x10;
        id = other.id;
        if (&g_unk_7101d47108->empty != id)
            ++id->ref_count;
        vtable = UnkPolyStrIdTraits<Tag>::vtable() + 0x10;
        field_0x10 = other.field_0x10;
    }
};
}  // namespace base

// Erases the node an iterator points to from a chained hash map keyed by string ids, keeping
// the map's cached first bucket and node current (fn_7100020fa8 and its copies). The bucket
// getter is out of line (UnkChainMapTraits<Tag>::at). Names and fields are placeholders.
namespace base {
template <class Tag, class Id>
struct UnkChainMapNode {
    Id key;
    u64 field_0x8;
    UnkChainMapNode* next;
};

template <class Tag, class Id>
struct UnkChainMap {
    u64 field_0x0;
    u32 bucket_count;
    u8 unk_0xc[0x1c];
    u32 first_bucket;
    UnkChainMapNode<Tag, Id>* first_node;
    UnkAllocator* allocator;
    u32 field_0x40;
    u32 count;
    u32 version;
};

template <class Tag, class Id>
struct UnkChainMapIter {
    UnkChainMapNode<Tag, Id>* node;
    UnkChainMapNode<Tag, Id>* prev;
    UnkChainMap<Tag, Id>* map;
    u32 bucket;
    u32 version;
};

template <class Tag>
struct UnkChainMapTraits;  // at(map, bucket): the bucket's head slot

template <class Tag, class Id>
void refl_ChainMapErase(UnkChainMap<Tag, Id>* m, UnkChainMapIter<Tag, Id>* it) {
    using Node = UnkChainMapNode<Tag, Id>;
    u32 bucket;
    if (it->map && it->map->version == it->version) {
        bucket = it->bucket;
    } else {
        bucket = -1;
        it->prev = nullptr;
        it->map = nullptr;
        it->node = nullptr;
        it->bucket = -1;
    }
    Node** slot = UnkChainMapTraits<Tag>::at(m, bucket);
    Node* n = it->node;
    if (!n)
        return;
    Node* prev = it->prev;
    if (*slot == n) {
        *slot = n->next;
        if (bucket == m->first_bucket) {
            while (bucket < m->bucket_count && !*UnkChainMapTraits<Tag>::at(m, bucket))
                bucket = ++m->first_bucket;
            if (n == m->first_node) {
                u32 b = m->first_bucket;
                m->first_node = b < m->bucket_count ? *UnkChainMapTraits<Tag>::at(m, b) : nullptr;
            }
        }
    }
    if (prev)
        prev->next = n->next;
    --m->count;
    n->key.~Id();
    m->allocator->vf_0x48(n);
    ++m->version;
}
}  // namespace base



// Destructors of a class with a callback list (base::UnkCallbackList) as a secondary base at
// `Sub`: store the class's vtable, destroy the list, call the primary base's destructor
// (fn_7100768410, fn_7100768504 and their shape copies). UnkDtorTraits<Tag> names the class's
// vtable and base; UnkCallbackTraits<Tag> the list's two vtables.


namespace base {
template <class Tag, u32 Sub>
void refl_CallbackBaseDtor(char* self) {
    *reinterpret_cast<char**>(self) = UnkDtorTraits<Tag>::vtable() + 0x10;
    reinterpret_cast<UnkCallbackList<Tag>*>(self + Sub)->~UnkCallbackList();
    UnkDtorTraits<Tag>::base(self);
}

template <class Tag, u32 Sub>
void refl_CallbackBaseDeletingDtor(char* self) {
    refl_CallbackBaseDtor<Tag, Sub>(self);
    __builtin_operator_delete(self);
}
}  // namespace base


// A linked list whose nodes come from an embedded fixed-size block pool (fn_71000e9f00, the
// constructor, and fn_71000ebef4, the destructor, with their shape copies). The list's base
// class holds the count, head, tail, allocator and allocation flags; the derived class adds the
// pool and makes it the list's allocator. Layouts and names are placeholders.


// The block pool's free list is UnkBlockFreeList (base/UnkBlockFreeList.h).
extern char g_unk_7101a1a4c8[];  // the pool's vtable

namespace base {
// vtable(): the list's; base_vtable(): its base's.
template <class Tag>
struct UnkPoolListTraits;

// An allocator handing out blocks of a buffer taken from `parent`.
struct UnkBlockPool {
    char* vtable;
    UnkAllocator* parent;
    UnkBlockFreeList blocks;
    u32 field_0x38;
    bool field_0x3c;

    void Init() {
        vtable = g_unk_7101a1a4c8 + 0x10;
        parent = nullptr;
        fn_710008377c(&blocks);
    }
    ~UnkBlockPool() {
        vtable = g_unk_7101a1a4c8 + 0x10;
        Release();
    }
    void Release() {
        if (void* b = blocks.buffer) {
            fn_71000837f4(&blocks);
            parent->vf_0x48(b);
            parent = nullptr;
        }
    }
};

template <class Tag, u32 Next>
struct UnkPoolListBase {
    char* vtable;
    u32 count;
    char* head;
    char* tail;
    UnkAllocator* allocator;
    u32 flags;

    __attribute__((always_inline)) void Init(u32 flags, UnkAllocator* a) {
        vtable = UnkPoolListTraits<Tag>::base_vtable() + 0x10;
        count = 0;
        head = nullptr;
        tail = nullptr;
        if (!a)
            a = fn_7100083990();
        allocator = a;
        this->flags = flags;
    }
    ~UnkPoolListBase() {
        vtable = UnkPoolListTraits<Tag>::base_vtable() + 0x10;
        FreeNodes();
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
    void FreeNodes() {
        for (char* n = head; n;) {
            char* next = *reinterpret_cast<char**>(n + Next);
            allocator->vf_0x48(n);
            n = next;
        }
    }
};

template <class Tag, u32 Next, u32 Elem>
struct UnkPoolList : UnkPoolListBase<Tag, Next> {
    UnkBlockPool pool;

    // The constructor, as a function: the class's vtable is stored before the pool is set up.
    __attribute__((always_inline)) void Init(u32 n, u32 flags, UnkAllocator* a) {
        UnkPoolListBase<Tag, Next>::Init(flags, a);
        this->vtable = UnkPoolListTraits<Tag>::vtable() + 0x10;
        pool.Init();
        UnkAllocator* parent = this->allocator;
        pool.field_0x38 = 0;
        pool.field_0x3c = false;
        u32 f = this->flags;
        pool.Release();
        if (n) {
            if (!parent)
                parent = fn_7100083990();
            pool.parent = parent;
            u64 size = (u64)n * Elem;
            fn_710008378c(&pool.blocks, parent->vf_0x40(size, 8, f), size, Elem);
        }
        // The pool's address, formed as an integer: the original recomputes it here rather than
        // keeping &pool live across the calls.
        this->allocator = reinterpret_cast<UnkAllocator*>(reinterpret_cast<u64>(this) + 0x30);
    }
    ~UnkPoolList() {
        this->vtable = UnkPoolListTraits<Tag>::vtable() + 0x10;
        pool.field_0x3c = true;
        this->FreeNodes();
        this->count = 0;
        this->head = nullptr;
        this->tail = nullptr;
        this->allocator = nullptr;
    }
};

static_assert(sizeof(UnkPoolList<void, 0, 0>) == 0x70);

// The constructor doesn't use Next (its instantiations pass 0).
template <class Tag, u32 Next, u32 Elem>
void refl_PoolListCtor(UnkPoolList<Tag, Next, Elem>* self, u32 n, u32, u32 flags, UnkAllocator* a) {
    self->Init(n, flags, a);
}

template <class Tag, u32 Next, u32 Elem>
void refl_PoolListDtor(UnkPoolList<Tag, Next, Elem>* self) {
    self->~UnkPoolList();
}
}  // namespace base


// A translation unit's static initialiser that builds a reflected type (fn_71001aab70 and its
// shape copies). Every such unit has its own copy of two header-defined string ids (after a u32,
// 6) and builds one type object from its name, registering each destructor with __cxa_atexit
// and then adding the type to a list. Names and layouts are placeholders.



extern const char g_unk_7101618faa[] __attribute__((visibility("hidden")));  // the ids' strings
extern const char g_unk_71015ba597[] __attribute__((visibility("hidden")));
// fn_7100096604, the type object's destructor, is declared in base/reflection/UnkReflTypeDtors.h.

namespace base {
// ids(): this unit's copy of the header statics (hidden); type(): the type object; name(): its
// name; list(): what it is added to; init(type, name, 0, 0, factory, 0) builds it.
template <class Tag>
struct UnkTypeInitTraits;

struct UnkTypeInitIds {
    u32 field_0x0;
    global::UnkStrIdBase field_0x8;
    global::UnkStrIdBase field_0x10;
};

template <class Tag>
void refl_TypeStaticInit() {
    using Traits = UnkTypeInitTraits<Tag>;
    UnkTypeInitIds* ids = Traits::ids();
    ids->field_0x0 = 6;
    new (&ids->field_0x8) global::UnkStrIdBase(g_unk_7101618faa, true);
    __cxa_atexit(fn_710000051c, &ids->field_0x8, g_unk_7101a19000);
    new (&ids->field_0x10) global::UnkStrIdBase(g_unk_71015ba597, true);
    __cxa_atexit(fn_710000051c, &ids->field_0x10, g_unk_7101a19000);
    {
        global::CStrId name(Traits::name(), true);
        Traits::init(Traits::type(), name);
    }
    __cxa_atexit(fn_7100096604, Traits::type(), g_unk_7101a19000);
    fn_710009ec00(Traits::list(), Traits::type());
}
}  // namespace base


// Arrays of 0x18-byte polymorphic {vtable, string id, value} elements (fn_7100f5fbb8, a
// push_back, and its shape copies). An element's copy constructor stores its base's vtable
// (constant), copies the id, then stores its own class's vtable and copies the value; it is
// destroyed through its vtable (slot 1). Names and layouts are placeholders.



extern char g_unk_7101cab1f8[];  // the elements' base vtable

namespace base {
// vtable(): the element class's vtable; push_back(v, e) (refl_IdValueAssign only): the array's
// refl_IdValuePushBack, called out of line.
template <class Tag>
struct UnkIdValueTraits;

// V is UnkCloneElem<Tag, 4 or 8>: copied as bytes (no type-based alias info), so the array's
// count is reloaded after the value is stored, as in the original.
template <class Tag, class V>
struct UnkIdValueElem {
    char* vtable;
    global::CStrId id;
    V value;

    UnkIdValueElem(const UnkIdValueElem& other) : vtable(g_unk_7101cab1f8 + 0x10), id(other.id) {
        vtable = UnkIdValueTraits<Tag>::vtable() + 0x10;
        value = other.value;
    }
    // The virtual destructor (slot 1).
    void Destroy() { reinterpret_cast<void (**)(UnkIdValueElem*)>(vtable)[1](this); }
};

template <class Tag, class V>
inline void refl_IdValueMoveTo(UnkVector<UnkIdValueElem<Tag, V>>* v, u32 new_capacity) {
    using T = UnkIdValueElem<Tag, V>;
    T* p = static_cast<T*>(v->allocator->vf_0x40((u64)new_capacity * sizeof(T), 8, v->flags));
    if (v->data) {
        T* src = v->data;
        T* dst = p;
        for (u32 i = 0; i < v->count; ++i, ++src, ++dst) {
            new (dst) T(*src);
            src->Destroy();
        }
        v->allocator->vf_0x48(v->data);
    }
    v->data = p;
    v->capacity = new_capacity;
}

// UnkVector::PushBack for elements copied one by one: the growth (and hang) of PushBack, the
// old elements copied over and destroyed through their vtables.
template <class Tag, class V>
void refl_IdValuePushBack(UnkVector<UnkIdValueElem<Tag, V>>* v, const UnkIdValueElem<Tag, V>& value) {
    using T = UnkIdValueElem<Tag, V>;
    u32 n = v->count;
    u32 old_capacity = v->capacity;
    u32 new_capacity = old_capacity;
    if (old_capacity <= n && v->step != 0)
        new_capacity = (n / v->step + 1) * v->step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    if (old_capacity < new_capacity) {
        refl_IdValueMoveTo(v, new_capacity);
        n = v->count;
    }
    new (&v->data[n]) T(value);
    ++v->count;
}

// The array's copy-assignment (fn_7100f5fd88 and its shape copies): destroys the elements,
// grows to the other's capacity and pushes a copy of each of its elements.
template <class Tag, class V>
UnkVector<UnkIdValueElem<Tag, V>>* refl_IdValueAssign(UnkVector<UnkIdValueElem<Tag, V>>* v,
                                                      const UnkVector<UnkIdValueElem<Tag, V>>* other) {
    using T = UnkIdValueElem<Tag, V>;
    if (v != other) {
        for (u64 i = 0; i < v->count; ++i)
            v->data[i].Destroy();
        v->count = 0;
        u32 cap = other->capacity;
        if (v->capacity < cap)
            refl_IdValueMoveTo(v, cap);
        for (const T *p = other->data, *end = p + other->count; p != end; ++p)
            UnkIdValueTraits<Tag>::push_back(v, *p);
    }
    return v;
}
}  // namespace base

// refl_CallAndCastField followed by a registration: the object at +Sub is appended to a list of
// 8-byte entries (u32 count at +0x1618, entries from +0x161c), then handed to a callee together
// with the cached object's member at +Off (fn_71003b8880 and its copies). Names and roles are
// placeholders inferred from the code.

namespace base::reflection {

// call(sub, member): the callee the registered object is handed to.
template <class Tag>
struct UnkCastRegisterTraits;

// A count followed by 4-byte-aligned pointer entries (the original stores them at +4).
struct UnkCastRegisterList {
    u32 count;
    struct __attribute__((packed, aligned(4))) Entry {
        char* p;
    } items[1];

    void Push(char* p) {
        u32 n = count;
        items[n].p = p;
        count = n + 1;
    }
};

template <class Tag, u32 Src, u32 Dst, u32 Sub, u32 Off>
bool refl_CallAndCastFieldRegister(char* self) {
    bool r = UnkCastTraits<Tag>::base(self);
    UnkReflObject* o = *reinterpret_cast<UnkReflObject**>(self + Src);
    *reinterpret_cast<UnkReflObject**>(self + Dst) = o ? refl_CastTo<Tag>(o) : nullptr;
    char* sub = self + Sub;
    reinterpret_cast<UnkCastRegisterList*>(self + 0x1618)->Push(sub);
    // Loaded after the push: the store may alias the cached pointer.
    char* cached = *reinterpret_cast<char**>(self + Dst);
    UnkCastRegisterTraits<Tag>::call(sub, cached + Off);
    return r;
}

}  // namespace base::reflection

// UnkIterValue::operator=(UnkIterValue&&) (UnkCollectionIter.h) with a u32 or u64 index and an
// element whose copy-assignment is out of line (UnkCloneAssignElem, UnkClone.h; it has no
// move-assignment, so the element is copy-assigned; fn_71000d4f1c and its copies).

// A 64-bit index stored bytewise: the original reloads the state after storing it, which a
// typed u64 store wouldn't force.
template <class Tag>
struct UnkCloneIndexU64 {
    u64 value;

    // By value, so the other's index is read before the destination is chosen.
    UnkCloneIndexU64& operator=(UnkCloneIndexU64 other) {
        __builtin_memcpy(&value, &other.value, sizeof(value));
        return *this;
    }
};

// The destructor of a class that owns one object at +0x108 (freed with its out-of-line
// destructor and operator delete), with its base's destructor inlined: the base marks itself
// dead (0xdeaddead at +0x100), destroys the object at +0x80, releases a counted object at +0x78
// (count at +0x54, freed by fn_710001737c) and a string id at +0x10 (fn_71001f2ae4 and its
// copies). Names and roles are placeholders inferred from the code.

void fn_7100216934(char* p);

namespace base {

// vtable(), base_vtable(), and destroy(p): the owned object's destructor.
template <class Tag>
struct UnkOwnerDtorTraits;

template <class Tag>
void refl_OwnerDtor(char* self) {
    using Traits = UnkOwnerDtorTraits<Tag>;
    *reinterpret_cast<char**>(self) = Traits::vtable() + 0x10;
    char* owned = *reinterpret_cast<char**>(self + 0x108);
    *reinterpret_cast<char**>(self + 0x108) = nullptr;
    if (owned) {
        Traits::destroy(owned);
        __builtin_operator_delete(owned);
    }
    *reinterpret_cast<char**>(self) = Traits::base_vtable() + 0x10;
    *reinterpret_cast<u32*>(self + 0x100) = 0xdeaddead;
    fn_7100216934(self + 0x80);
    if (UnkCountedResource* counted = *reinterpret_cast<UnkCountedResource**>(self + 0x78)) {
        if (--counted->ref_count == 0)
            fn_710001737c(counted);
    }
    reinterpret_cast<UnkStrIdMember*>(self + 0x10)->~UnkStrIdMember();
}

}  // namespace base

// Appends the type a type getter returns (e.g. fn_7100009658, CStrId's) to the array at +8, then tail-calls the next step with
// the same object (fn_7100302b38 and its copies). Names and roles are placeholders inferred
// from the code.

namespace base::reflection {
struct CType;
}

namespace base {

// get(): the type appended; next(self): the function called afterwards.
template <class Tag>
struct UnkPushNextTraits;

template <class Tag>
void refl_PushThenNext(char* self) {
    using Traits = UnkPushNextTraits<Tag>;
    reinterpret_cast<UnkVector<const reflection::CType*>*>(self + 8)->PushBack(Traits::get());
    Traits::next(self);
}

}  // namespace base

// Sets up a chained hash map's storage (UnkHashMap.h layout; fn_7100075958 and its copies):
// releases the node pool's old block, carves a new block of `nodes` nodes of NodeSize bytes
// (from the map's allocator, or the default one) into the pool at +0x58, makes the pool at +0x48
// the map's allocator, then resizes the bucket array at +0x10 to `buckets`, zero-filling new
// buckets. Names and roles are placeholders inferred from the code.

namespace base {

template <class Tag, u64 NodeSize>
void refl_HashMapInit(char* self, u32 buckets, u32 nodes) {
    u32 flags = *reinterpret_cast<u32*>(self + 8);
    UnkAllocator* a = *reinterpret_cast<UnkAllocator**>(self);
    UnkAllocator*& pool_alloc = *reinterpret_cast<UnkAllocator**>(self + 0x50);
    if (void* block = *reinterpret_cast<void**>(self + 0x78)) {
        fn_71000837f4(reinterpret_cast<UnkBlockFreeList*>(self + 0x58));
        pool_alloc->vf_0x48(block);
        pool_alloc = nullptr;
    }
    if (nodes) {
        if (!a)
            a = fn_7100083990();
        pool_alloc = a;
        u64 size = (u64)nodes * NodeSize;
        void* block = a->vf_0x40(size, 8, flags);
        fn_710008378c(reinterpret_cast<UnkBlockFreeList*>(self + 0x58), block, size, NodeSize);
    }
    *reinterpret_cast<char**>(self) = self + 0x48;
    UnkVector<void*>* v = reinterpret_cast<UnkVector<void*>*>(self + 0x10);
    u32 count = v->count;
    if (count < buckets) {
        if (v->capacity < buckets) {
            void** p = static_cast<void**>(v->allocator->vf_0x40((u64)buckets * 8, 8, v->flags));
            if (v->data) {
                __builtin_memcpy(p, v->data, (u64)v->count * 8);
                v->allocator->vf_0x48(v->data);
            }
            count = v->count;
            v->data = p;
            v->capacity = buckets;
        }
        __builtin_memset(v->data + count, 0, (u64)(buckets - count) * 8);
        v->count = buckets;
    } else if (count > buckets) {
        v->count = buckets;
    }
}

}  // namespace base

// Appends two types to the array at +8: a type object named through the GOT, then the one a
// type getter returns (fn_7100601f24 and its copies). Names and roles are placeholders inferred
// from the code.

namespace base::reflection {
struct CType;
}

namespace base {

// first(): the type object's address; get(): the getter.
template <class Tag>
struct UnkPushTwoTraits;

template <class Tag>
void refl_PushTwoTypes(char* self) {
    using Traits = UnkPushTwoTraits<Tag>;
    UnkVector<const reflection::CType*>* v = reinterpret_cast<UnkVector<const reflection::CType*>*>(self + 8);
    v->PushBack(Traits::first());
    v->PushBack(Traits::get());
}

}  // namespace base


// Assigns a string id and a value into the entry a lookup finds (or inserts) for a u32 key
// (fn_7100f5f6fc and its shape copies). The lookup is a per-clone function, named through
// UnkKeyedSetTraits<Tag>::find; the source has the entry's layout: the id at Off, the value
// (u32 or u64) right after it.


namespace base {
template <class Tag>
struct UnkKeyedSetTraits;

template <class Tag, class Id, class V, u64 Off>
void refl_KeyedSetAssign(void* map, const u32* key, const char* src) {
    char* e = UnkKeyedSetTraits<Tag>::find(map, *key);
    *reinterpret_cast<Id*>(e + Off) = *reinterpret_cast<const Id*>(src + Off);
    *reinterpret_cast<V*>(e + Off + 8) = *reinterpret_cast<const V*>(src + Off + 8);
}
}  // namespace base


// The entry of a u32-keyed set whose value is a string id and a u32/u64 (fn_7100f5ef98 and its
// shape copies copy-assign an iterator value over it). The id sits at Off (0 or 8; what comes
// before it is not copied), the value right after it.


namespace base {
template <u64 N>
struct UnkKeyedSetHead {
    u64 words[N];
};
template <>
struct UnkKeyedSetHead<0> {};

template <class Tag, class Id, class V, u64 Off>
struct UnkKeyedSetEntry : UnkKeyedSetHead<Off / 8> {
    Id id;
    V value;

    UnkKeyedSetEntry& operator=(const UnkKeyedSetEntry& other) {
        id = other.id;
        value = other.value;
        return *this;
    }
};
}  // namespace base


// refl_Begin/refl_End over an array of polymorphic elements holding a string id (fn_7100f606b8,
// fn_7100f605d0 and their shape copies). The iterator's assignment syncs its current value out
// of line (UnkVtIterTraits<Tag>::sync, a refl_IterSync copy); the temporary's owned value would
// be destroyed inline: the base class's vtable is stored, then the id released.


namespace base {
// base_vtable(): the element's base class vtable; sync(it): the out-of-line refl_IterSync.
template <class Tag>
struct UnkVtIterTraits;

template <class Tag, u64 Size>
struct UnkStrIdVtElem {
    char* vtable;
    UnkStrIdMember id;
    u64 words[(Size - 0x10) / 8];

    ~UnkStrIdVtElem() { vtable = UnkVtIterTraits<Tag>::base_vtable() + 0x10; }
};

template <class Tag, u64 Size>
inline void refl_VtIterAssign(UnkCollectionIter<UnkStrIdVtElem<Tag, Size>>* out,
                              const UnkCollectionIter<UnkStrIdVtElem<Tag, Size>>& it) {
    if (out != &it) {
        out->index = it.index;
        out->element = it.element;
        UnkVtIterTraits<Tag>::sync(out);
    }
}

template <class Tag, u64 Size>
void refl_VtBegin(const UnkVector<UnkStrIdVtElem<Tag, Size>>* v, UnkCollectionIter<UnkStrIdVtElem<Tag, Size>>* out) {
    UnkCollectionIter<UnkStrIdVtElem<Tag, Size>> it(0, v->data);
    refl_VtIterAssign(out, it);
}

template <class Tag, u64 Size>
void refl_VtEnd(const UnkVector<UnkStrIdVtElem<Tag, Size>>* v, UnkCollectionIter<UnkStrIdVtElem<Tag, Size>>* out) {
    UnkCollectionIter<UnkStrIdVtElem<Tag, Size>> it(v->count, v->data + v->count);
    refl_VtIterAssign(out, it);
}
}  // namespace base


// Moves an element in at `index` of an array of owner arrays (fn_71004a918c and its shape
// copies): appending is the out-of-line EmplaceBack (UnkOwnerVecInsertTraits<Tag>::append, a
// tail call); otherwise the array grows to the next multiple of the step and the elements from
// `index` on move up one, from the last. Each element is a UnkCloneVecHandle whose array holds
// owning pointers, freed when it is destroyed.



template <class Tag>
struct UnkCloneOwnerVecHandle : UnkCloneVecHandle<Tag> {
    UnkCloneOwnerVecHandle(UnkCloneOwnerVecHandle&& other)
        : UnkCloneVecHandle<Tag>(static_cast<UnkCloneVecHandle<Tag>&&>(other)) {}
    ~UnkCloneOwnerVecHandle() { reinterpret_cast<base::UnkVector<UnkCloneOwner<Tag>>*>(this)->Free(); }
};

namespace base {
// append(v, e): the out-of-line EmplaceBack.
template <class Tag>
struct UnkOwnerVecInsertTraits;

template <class Tag>
void refl_OwnerVecInsert(UnkVector<UnkCloneOwnerVecHandle<Tag>>* v, UnkCloneOwnerVecHandle<Tag>* e, u32 index) {
    using E = UnkCloneOwnerVecHandle<Tag>;
    u32 n = v->count;
    if (n == index) {
        UnkOwnerVecInsertTraits<Tag>::append(v, e);
        return;
    }
    u32 cap = v->capacity;
    if (cap <= n && v->step != 0) {
        u32 grown = (n / v->step + 1) * v->step;
        if (cap < grown) {
            E* p = static_cast<E*>(v->allocator->vf_0x40((u64)grown * sizeof(E), 8, v->flags));
            if (E* s = v->data) {
                E* o = p;
                for (u32 i = 0; i < v->count; ++i, ++s, ++o) {
                    new (o) E(static_cast<E&&>(*s));
                    s->~E();
                }
                v->allocator->vf_0x48(v->data);
            }
            v->data = p;
            v->capacity = grown;
        }
    }
    for (u32 i = v->count - 1; i >= index; --i) {
        E* q = &v->data[i];
        new (q + 1) E(static_cast<E&&>(*q));
        v->data[i].~E();
        if (i == 0)
            break;
    }
    new (&v->data[index]) E(static_cast<E&&>(*e));
    ++v->count;
}
}  // namespace base

// A two-alternative value: 1 holds a string id and a value of `Size` bytes with an out-of-line
// destructor (a reflected value in most copies), 2 a 16-byte pair, 0 is empty. Assigning a pair
// or another variant's pair resets first (fn_71000c3c14, fn_71000c3cc0 and their shape copies).
namespace base {
template <class Tag, u64 Size>
struct UnkStrIdValueVariant {
    struct Pair {
        u64 a, b;
    };
    union {
        struct {
            UnkStrIdMember id;
            UnkCloneDtorElem<Tag, Size> value;
        } named;
        Pair pair;
    };
    u32 state;

    void Reset() {
        switch (state) {
        case 1:
            named.value.~UnkCloneDtorElem();
            named.id.~UnkStrIdMember();
            state = 0;
            break;
        case 2:
            state = 0;
            break;
        }
    }
    UnkStrIdValueVariant& operator=(const Pair& p) {
        if (static_cast<const void*>(&p) != this) {
            u64 a = p.a, b = p.b;
            Reset();
            state = 2;
            pair.a = a;
            pair.b = b;
        }
        return *this;
    }
    UnkStrIdValueVariant& AssignPair(const UnkStrIdValueVariant& other) {
        if (this != &other) {
            Reset();
            state = other.state;
            pair = other.pair;
        }
        return *this;
    }
};

template <class Tag, u64 Size>
UnkStrIdValueVariant<Tag, Size>& refl_VariantAssign(UnkStrIdValueVariant<Tag, Size>* self,
                                                   const UnkStrIdValueVariant<Tag, Size>* other) {
    return self->AssignPair(*other);
}

template <class Tag, u64 Size>
UnkStrIdValueVariant<Tag, Size>& refl_VariantAssignPair(UnkStrIdValueVariant<Tag, Size>* self,
                                                        const typename UnkStrIdValueVariant<Tag, Size>::Pair* p) {
    return *self = *p;
}
}  // namespace base

// Destroys a keyed array of values with an out-of-line destructor (the UnkCloneKeyedArray
// layout; 0x18-byte reflected values in most copies): releases the keys, destroys the
// values, then frees both (fn_710009abf8 and its shape copies).
namespace base {
template <class Tag, class V>
struct UnkKeyedValueArray {
    V* values;
    UnkStrIdMember* keys;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 flags;
    UnkAllocator* allocator;

    __attribute__((always_inline)) ~UnkKeyedValueArray() {
        if (keys && count)
            for (u64 i = 0; i < count; ++i)
                keys[i].~UnkStrIdMember();
        if (values && count)
            for (u64 i = 0; i < count; ++i)
                values[i].~V();
        count = 0;
        if (keys)
            allocator->vf_0x48(keys);
        if (values)
            allocator->vf_0x48(values);
    }
};

// The array is at `Offset` in the object destroyed.
template <class Tag, class V, u32 Offset>
void refl_KeyedValueArrayDtor(char* self) {
    reinterpret_cast<UnkKeyedValueArray<Tag, V>*>(self + Offset)->~UnkKeyedValueArray();
}
}  // namespace base

// Reads a value from a keyed array of reflected values at +0x20 (UnkKeyedValueArray) into `out`,
// converted to the traits' type; false when the key is missing or the value is empty
// (fn_710006d11c and its copies, one per value type).
namespace base::reflection {
// A typed pointer: a type descriptor and the object it describes.
struct UnkTypedRef {
    const CType* type;
    void* ptr;
};
}  // namespace base::reflection

void fn_71000a16e0(base::reflection::UnkTypedRef* out, const char* type, void* data);
bool fn_710009d7a8(const base::reflection::CType* type, base::reflection::UnkTypedRef* dst,
                   base::reflection::UnkTypedRef* src);

namespace base {
template <class Tag, class V = reflection::UnkReflValue>
struct UnkKeyedValueLookup : UnkKeyedValueArray<Tag, V> {
    V* End() { return &this->values[this->count]; }
    V* Find(const UnkStrIdMember& key) {
        for (u64 i = 0; i < this->count; ++i)
            if (this->keys[i].data == key.data)
                return &this->values[i];
        return End();
    }
};

template <class Tag>
bool refl_GetKeyedValue(char* self, const UnkStrIdMember* key, void* out) {
    using namespace reflection;
    auto* a = reinterpret_cast<UnkKeyedValueLookup<Tag>*>(self + 0x20);
    UnkReflValue* v = a->Find(*key);
    if (v == a->End())
        return false;
    const char* type = v->type;
    if (!type)
        return false;
    void* p = v->field_0x14 == 1 || *reinterpret_cast<const u64*>(type + 0x18) > 8 ? v->field_0x0 : v;
    if (!p)
        return false;
    const CType* t = UnkFieldTraits<Tag>::type();
    UnkTypedRef dst{UnkFieldTraits<Tag>::type(), out};
    UnkTypedRef src;
    fn_71000a16e0(&src, type, p);
    return fn_710009d7a8(t, &dst, &src);
}
}  // namespace base

// The deleting destructor of a class holding a reflected value at +0x18, with four 16-byte
// members after it that clear themselves (fn_71002fa6cc and its copies). UnkValueHolderTraits
// names the base class's vtable.
namespace base {
template <class Tag>
struct UnkValueHolderTraits;  // vtable()

struct UnkClearedPair {
    u64 a, b;
    ~UnkClearedPair() {
        a = 0;
        b = 0;
    }
};

template <class Tag>
struct UnkValueHolder {
    void* vtable;
    u64 field_0x8;
    u64 field_0x10;
    reflection::UnkReflValue value;
    UnkClearedPair pairs[4];
};

template <class Tag>
void refl_ValueHolderDeletingDtor(UnkValueHolder<Tag>* self) {
    for (int i = 3; i >= 0; --i)
        self->pairs[i].~UnkClearedPair();
    self->vtable = UnkValueHolderTraits<Tag>::vtable() + 0x10;
    fn_71000a1c60(&self->value);
    ::operator delete(self);
}
}  // namespace base

// A copy constructor of a 0x48-byte class derived from a polymorphic base of two words: the
// base's vtable and copy, then 0x1c bytes from 0x10 and six u32 fields (0x34 is padding), then
// the class's vtable (fn_710098e094 and its copies, one per derived class).
namespace base {
template <class Tag>
void refl_DerivedCopyCtor48(char* self, const char* other) {
    *reinterpret_cast<char**>(self) = UnkCopyCtorTraits<Tag>::base_vtable() + 0x10;
    __builtin_memcpy(self + 8, other + 8, 8);
    __builtin_memcpy(self + 0x10, other + 0x10, 0x1c);
    static constexpr u32 fields[] = {0x2c, 0x30, 0x38, 0x3c, 0x40, 0x44};
    for (u32 off : fields)
        *reinterpret_cast<u32*>(self + off) = *reinterpret_cast<const u32*>(other + off);
    *reinterpret_cast<char**>(self) = UnkCopyCtorTraits<Tag>::vtable() + 0x10;
}
}  // namespace base

// A copy constructor through two polymorphic bases: the first base's vtable and two words, the
// second base's vtable and a string id at 0x18 (retained), then 0x14 bytes from 0x20 and the
// class's vtable (fn_7100bf7624 and its copies, one per class). UnkCopyCtorTraits names the
// first base's (base_vtable), the second base's (mid_vtable) and the class's vtables.
namespace base {
template <class Tag>
void refl_StrIdCopyCtor(char* self, const char* other) {
    using Traits = UnkCopyCtorTraits<Tag>;
    *reinterpret_cast<char**>(self) = Traits::base_vtable() + 0x10;
    __builtin_memcpy(self + 8, other + 8, 16);
    *reinterpret_cast<char**>(self) = Traits::mid_vtable() + 0x10;
    global::UnkStringData* d = *reinterpret_cast<global::UnkStringData* const*>(other + 0x18);
    *reinterpret_cast<global::UnkStringData**>(self + 0x18) = d;
    if (&g_unk_7101d47108->empty != d)
        ++d->ref_count;
    __builtin_memcpy(self + 0x20, other + 0x20, 0x14);
    *reinterpret_cast<char**>(self) = Traits::vtable() + 0x10;
}
}  // namespace base

// A keyed-node iterator (node at 0, key at +0 and value at +8 of the node) whose current pair
// has an out-of-line destructor (UnkCloneDtorTraits); assigning copies the node and re-points
// or resets the pair (fn_7100239cf4 and its shape copies).
namespace base {
template <class Tag, u64 Size>
struct UnkKeyedPairValue {
    union {
        UnkCloneDtorElem<Tag, Size> pair;
        struct {
            char* key;
            char* value;
        } ref;
    };
    s32 state;  // 0 empty, 1 owns `pair`, 2 refers through `ref`

    void Reset() {
        if (state == 2) {
        } else if (state == 1) {
            pair.~UnkCloneDtorElem();
        } else {
            return;
        }
        state = 0;
    }
    void SetRef(char* key, char* value) {
        Reset();
        state = 2;
        ref.key = key;
        ref.value = value;
    }
};

template <class Tag, u64 Size>
struct UnkKeyedPairIter {
    char* node;
    UnkKeyedPairValue<Tag, Size> current;
};

template <class Tag, u64 Size>
UnkKeyedPairIter<Tag, Size>* refl_KeyedPairIterAssign(UnkKeyedPairIter<Tag, Size>* self,
                                                      const UnkKeyedPairIter<Tag, Size>* other) {
    if (self != other) {
        self->node = other->node;
        if (char* n = self->node)
            self->current.SetRef(n, n + 8);
        else
            self->current.Reset();
    }
    return self;
}
}  // namespace base

// Sets a 4x4 matrix of f32 at +0x18 when it differs from the current one (read through
// fn_710002614c), then updates the flags at +0x70 with a global mask and marks the object dirty
// (fn_71001f30c0, fn_71001f3d60 and their copies; CurFirst: the comparison's operand order).
namespace base {
template <class Tag>
struct UnkSetMatrixTraits;  // mask(): the global the flags are cleared with

template <class Tag, bool CurFirst>
void refl_SetMatrix(char* self, const UnkMatrix44* m) {
    UnkMatrix44* cur = reinterpret_cast<UnkMatrix44*>(self + 0x18);
    const UnkMatrix44* now = &fn_710002614c(cur);
    if ((CurFirst ? __builtin_memcmp(now, m, sizeof(UnkMatrix44)) : __builtin_memcmp(m, now, sizeof(UnkMatrix44))) == 0)
        return;
    for (int i = 0; i < 16; ++i)
        cur->m[i] = m->m[i];
    u32& flags = *reinterpret_cast<u32*>(self + 0x70);
    flags = (flags | 8) & ~UnkSetMatrixTraits<Tag>::mask();
    self[0xfa] = 1;
}
}  // namespace base

// A chained hash map of string-id keys to values of `Size` bytes copied by an out-of-line
// constructor (UnkValueMapTraits::copy; reflected values in most copies). Nodes are linked into
// a bucket chain and a list of all nodes (tail at +0x38, head at +0x40). Insert takes a key and
// a value (fn_71000d6914 and its shape copies). The node's list link is stored through a raw
// pointer, so the tail is reloaded after it, as in the original.
namespace base {
template <class Tag>
struct UnkValueMapTraits;  // copy(dst, src): the value's copy constructor

// The string data's hash words.
struct UnkHashedStringData {
    char unk_0x0[0x30];
    u32 hash_lo;
    u32 hash_hi;
};

template <class Tag, u64 Size, global::UnkStringPool*& Pool>
struct UnkValueMapNode {
    global::UnkStringData* key;
    UnkCloneBlock<Tag, Size> value;
    UnkValueMapNode* next;
    UnkValueMapNode* prev;
    UnkValueMapNode* chain;

    UnkValueMapNode(global::UnkStringData* const* k, const void* v) {
        global::UnkStringData* d = *k;
        key = d;
        if (&Pool->empty != d)
            ++d->ref_count;
        UnkValueMapTraits<Tag>::copy(&value, v);
        prev = nullptr;
        chain = nullptr;
        next = nullptr;
    }
};

template <class Tag, u64 Size, global::UnkStringPool*& Pool>
struct UnkValueMap {
    using Node = UnkValueMapNode<Tag, Size, Pool>;
    UnkAllocator* allocator;
    u32 flags;
    u32 count;
    Node** buckets;
    u32 bucket_used;
    u32 bucket_count;
    u64 field_0x20;
    u64 field_0x28;
    u64 field_0x30;
    Node* tail;
    Node* head;

    void Insert(global::UnkStringData* const* key, const void* value) {
        Node** b = nullptr;
        if (bucket_used) {
            const auto* d = reinterpret_cast<const UnkHashedStringData*>(*key);
            u32 h = d->hash_lo ^ d->hash_hi;
            u32 i = h % bucket_count;
            b = buckets + i;
        }
        Node* old = *b;
        auto* n = static_cast<Node*>(allocator->vf_0x40(sizeof(Node), 8, flags));
        if (n)
            new (n) Node(key, value);
        n->chain = old;
        *b = n;
        if (!head)
            head = n;
        else if (Node* t = tail) {
            *reinterpret_cast<Node**>(reinterpret_cast<char*>(n) + __builtin_offsetof(Node, prev)) = t;
            tail->next = n;
        }
        tail = n;
        ++count;
    }
};

template <class Tag, u64 Size, global::UnkStringPool*& Pool>
void refl_ValueMapInsert(UnkValueMap<Tag, Size, Pool>* m, global::UnkStringData* const* key, const void* value) {
    m->Insert(key, value);
}
}  // namespace base

// A copy constructor of a 0xd0-byte class: the base resets its u32 at +8 and stores its vtable,
// then the fields are copied in runs (as their struct copies lower), the shared pointer at 0xc8
// is retained (count at +8, atomically), and the class's vtable is stored (fn_7100b8eec8 and its
// copies, one per class).
namespace base {
template <class T>
inline void refl_CopyField(char* self, const char* other, u32 off) {
    *reinterpret_cast<T*>(self + off) = *reinterpret_cast<const T*>(other + off);
}

template <class Tag>
void refl_SharedCopyCtor(char* self, const char* other) {
    *reinterpret_cast<u32*>(self + 8) = 0;
    *reinterpret_cast<char**>(self) = UnkCopyCtorTraits<Tag>::base_vtable() + 0x10;
    __builtin_memcpy(self + 0x10, other + 0x10, 0x20);
    __builtin_memcpy(self + 0x30, other + 0x30, 0x20);
    refl_CopyField<u32>(self, other, 0x50);
    refl_CopyField<u32>(self, other, 0x54);
    refl_CopyField<u8>(self, other, 0x58);
    __builtin_memcpy(self + 0x60, other + 0x60, 0x10);
    refl_CopyField<u32>(self, other, 0x70);
    __builtin_memcpy(self + 0x74, other + 0x74, 3);
    __builtin_memcpy(self + 0x78, other + 0x78, 0x20);
    refl_CopyField<u32>(self, other, 0x98);
    refl_CopyField<u32>(self, other, 0x9c);
    refl_CopyField<u8>(self, other, 0xa0);
    __builtin_memcpy(self + 0xa8, other + 0xa8, 0x18);
    refl_CopyField<u64>(self, other, 0xc0);
    char* p = *reinterpret_cast<char* const*>(other + 0xc8);
    *reinterpret_cast<char**>(self + 0xc8) = p;
    if (p)
        __atomic_fetch_add(reinterpret_cast<u64*>(p + 8), 1, __ATOMIC_RELAXED);
    *reinterpret_cast<char**>(self) = UnkCopyCtorTraits<Tag>::vtable() + 0x10;
}
}  // namespace base

// The same over an array: the iterator holds an index and an element, and its pair refers to
// both (fn_71008932bc and its shape copies).
namespace base {
template <class Tag, u64 Size>
struct UnkIndexPairIter {
    u32 index;
    char* element;
    UnkKeyedPairValue<Tag, Size> current;
};

template <class Tag, u64 Size>
UnkIndexPairIter<Tag, Size>* refl_IndexPairIterAssign(UnkIndexPairIter<Tag, Size>* self,
                                                      const UnkIndexPairIter<Tag, Size>* other) {
    if (self != other) {
        self->index = other->index;
        self->element = other->element;
        if (char* e = self->element)
            self->current.SetRef(reinterpret_cast<char*>(&self->index), e);
        else
            self->current.Reset();
    }
    return self;
}
}  // namespace base

// Calls the base method, then passes a copy of the owner's string id (the owner at +0x18, its id
// at +0x30) to a method of the member at `Offset` (fn_7100759148 and its copies). The id is a
// union member so its release reloads the pool, as in the original.
namespace base {
template <class Tag>
struct UnkCallWithIdTraits;  // base(self), call(member, id)

template <class Tag, u32 Offset>
void refl_CallWithOwnerId(char* self) {
    UnkCallWithIdTraits<Tag>::base(self);
    char* m = self + Offset;
    global::UnkStringData* d =
        *reinterpret_cast<global::UnkStringData* const*>(*reinterpret_cast<char**>(self + 0x18) + 0x30);
    union Id {
        UnkStrIdMember id;
        Id() {}
        ~Id() { id.~UnkStrIdMember(); }
    } u;
    u.id.data = d;
    if (&g_unk_7101d47108->empty != d)
        ++d->ref_count;
    UnkCallWithIdTraits<Tag>::call(m, &u.id);
}
}  // namespace base

// Lookups in a keyed array (UnkKeyedValueLookup) at `Offset` with values of `Size` bytes: the
// value or null (fn_7100020320 and its copies), or a call with the value when the key is present
// (fn_71000d7b5c and its copies; UnkKeyedFoundTraits names the call).
namespace base {
template <class Tag, u32 Offset, u64 Size>
void* refl_KeyedFind(char* self, const UnkStrIdMember* key) {
    auto* a = reinterpret_cast<UnkKeyedValueLookup<Tag, UnkCloneBlock<Tag, Size>>*>(self + Offset);
    auto* v = a->Find(*key);
    return v == a->End() ? nullptr : v;
}

template <class Tag>
struct UnkKeyedFoundTraits;  // found(self, value)

template <class Tag, u32 Offset, u64 Size>
void refl_KeyedFindThen(char* self, const UnkStrIdMember* key) {
    auto* a = reinterpret_cast<UnkKeyedValueLookup<Tag, UnkCloneBlock<Tag, Size>>*>(self + Offset);
    auto* v = a->Find(*key);
    if (v != a->End())
        UnkKeyedFoundTraits<Tag>::found(self, v);
}

// An out-of-line destructor of a string id (Id) at `Offset` (fn_7100005358 and its copies).
template <class Tag, u32 Offset, class Id>
void refl_StrIdDtorAt(char* self) {
    reinterpret_cast<Id*>(self + Offset)->~Id();
}
}  // namespace base

// Copy-constructs a refcounted handle: null, then Set (fn_7100234e38 and its identical copies).
namespace base {
template <class Tag>
void refl_RefHandleCopyCtor(UnkRefHandle* self, const UnkRefHandle* other) {
    self->ptr = nullptr;
    self->Set(other->ptr);
}
}  // namespace base

// Assigns `value` to the value a key maps to in a keyed array at `Offset`, if present; the
// assignment is out of line (UnkCloneAssignElem; fn_71000d7c58 and its copies). The original
// doesn't tail-call it (probably a non-void result in the source); disable_tail_calls stands in.
namespace base {
template <class Tag, u32 Offset, u64 Size>
__attribute__((disable_tail_calls)) void refl_KeyedFindAssign(char* self, const UnkStrIdMember* key,
                                                              const UnkCloneAssignElem<Tag, Size>* value) {
    auto* a = reinterpret_cast<UnkKeyedValueLookup<Tag, UnkCloneAssignElem<Tag, Size>>*>(self + Offset);
    auto* v = a->Find(*key);
    if (v != a->End())
        *v = *value;
}
}  // namespace base

// The value holder's complete destructor (fn_71002fa684 and its copies; see
// refl_ValueHolderDeletingDtor).
namespace base {
template <class Tag>
void refl_ValueHolderDtor(UnkValueHolder<Tag>* self) {
    for (int i = 3; i >= 0; --i)
        self->pairs[i].~UnkClearedPair();
    self->vtable = UnkValueHolderTraits<Tag>::vtable() + 0x10;
    fn_71000a1c60(&self->value);
}

// A constructor that stores the class's vtable (UnkDtorTraits) and zeroes 0x10..0x24
// (fn_71002fa6b0 and its copies).
template <class Tag>
void refl_ZeroedCtor(char* self) {
    *reinterpret_cast<char**>(self) = UnkDtorTraits<Tag>::vtable() + 0x10;
    *reinterpret_cast<u64*>(self + 0x10) = 0;
    *reinterpret_cast<u64*>(self + 0x18) = 0;
    *reinterpret_cast<u32*>(self + 0x20) = 0;
}

// A destructor that calls a method of the object, then (an inlined base destructor) stores the
// base's vtable (UnkDtorTraits), clears the T at `Offset` and calls the next base destructor
// (fn_71003990ac and its copies).
template <class Tag>
struct UnkDtorCallTraits;  // call(self), base(self)

template <class Tag, class T, u32 Offset>
void refl_DtorCallClear(char* self) {
    UnkDtorCallTraits<Tag>::call(self);
    *reinterpret_cast<char**>(self) = UnkDtorTraits<Tag>::vtable() + 0x10;
    *reinterpret_cast<T*>(self + Offset) = 0;
    UnkDtorCallTraits<Tag>::base(self);
}
}  // namespace base

// Small helpers repeated per class: adding to an f32 field (fn_71003529d4 and its copies), and a
// 0x18-byte record of two zero words and the u32s 2 and 1, initialised in place (fn_710009eef4)
// or returned (its copies that return through x8).
namespace base {
template <class Tag, u32 Offset>
void refl_AddF32(char* self, f32 v) {
    *reinterpret_cast<f32*>(self + Offset) += v;
}

template <class Tag>
struct UnkPairCounts {
    u64 a;
    u64 b;
    u32 c;
    u32 d;
};

template <class Tag>
void refl_InitPairCounts(UnkPairCounts<Tag>* self) {
    self->a = 0;
    self->b = 0;
    self->c = 2;
    self->d = 1;
}

template <class Tag>
UnkPairCounts<Tag> refl_MakePairCounts() {
    return {0, 0, 2, 1};
}
}  // namespace base
