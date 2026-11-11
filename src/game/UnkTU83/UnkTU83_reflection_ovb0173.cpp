// Unit 83 (tools/tu_map.py; base::reflection): reflected collection slots of keyed arrays, string-id vectors
// and string vectors: array equality, move and copy assignment, element erase, grow-and-set, Reserve, and the
// Begin/End iterator values. Names and layouts are placeholders.
#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))
#include <new>
#include <utility>

#include "base/UnkCloneRecords.h"
#include "base/UnkDestructors.h"
#include "base/UnkKeyedStrIds.h"
#include "base/UnkResPathVec.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTU83.h"
#include "game/UnkTU85.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;

// ---- Keyed arrays (values at +0, u64 keys at +8, count at +0x10): equality ----

template <class V>
struct UnkKeyedVals_ovb0173 {
    V* values;
    u64* keys;
    u32 count;

    V* Find(const u64& key) const {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return values + count;
    }
};

// Equal when the counts match and every key of the first is in the second with an equal value (the
// second is read through a copy: the original loads its fields before the first's keys).
template <class V>
static inline __attribute__((always_inline)) bool KeyedEqual(const UnkKeyedVals_ovb0173<V>* a,
                                                              const UnkKeyedVals_ovb0173<V>* b) {
    if (a->count != b->count)
        return false;
    UnkKeyedVals_ovb0173<V> bb = *b;
    for (u32 i = 0; i < a->count; ++i) {
        V* v = bb.Find(a->keys[i]);
        if (v == bb.values + bb.count)
            return false;
        if (*v != a->values[i])
            return false;
    }
    return true;
}

bool fn_71000def10(const UnkKeyedVals_ovb0173<f32>* a, const UnkKeyedVals_ovb0173<f32>* b) {
    return KeyedEqual(a, b);
}
bool fn_71000d94dc(const UnkKeyedVals_ovb0173<u64>* a, const UnkKeyedVals_ovb0173<u64>* b) {
    return KeyedEqual(a, b);
}
bool fn_71000daf10(const UnkKeyedVals_ovb0173<u64>* a, const UnkKeyedVals_ovb0173<u64>* b) {
    return KeyedEqual(a, b);
}

// ---- Arrays of strings (UnkVector's layout, 0x30-byte CRntString elements) ----

static inline void SwapVec(base::UnkVector<CRntString>& a, base::UnkVector<CRntString>& b) {
    std::swap(a.data, b.data);
    std::swap(a.count, b.count);
    std::swap(a.capacity, b.capacity);
    std::swap(a.step, b.step);
    std::swap(a.field_0x14, b.field_0x14);
    std::swap(a.flags, b.flags);
    std::swap(a.allocator, b.allocator);
}

// A one-element string array in the making (flags 0x59), freed when the assignment is done.
struct UnkTmpStrVec_ovb0173 {
    base::UnkVector<CRntString> v{0x59};
    ~UnkTmpStrVec_ovb0173() { v.Free(); }
};

