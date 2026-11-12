// base::reflection's unit (tools/tu_map.py unit 346): the slots and special members of two reflected types and
// their containers. Names and layouts are placeholders.
// - A 0x60-byte polymorphic record (vtable g_unk_7101a20c98; destroyed by fn_7100262dd4, moved by
//   fn_7100262f74): its constructor from a name and an owner (fn_710025bc80) builds the string id at +8, an
//   allocator handle and eleven zeroed words, as fn_710025f17c (src/base/reflection/fn_710025f17c.cpp).
// - The map of string ids to those records (values, keys, count, capacity, step, flags, allocator; the
//   types of src/base/core/CAsset/fn_7100018558.cpp): its move-assignment (fn_7100260310), erase at a
//   value (fn_71002613ac, the last entry moved into the gap), shrink to the count (fn_7100260164, as
//   fn_710001871c in CAsset_RecordMapShrink_b49.cpp), the begin/end slots (fn_7100260488, fn_710026059c, as
//   CMinimapData_StrIdMapBeginEnd_A6_b75.cpp) and the add of a record named after a key at +8 of its owner
//   (fn_710025bb90).
// - The 0xc8-byte value of the fixed arrays in UnkUnit346_FixedArrayReset_b60.cpp (vtable g_unk_7101a20c78):
//   its constructors (fn_710025b068, fn_7100260718: a list and two word sets, their allocation flags 5 or
//   0xe) and the insert into its hashed node map at +0x30 (fn_710025c1a4: the key's node is erased as in
//   CActor_PtrMapErase_b61.cpp, then a node with an empty vector is chained in and appended to the list).
#include <new>

#include "../../game/CMinimapData/UnkStrIdIterBeginEnd_A6_b75.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkTU0.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::UnkAllocator;
using base::UnkVector;
using namespace base::global;

void fn_7100262dd4(char*);

struct alignas(8) UnkReflRecord60_ovb0036 {
    u8 data[0x60];
    ~UnkReflRecord60_ovb0036() { fn_7100262dd4(reinterpret_cast<char*>(this)); }
};

void fn_7100260488(UnkStrIdMap_A6_b75<UnkReflRecord60_ovb0036>* self,
                   UnkStrIdIterValue_A6_b75<UnkReflRecord60_ovb0036>* out) {
    UnkStrIdMapBegin_A6_b75(self, out);
}

void fn_710026059c(UnkStrIdMap_A6_b75<UnkReflRecord60_ovb0036>* self,
                   UnkStrIdIterValue_A6_b75<UnkReflRecord60_ovb0036>* out) {
    UnkStrIdMapEnd_A6_b75(self, out);
}

template <u32 Flags>
struct UnkAllocatorHandle_ovb0036 {
    UnkAllocator* allocator = fn_7100083990();
    u32 flags = Flags;
    u32 field_0xc = 0;
};

template <class T>
struct UnkResizedVector_ovb0036 : UnkVector<T> {
    using UnkVector<T>::UnkVector;

    // Resizes to n entries, zeroing the new ones.
    __attribute__((always_inline)) void Resize(u32 n) {
        if (this->count < n) {
            T* p;
            if (this->capacity >= n) {
                p = this->data;
            } else {
                p = static_cast<T*>(this->allocator->vf_0x40((u64)n * sizeof(T), 8, this->flags));
                if (this->data) {
                    __builtin_memcpy(p, this->data, (u64)this->count * sizeof(T));
                    this->allocator->vf_0x48(this->data);
                }
                this->capacity = n;
                this->data = p;
            }
            __builtin_memset(p + this->count, 0, (u64)(n - this->count) * sizeof(T));
        } else if (this->count == n) {
            return;
        }
        this->count = n;
    }
};

extern void* g_unk_7101a20c98[];

// A string id made from a C string (fn_71000003d4).
struct UnkNameId_ovb0036 {
    void* data;
    __attribute__((always_inline)) UnkNameId_ovb0036(const char* text) { fn_71000003d4(this, text, true); }
};

struct UnkNamedFlagObject_ovb0036 {
    void** vtable;
    UnkNameId_ovb0036 name;  // 0x8
    UnkAllocatorHandle_ovb0036<0xe> field_0x10;
    UnkResizedVector_ovb0036<u64> field_0x20;
    u64 field_0x48;
    u64 field_0x50;
    void* field_0x58;