// The move assignment: the other's fields are taken (the other left empty), the old array released.
struct UnkRntStringVec_ovb0173 {
    CRntString* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkRntStringVec_ovb0173(UnkRntStringVec_ovb0173&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
    ~UnkRntStringVec_ovb0173() { reinterpret_cast<base::UnkVector<CRntString>*>(this)->Free(); }

    UnkRntStringVec_ovb0173& operator=(UnkRntStringVec_ovb0173&& other) {
        if (this != &other) {
            UnkRntStringVec_ovb0173 tmp(std::move(other));
            std::swap(data, tmp.data);
            std::swap(count, tmp.count);
            std::swap(capacity, tmp.capacity);
            std::swap(step, tmp.step);
            std::swap(field_0x14, tmp.field_0x14);
            std::swap(flags, tmp.flags);
            std::swap(allocator, tmp.allocator);
        }
        return *this;
    }
};

__attribute__((flatten)) void* fn_71000e22ec(void* dst, void* src) {
    return &(*static_cast<UnkRntStringVec_ovb0173*>(dst) =
                 static_cast<UnkRntStringVec_ovb0173&&>(*static_cast<UnkRntStringVec_ovb0173*>(src)));
}

typedef u32 UnkAliasU32_ovb0173 __attribute__((may_alias));

struct UnkStringHolder_ovb0173 {
    u8 pad_0x0[0x10];
    CRntString str;  // +0x10
    u8 pad_after[0x44 - 0x10 - sizeof(CRntString)];
    UnkAliasU32_ovb0173 ref_count;  // +0x44
};

// Assigns the array a single copy of the string a refcounted holder keeps (the count is retained).
__attribute__((flatten)) base::UnkVector<CRntString>* fn_71000e26f8(base::UnkVector<CRntString>* self,
                                                                    UnkStringHolder_ovb0173** holder) {
    if (reinterpret_cast<void*>(holder) != reinterpret_cast<void*>(self)) {
        UnkTmpStrVec_ovb0173 guard;
        base::UnkVector<CRntString>& tmp = guard.v;
        ++*reinterpret_cast<UnkAliasU32_ovb0173*>(&(*holder)->ref_count);
        tmp.PushBackCopy((*holder)->str);
        SwapVec(*self, tmp);
    }
    return self;
}

// Assigns the array a single string built from a literal.
__attribute__((flatten)) base::UnkVector<CRntString>* fn_71000e23ac(base::UnkVector<CRntString>* self,
                                                                    const char** src) {
    if (reinterpret_cast<void*>(src) != reinterpret_cast<void*>(self)) {
        const char* s = *src;
        UnkTmpStrVec_ovb0173 guard;
        base::UnkVector<CRntString>& tmp = guard.v;
        {
            CRntString str(s);
            tmp.EmplaceBack(static_cast<CRntString&&>(str));
        }
        SwapVec(*self, tmp);
    }
    return self;
}

// ---- Byte arrays: set the element at an index, growing the array first ----

void fn_71000e1518(base::UnkVector<u8>* self, const u32* index, const u8* value) {
    u8 v = *value;
    self->GrowAt(*index) = v;
}

// An identical copy.
void fn_71000e1660(base::UnkVector<u8>* self, const u32* index, const u8* value) {
    u8 v = *value;
    self->GrowAt(*index) = v;
}

// ---- Vectors of string ids and file-path ids: copy assignment ----

using UnkIdMember_ovb0173 = base::UnkStrIdMember;
static inline void DestroyId(UnkIdMember_ovb0173* p) { p->~UnkIdMember_ovb0173(); }

base::UnkStrIdVec* fn_71000ce19c(base::UnkStrIdVec* self, const base::UnkStrIdVec* other) {
    if (self != other) {
        for (u32 i = 0; i < self->count; ++i)
            DestroyId(reinterpret_cast<base::UnkStrIdMember*>(&self->data[i]));
        self->count = 0;
        self->Reserve(other->capacity);
        for (const CStrId *p = other->data, *e = p + other->count; p != e; ++p)
            self->PushBack(*p);
    }
    return self;
}

using UnkPathIdMember_ovb0173 = base::UnkStrIdMemberOf<g_unk_7101d47200>;
static inline void DestroyId(UnkPathIdMember_ovb0173* p) { p->~UnkPathIdMember_ovb0173(); }

UnkResPathVec* fn_71000d0958(UnkResPathVec* self, const UnkResPathVec* other) {
    if (self != other) {
        for (u32 i = 0; i < self->count; ++i)
            DestroyId(reinterpret_cast<UnkPathIdMember_ovb0173*>(&self->data[i]));
        self->count = 0;
        fn_7100018450(self, other->capacity);
        for (const CFilePathStrId *p = other->data, *e = p + other->count; p != e; ++p) {
            u32 n = self->count;
            u32 c = self->capacity;
            if (c <= n && self->step != 0)
                c = (n / self->step + 1) * self->step;
            if ((s32)n >= (s32)c)
                for (;;) {
                }
            fn_7100018450(self, c);
            CFilePathStrId* slot = &self->data[self->count];
            base::global::UnkStringData* d = p->data;
            __builtin_memcpy(slot, &d, 8);
            if (&g_unk_7101d47200->empty != d)
                ++d->ref_count;
            ++self->count;
        }
    }
    return self;
}

// ---- Keyed arrays of string ids ----

// A string id taken over bitwise by a move (the moved-from id left empty), released in two steps.
struct UnkMovedId_ovb0173 : base::global::UnkStrIdBase {
    __attribute__((always_inline)) UnkMovedId_ovb0173(UnkMovedId_ovb0173&& other) : UnkStrIdBase(other) {
        base::global::UnkStringData* empty = &g_unk_7101d47108->empty;
        __builtin_memcpy(&other.data, &empty, sizeof(empty));
    }
    ~UnkMovedId_ovb0173() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
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

// Grows an array of u32 values with string ids to `capacity` entries.
void fn_71000df22c(UnkKeyedStrIds* self, u32 capacity) {
    if (self->capacity < capacity) {
        u32* values = static_cast<u32*>(self->allocator->vf_0x40((u64)capacity * sizeof(u32), 8, self->field_0x1c));
        auto* ids = static_cast<UnkMovedId_ovb0173*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(UnkMovedId_ovb0173), 8, self->field_0x1c));
        if (self->keys) {
            __builtin_memcpy(values, self->keys, (u64)self->count * sizeof(u32));
            self->allocator->vf_0x48(self->keys);
        }
        if (auto* old = reinterpret_cast<UnkMovedId_ovb0173*>(self->ids)) {
            u64 j = 0;
            for (u32 i = 0; i < self->count; ++i, ++j, ++old) {
                new (&ids[j]) UnkMovedId_ovb0173(static_cast<UnkMovedId_ovb0173&&>(*old));
                old->~UnkMovedId_ovb0173();
            }
            self->allocator->vf_0x48(self->ids);
        }
        self->keys = values;
        self->ids = reinterpret_cast<base::UnkStrIdMember*>(ids);
        self->capacity = capacity;
    }
}

template <class V>
struct UnkStrIdTable;
void fn_71000df5ec(UnkStrIdTable<u32>* self);

// A new reference to an id, stored bytewise (no alias info: the original reloads after it).
static inline void CopyId(base::global::UnkStringData** dst, base::global::UnkStringData* const* src) {
    base::global::UnkStringData* d = *src;
    __builtin_memcpy(dst, &d, sizeof(d));
    if (&g_unk_7101d47108->empty != d)
        ++d->ref_count;
}

// The copy assignment.
UnkKeyedStrIds* fn_71000df484(UnkKeyedStrIds* self, const UnkKeyedStrIds* other) {
    if (self != other) {
        if (self->ids) {
            for (u32 i = 0; i < self->count; ++i)
                reinterpret_cast<UnkMovedId_ovb0173*>(&self->ids[i])->~UnkMovedId_ovb0173();
        }
        self->count = 0;
        if (other->count != self->capacity) {
            fn_71000df5ec(reinterpret_cast<UnkStrIdTable<u32>*>(self));
            fn_71000df22c(self, other->count);
        }
        __builtin_memcpy(self->keys, other->keys, (u64)other->count * sizeof(u32));
        self->count = other->count;
        self->count = other->count;
        for (u64 i = 0; i < self->count; ++i)
            CopyId(reinterpret_cast<base::global::UnkStringData**>(&self->ids[i]),
                   reinterpret_cast<base::global::UnkStringData* const*>(&other->ids[i]));
    }
    return self;
}

// Erase at an iterator over 4-byte values with string-id keys: the last entry moves into its place.
struct UnkIdVals_ovb0173 {
    u32* values;
    UnkMovedId_ovb0173* keys;
    u32 count;
};

u32* fn_71000df750(UnkIdVals_ovb0173* self, u32* it) {
    u64 pos = (u64)((char*)it - (char*)self->values) >> 2;
    self->keys[(u32)pos].~UnkMovedId_ovb0173();
    u32 last = self->count - 1;
    if (last != (u32)pos) {
        self->values[(u32)pos] = self->values[last];
        new (&self->keys[(u32)pos])
            UnkMovedId_ovb0173(static_cast<UnkMovedId_ovb0173&&>(self->keys[self->count - 1]));
        self->keys[self->count - 1].~UnkMovedId_ovb0173();
    }
    self->count = self->count - 1;
    return self->values + (u32)pos;
}

// ---- Keyed array of 0xb0-byte records (as src/base/reflection/UnkReflKeyedB0_b44.cpp) ----

struct UnkRecordB0_b44a1;
// As src/game_s1/fn_71000dbc34.cpp declares it.
void fn_7100097d18(void* self, void* other);

struct UnkRecordB0_b44a1 {
    u64 words[0xb0 / 8];

    UnkRecordB0_b44a1(UnkRecordB0_b44a1&& other) { fn_7100097d18(this, &other); }
    ~UnkRecordB0_b44a1() { fn_71000e70c4(this); }
};

struct UnkRecIds_ovb0173 {
    UnkRecordB0_b44a1* values;
    UnkMovedId_ovb0173* keys;
    u32 count;
};

// Erase at an iterator: the last record and id move into its place.
UnkRecordB0_b44a1* fn_71000dcebc(UnkRecIds_ovb0173* self, UnkRecordB0_b44a1* it) {
    u32 i = it - self->values;
    self->values[i].~UnkRecordB0_b44a1();
    self->keys[i].~UnkMovedId_ovb0173();
    u32 last = self->count - 1;
    if (last != i) {
        new (&self->values[i]) UnkRecordB0_b44a1(static_cast<UnkRecordB0_b44a1&&>(self->values[last]));
        self->values[self->count - 1].~UnkRecordB0_b44a1();
        new (&self->keys[i]) UnkMovedId_ovb0173(static_cast<UnkMovedId_ovb0173&&>(self->keys[self->count - 1]));
        self->keys[self->count - 1].~UnkMovedId_ovb0173();
    }
    self->count = self->count - 1;
    return self->values + i;
}