    __attribute__((always_inline)) UnkNamedFlagObject_ovb0036(const char* text, void* value)
        : vtable(g_unk_7101a20c98 + 2), name(text), field_0x20(0xe), field_0x48(0), field_0x50(0) {
        field_0x20.Resize(11);
        field_0x58 = value;
    }
};

void fn_710025bc80(UnkNamedFlagObject_ovb0036* self, const char* text, void* value) {
    new (self) UnkNamedFlagObject_ovb0036(text, value);
}

// Eleven zeroed words with their allocator handle (flags `Flags`).
template <u32 Flags>
struct UnkWordSet_ovb0036 {
    UnkAllocatorHandle_ovb0036<Flags> handle;
    UnkResizedVector_ovb0036<u64> words;
    u64 field_0x38;
    u64 field_0x40;

    __attribute__((always_inline)) UnkWordSet_ovb0036() : words(Flags), field_0x38(0), field_0x40(0) {
        words.Resize(11);
    }
};

template <u32 Flags>
struct UnkWordList_ovb0036 {
    u64 field_0x0 = 0;
    u64 field_0x8 = 0;
    u64 field_0x10 = 0;
    u32 step = 10;
    u32 flags = Flags;
    UnkAllocator* allocator = fn_7100083990();
};

extern void* g_unk_7101a20c78[];

template <u32 Flags>
struct UnkReflValueC8Data_ovb0036 {
    void** vtable;
    UnkWordList_ovb0036<Flags> list;     // 0x8
    UnkWordSet_ovb0036<Flags> set_a;     // 0x30
    bool field_0x78;
    UnkWordSet_ovb0036<0xe> set_b;       // 0x80

    __attribute__((always_inline)) UnkReflValueC8Data_ovb0036() : vtable(g_unk_7101a20c78 + 2), field_0x78(true) {}
};
static_assert(sizeof(UnkReflValueC8Data_ovb0036<5>) == 0xc8, "layout");

void fn_710025b068(char* self) {
    new (self) UnkReflValueC8Data_ovb0036<5>;
}

void fn_7100260718(char* self, void*) {
    new (self) UnkReflValueC8Data_ovb0036<0xe>;
}

// A string id released on destruction (the empty string is left behind).
struct UnkMapKey_ovb0036 {
    UnkStringData* data;

    ~UnkMapKey_ovb0036() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

// The map's 0x60-byte records, destroyed through their vtable.
struct UnkMapRecord_ovb0036 {
    virtual ~UnkMapRecord_ovb0036();
    u64 unk_0x8[0x58 / 8];
};

void fn_7100262f74(UnkMapRecord_ovb0036* self, UnkMapRecord_ovb0036* other);  // move constructor

struct UnkRecordMap_ovb0036 {
    UnkMapRecord_ovb0036* values;
    UnkMapKey_ovb0036* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

UnkMapRecord_ovb0036* fn_71002613ac(UnkRecordMap_ovb0036* self, UnkMapRecord_ovb0036* it) {
    u32 index = static_cast<u32>(it - self->values);
    self->values[index].~UnkMapRecord_ovb0036();
    self->keys[index].~UnkMapKey_ovb0036();
    if (index != self->count - 1) {
        fn_7100262f74(&self->values[index], &self->values[self->count - 1]);
        self->values[self->count - 1].~UnkMapRecord_ovb0036();
        UnkMapKey_ovb0036* keys = self->keys;
        keys[index].data = keys[self->count - 1].data;
        UnkStringData* empty = &g_unk_7101d47108->empty;
        __builtin_memcpy(&keys[self->count - 1].data, &empty, sizeof(empty));
        self->keys[self->count - 1].~UnkMapKey_ovb0036();
    }
    --self->count;
    return &self->values[index];
}

UnkRecordMap_ovb0036* fn_7100260310(UnkRecordMap_ovb0036* self, UnkRecordMap_ovb0036* other) {
    if (self->keys)
        for (u64 i = 0; i < self->count; ++i)
            self->keys[i].~UnkMapKey_ovb0036();
    if (self->values)
        for (u64 i = 0; i < self->count; ++i)
            self->values[i].~UnkMapRecord_ovb0036();
    self->count = 0;
    self->allocator = other->allocator;
    self->keys = other->keys;
    self->values = other->values;
    self->capacity = other->capacity;
    self->count = other->count;
    self->flags = other->flags;
    self->step = other->step;
    other->count = 0;
    other->values = nullptr;
    other->keys = nullptr;
    return self;
}

struct UnkIdVirtualRecordMap;

void fn_7100260164(UnkIdVirtualRecordMap* map) {
    UnkRecordMap_ovb0036* self = reinterpret_cast<UnkRecordMap_ovb0036*>(map);
    if (self->count != self->capacity) {
        UnkMapRecord_ovb0036* values = static_cast<UnkMapRecord_ovb0036*>(
            self->allocator->vf_0x40((u64)self->count * sizeof(UnkMapRecord_ovb0036), 8, self->flags));
        UnkMapKey_ovb0036* keys = static_cast<UnkMapKey_ovb0036*>(
            self->allocator->vf_0x40((u64)self->count * sizeof(UnkMapKey_ovb0036), 8, self->flags));
        if (self->values) {
            UnkMapRecord_ovb0036* src = self->values;
            UnkMapRecord_ovb0036* dst = values;
            for (u32 i = 0; i < self->count; ++i) {
                fn_7100262f74(dst, src);
                UnkMapRecord_ovb0036* old = src++;
                old->~UnkMapRecord_ovb0036();
                ++dst;
            }
            self->allocator->vf_0x48(self->values);
        }
        if (self->keys) {
            UnkMapKey_ovb0036* src = self->keys;
            u64 j = 0;
            for (u32 i = 0; i < self->count; ++i, ++j, ++src) {
                asm("" : "+r"(src));  // the old keys are walked by pointer
                UnkStringData* data = src->data;
                keys[j].data = data;
                if (&g_unk_7101d47108->empty != data)
                    ++data->ref_count;
                src->~UnkMapKey_ovb0036();
            }
            self->allocator->vf_0x48(self->keys);
        }
        self->values = values;
        self->keys = keys;
        self->capacity = self->count;
    }
}

// The record fn_710025bc80 builds, as a temporary (destroyed out of line by fn_7100262dd4).
void fn_710025fdf4(UnkIdVirtualRecordMap* self, u32 capacity);  // Reserve

struct UnkNewRecord_ovb0036 {
    alignas(8) char data[0x60];
    __attribute__((always_inline)) UnkNewRecord_ovb0036(const char* text, void* value) {
        fn_710025bc80(reinterpret_cast<UnkNamedFlagObject_ovb0036*>(this), text, value);
    }
    ~UnkNewRecord_ovb0036() { fn_7100262dd4(data); }
};

struct UnkRecordMapOwner_ovb0036 {
    void* field_0x0;
    UnkRecordMap_ovb0036 map;  // 0x8
};

struct UnkKeyData_ovb0036 {
    u8 pad_0x0[0x10];
    const char* text;  // 0x10
    u8 pad_0x18[0x2c];
    union {
        s32 uses;  // 0x44
    };
};

UnkMapRecord_ovb0036* fn_710025bb90(UnkRecordMapOwner_ovb0036* self, const UnkMapKey_ovb0036* key) {
    ++reinterpret_cast<UnkKeyData_ovb0036*>(key->data)->uses;
    const char* text = reinterpret_cast<UnkKeyData_ovb0036*>(key->data)->text;
    UnkNewRecord_ovb0036 record(text ? text : g_unk_710162e2ed, self);
    UnkRecordMap_ovb0036* map = &self->map;
    if (map->count == map->capacity)
        fn_710025fdf4(reinterpret_cast<UnkIdVirtualRecordMap*>(map), map->step + map->count);
    UnkMapKey_ovb0036* slot = &map->keys[map->count];
    UnkStringData* data = key->data;
    __builtin_memcpy(slot, &data, sizeof(data));
    if (&g_unk_7101d47108->empty != data)
        ++data->ref_count;
    fn_7100262f74(&map->values[map->count], reinterpret_cast<UnkMapRecord_ovb0036*>(&record));
    u32 count = ++map->count;
    return count ? map->values + count - 1 : map->values;
}

struct UnkNodeMap_710025ad0c;
struct UnkNode_710025ad0c;

struct UnkHashedKeyData_ovb0036 {
    u8 pad_0x0[0x30];
    u32 field_0x30;
    u32 field_0x34;
};

// The node's value: a vector, moved in.
struct UnkNodeValue_ovb0036 {
    u64* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

struct UnkKeyNode_ovb0036 {
    UnkStringData* key;
    UnkNodeValue_ovb0036 value;  // 0x8
    UnkKeyNode_ovb0036* prev;   // 0x30
    UnkKeyNode_ovb0036* next;   // 0x38
    UnkKeyNode_ovb0036* chain;  // 0x40