// ---- A file-path id assigned from an object that starts with its text ----

struct UnkCStrHolder_ovb0173 {
    const char* str;
};

CFilePathStrId* fn_71000cd338(CFilePathStrId* self, const UnkCStrHolder_ovb0173* other) {
    if (other != static_cast<const void*>(self)) {
        CFilePathStrId id(base::global::UnkStrIdInline(), other->str, true);
        *self = static_cast<CFilePathStrId&&>(id);
    }
    return self;
}

// ---- A tree node and its children (+0x90, count at +0x98) appended to a list ----

struct UnkTreeNode_ovb0173 {
    u8 pad_0x0[0x90];
    UnkTreeNode_ovb0173** children;  // 0x90
    u32 child_count;                 // 0x98
};

void fn_71000a5020(UnkTreeNode_ovb0173* node, base::UnkVector<UnkTreeNode_ovb0173*>* out) {
    out->PushBack(node);
    for (UnkTreeNode_ovb0173 **p = node->children, **e = p + node->child_count; p != e; ++p)
        fn_71000a5020(*p, out);
}

// ---- Begin and End of keyed arrays (values at +0, string-id keys at +8, count at +0x10) ----

// An iterator value (state 0 empty, 1 owns, 2 refers) that a keyed collection's Begin and End slots
// build as a temporary and take over bitwise.
template <class T, class K>
struct UnkMoveIter_ovb0173 {
    struct Value {
        K index;
        T element;
    };
    struct Ref {
        const K* index;
        T* element;
    };
    union {
        Value value;
        Ref ref;
    };
    s32 state;

    UnkMoveIter_ovb0173(const K* k, T* e) : state(2) {
        ref.index = k;
        ref.element = e;
    }
    ~UnkMoveIter_ovb0173() { Reset(); }
    void Reset() {
        if (state == 2) {
        } else if (state == 1) {
            value.~Value();
        } else {
            return;
        }
        state = 0;
    }
    // The owned value is released without clearing the state (it is overwritten right after).
    UnkMoveIter_ovb0173& operator=(UnkMoveIter_ovb0173&& other) {
        if (state == 1) {
            value.~Value();
        } else if (state == 2) {
            state = 0;
        }
        state = other.state;
        __builtin_memcpy(&ref, &other.ref, sizeof(ref));
        return *this;
    }
};

template <class T, class K>
struct UnkKeyedArr_ovb0173 {
    T* values;
    K* keys;
    u32 count;

    using Iter = UnkMoveIter_ovb0173<T, K>;
    __attribute__((always_inline)) void Begin(Iter* out) { *out = Iter(keys, values); }
    __attribute__((always_inline)) void End(Iter* out) { *out = Iter(keys + count, values + count); }
};

// Reflected values (fn_71000a1c60).
struct UnkReflValueElem_ovb0173 {
    base::reflection::UnkReflValue v;
    ~UnkReflValueElem_ovb0173() { fn_71000a1c60(&v); }
};
using UnkValArr_ovb0173 = UnkKeyedArr_ovb0173<UnkReflValueElem_ovb0173, base::UnkStrIdMember>;
void fn_71000d8590(UnkValArr_ovb0173* self, UnkValArr_ovb0173::Iter* out) { self->Begin(out); }
void fn_71000d86a4(UnkValArr_ovb0173* self, UnkValArr_ovb0173::Iter* out) { self->End(out); }

// 0xb0-byte records.
using UnkRecArr_ovb0173 = UnkKeyedArr_ovb0173<UnkRecordB0_b44a1, base::UnkStrIdMember>;
void fn_71000dd04c(UnkRecArr_ovb0173* self, UnkRecArr_ovb0173::Iter* out) { self->Begin(out); }
void fn_71000dd160(UnkRecArr_ovb0173* self, UnkRecArr_ovb0173::Iter* out) { self->End(out); }

// 0xb8-byte function data (destroyed by fn_710009aae8).
struct UnkFuncData_ovb0173 {
    u64 words[0xb8 / 8];
    ~UnkFuncData_ovb0173() { fn_710009aae8(this); }
};
using UnkFuncArr_ovb0173 = UnkKeyedArr_ovb0173<UnkFuncData_ovb0173, base::UnkStrIdMember>;
void fn_71000de178(UnkFuncArr_ovb0173* self, UnkFuncArr_ovb0173::Iter* out) { self->Begin(out); }
void fn_71000de28c(UnkFuncArr_ovb0173* self, UnkFuncArr_ovb0173::Iter* out) { self->End(out); }