    UnkKeyNode_ovb0036*& Next() { return next; }
    UnkKeyNode_ovb0036*& Prev() { return prev; }
};

struct UnkKeyNodeMap_ovb0036 {
    base::UnkAllocator* allocator;  // 0x0
    u32 flags;                      // 0x8
    u32 count;                      // 0xc
    UnkKeyNode_ovb0036** buckets;   // 0x10
    u32 field_0x18;
    u32 bucket_count;  // 0x1c
    u8 pad_0x20[0x18];
    UnkKeyNode_ovb0036* tail;  // 0x38
    UnkKeyNode_ovb0036* head;  // 0x40
    bool field_0x48;

    static u32 Hash(UnkStringData* k) {
        UnkHashedKeyData_ovb0036* d = reinterpret_cast<UnkHashedKeyData_ovb0036*>(k);
        return d->field_0x30 ^ d->field_0x34;
    }

    __attribute__((always_inline)) void Erase(UnkStringData* k) {
        u32 b = Hash(k) % bucket_count;
        UnkKeyNode_ovb0036** slot = buckets + b;
        UnkKeyNode_ovb0036* n = *slot;
        if (!n)
            return;
        if (n->key != k) {
            UnkKeyNode_ovb0036* before;
            do {
                before = n;
                n = n->chain;
                if (!n)
                    return;
            } while (n->key != k);
            slot = &before->chain;
        }
        *slot = n->chain;
        if (head == n)
            head = n->Prev();
        if (tail == n)
            tail = n->Next();
        if (n->Next())
            n->Next()->Prev() = n->Prev();
        if (n->Prev())
            n->Prev()->Next() = n->Next();
        fn_71002486a8(reinterpret_cast<UnkNodeMap_710025ad0c*>(this), reinterpret_cast<UnkNode_710025ad0c*>(n));
        --count;
    }
};

struct UnkKeyNodeOwner_ovb0036 {
    u8 pad_0x0[0x30];
    UnkKeyNodeMap_ovb0036 map;  // 0x30
};

UnkNodeValue_ovb0036* fn_710025c1a4(UnkKeyNodeOwner_ovb0036* self, UnkStringData* const* key) {
    UnkKeyNodeMap_ovb0036* map = &self->map;
    map->Erase(*key);
    base::UnkAllocator* allocator = fn_7100083990();
    UnkKeyNode_ovb0036** slot = nullptr;
    if (map->field_0x18) {
        u32 b = UnkKeyNodeMap_ovb0036::Hash(*key) % map->bucket_count;
        slot = map->buckets + b;
    }
    UnkKeyNode_ovb0036* old = *slot;
    UnkKeyNode_ovb0036* node =
        static_cast<UnkKeyNode_ovb0036*>(map->allocator->vf_0x40(sizeof(UnkKeyNode_ovb0036), 8, map->flags));
    if (node) {
        UnkStringData* data = *key;
        node->key = data;
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
        node->value.data = nullptr;
        node->value.count = 0;
        node->value.capacity = 0;
        node->value.step = 10;
        node->value.field_0x14 = true;
        node->value.flags = 0xf;
        node->value.allocator = allocator;
        node->prev = nullptr;
        node->next = nullptr;
        node->chain = nullptr;
    }
    node->chain = old;
    *slot = node;
    if (!map->head) {
        map->head = node;
    } else if (map->tail) {
        node->Next() = map->tail;
        map->tail->Prev() = node;
    }
    ++map->count;
    map->tail = node;
    map->field_0x48 = true;
    return node == nullptr ? nullptr : &node->value;
}
