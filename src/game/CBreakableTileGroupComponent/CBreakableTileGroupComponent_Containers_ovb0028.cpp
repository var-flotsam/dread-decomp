// CBreakableTileGroupComponent unit (unit 1167): the container code of its reflected collections (the names
// come from the unit's reflected types, src/game/CBreakableTileGroupComponent/fn_71007618d0.cpp):
// - base::global::CRntVector<STileInfo>: 0x48-byte tile records (three words, a string id, an array of 0x40-byte
//   polymorphic states, a byte) in the engine array; its destructor, clear and move assignment, and the
//   reflected iterator that hands out (index, element) pairs;
// - TActorTileStatesMap: parallel arrays of 16-byte-element arrays and string-id keys; erase, shrink, copy
//   construction and insertion, and the key/value pair its iterators give.
// - the parameter types of the reflected CreateNewTile (fn_710075d430).
// fn_7100760774 (the map's reserve) is defined in src/base/tunable/CTunable/fn_71001358e0.cpp. Names and
// layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game_s2/UnkOwnedList.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

// ---- CRntVector<STileInfo> ----

struct UnkTileState_ovb0028 {
    void** vtable;
    u8 pad_0x8[0x38];

    void vf_0x0() { reinterpret_cast<void (*)(UnkTileState_ovb0028*)>(vtable[0])(this); }
};

struct UnkTileStateArray_ovb0028 {
    UnkTileState_ovb0028* data;     // 0x0
    u32 count;                      // 0x8
    u32 capacity;                   // 0xc
    u32 step;                       // 0x10
    bool growable;                  // 0x14
    u32 flags;                      // 0x18
    base::UnkAllocator* allocator;  // 0x20

    void Free() {
        if (data) {
            for (u64 i = 0; i < count; ++i)
                data[i].vf_0x0();
            UnkTileState_ovb0028* p = data;
            count = 0;
            allocator->vf_0x48(p);
            capacity = 0;
        }
    }
};

// CBreakableTileGroupComponent::STileInfo; Destroy is fn_710075da30 inlined.
struct UnkTileInfo_ovb0028 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    CStrId id;                         // 0x10
    UnkTileStateArray_ovb0028 states;  // 0x18
    u8 field_0x40;

    void Destroy() {
        states.Free();
        id.Reset();
    }
};
static_assert(sizeof(UnkTileInfo_ovb0028) == 0x48, "STileInfo layout");

struct UnkTileInfoVector_ovb0028 {
    UnkTileInfo_ovb0028* data;      // 0x0
    u32 count;                      // 0x8
    u32 capacity;                   // 0xc
    u32 step;                       // 0x10
    bool growable;                  // 0x14
    u32 flags;                      // 0x18
    base::UnkAllocator* allocator;  // 0x20
};

// The vector's destructor.
void fn_71007586b0(UnkTileInfoVector_ovb0028* self) {
    if (self->data) {
        for (u64 i = 0; i < self->count; ++i)
            self->data[i].Destroy();
        UnkTileInfo_ovb0028* p = self->data;
        self->count = 0;
        self->allocator->vf_0x48(p);
        self->capacity = 0;
    }
}

// Clear.
void fn_710075e1f0(UnkTileInfoVector_ovb0028* self) {
    for (u64 i = 0; i < self->count; ++i)
        self->data[i].Destroy();
    self->count = 0;
}

// Move assignment (the old array is not freed, as in base::UnkVector's).
UnkTileInfoVector_ovb0028* fn_710075e508(UnkTileInfoVector_ovb0028* self, UnkTileInfoVector_ovb0028* other) {
    if (self != other) {
        for (u64 i = 0; i < self->count; ++i)
            self->data[i].Destroy();
        self->count = 0;
        self->allocator = other->allocator;
        self->flags = other->flags;
        self->step = other->step;
        self->growable = other->growable;
        self->count = other->count;
        self->capacity = other->capacity;
        self->data = other->data;
        other->data = nullptr;
        other->count = 0;
        other->capacity = 0;
    }
    return self;
}

// The reflected iterator: the position (index and element) and the current pair, either a copied
// element (state 1) or the pair (iterator, element) by reference (state 2). fn_710075ad7c refreshes
// the pair after the position changes.
struct UnkTileInfoIter_ovb0028;
__attribute__((noinline)) void fn_710075ad7c(UnkTileInfoIter_ovb0028* self);

struct UnkTileInfoIter_ovb0028 {
    u32 index;                  // 0x0
    UnkTileInfo_ovb0028* elem;  // 0x8
    union {
        struct {
            void* key;    // 0x10
            void* value;  // 0x18
        } ref;
        struct {
            u64 pad;
            UnkTileInfo_ovb0028 info;  // 0x18
        } own;
    };
    s32 state;  // 0x60

    UnkTileInfoIter_ovb0028(u32 i, UnkTileInfo_ovb0028* e) : index(i), elem(e), state(0) { fn_710075ad7c(this); }
    UnkTileInfoIter_ovb0028& operator=(const UnkTileInfoIter_ovb0028& o) {
        if (this != &o) {
            index = o.index;
            elem = o.elem;
            fn_710075ad7c(this);
        }
        return *this;
    }
    void Reset() {
        if (state == 2) {
            state = 0;
        } else if (state == 1) {
            own.info.Destroy();
            state = 0;
        }
    }
    ~UnkTileInfoIter_ovb0028() { Reset(); }
};

// Without an element the pair is just dropped. The state is read untyped there: the original keeps
// both resets apart instead of hoisting the common state test above the element test.
static inline void UpdatePair(UnkTileInfoIter_ovb0028* self) {
    UnkTileInfo_ovb0028* e = self->elem;
    if (e) {
        self->Reset();
        self->state = 2;
        self->ref.key = self;
        self->ref.value = e;
    } else {
        s32 state;
        __builtin_memcpy(&state, reinterpret_cast<char*>(self) + 0x60, sizeof(state));
        switch (state) {
        case 1:
            self->own.info.Destroy();
        case 2:
            self->state = 0;
        }
    }
}

void fn_710075ad7c(UnkTileInfoIter_ovb0028* self) { UpdatePair(self); }

// The same for the const iterator.
void fn_710075b334(UnkTileInfoIter_ovb0028* self) { UpdatePair(self); }

// begin() and end() into an iterator.
void fn_710075e6c8(UnkTileInfoVector_ovb0028* self, UnkTileInfoIter_ovb0028* out) {
    UnkTileInfoIter_ovb0028 it(0, self->data);
    *out = it;
}

void fn_710075e804(UnkTileInfoVector_ovb0028* self, UnkTileInfoIter_ovb0028* out) {
    UnkTileInfoIter_ovb0028 it(self->count, self->data + self->count);
    *out = it;
}

// ---- TActorTileStatesMap ----

struct UnkTileElem_ovb0028 {
    u64 a;
    u64 b;
};
using UnkTileElemVector_ovb0028 = base::UnkVector<UnkTileElem_ovb0028>;

struct UnkTileStatesMap_ovb0028 {
    UnkTileElemVector_ovb0028* values;  // 0x0
    CStrId* keys;                       // 0x8
    u32 count;                          // 0x10
    u32 capacity;                       // 0x14
    u32 step;                           // 0x18
    u32 flags;                          // 0x1c
    base::UnkAllocator* allocator;      // 0x20
};

template <class V>
struct UnkIdMap;
void fn_7100760774(UnkIdMap<UnkOwnedList>* self, u32 capacity);

static inline void MoveVector(UnkTileElemVector_ovb0028* dst, UnkTileElemVector_ovb0028* src) {
    dst->data = src->data;
    dst->count = src->count;
    dst->capacity = src->capacity;
    dst->step = src->step;
    dst->field_0x14 = src->field_0x14;
    dst->flags = src->flags;
    dst->allocator = src->allocator;
    src->data = nullptr;
    src->count = 0;
    src->capacity = 0;
}

static inline void CopyConstructVector(UnkTileElemVector_ovb0028* dst, const UnkTileElemVector_ovb0028* src) {
    dst->data = nullptr;
    dst->count = 0;
    dst->capacity = 0;
    dst->step = src->step;
    dst->field_0x14 = src->field_0x14;
    dst->flags = src->flags;
    dst->allocator = src->allocator;
    dst->Reserve(src->capacity);
    __builtin_memcpy(dst->data, src->data, (u64)src->count * sizeof(UnkTileElem_ovb0028));
    dst->count = src->count;
}

static inline void CopyAssignVector(UnkTileElemVector_ovb0028* dst, const UnkTileElemVector_ovb0028* src) {
    if (dst == src)
        return;
    dst->Clear();
    dst->Reserve(src->capacity);
    __builtin_memcpy(dst->data, src->data, (u64)src->count * sizeof(UnkTileElem_ovb0028));
    dst->count = src->count;
}

// A new reference to a key, stored bytewise (no alias info: the original reloads the count after it).
static inline void CopyKey(CStrId* dst, const CStrId* src) {
    UnkStringData* d = src->data;
    __builtin_memcpy(&dst->data, &d, sizeof(d));
    if (&g_unk_7101d47108->empty != d)
        ++d->ref_count;
}

// Erase at an entry: the entry's array is freed and its key released, then the last entry moves into the hole.
// Returns the entry now at that position.
UnkTileElemVector_ovb0028* fn_71007616e8(UnkTileStatesMap_ovb0028* self, UnkTileElemVector_ovb0028* it) {
    u32 index = it - self->values;
    self->values[index].Free();
    self->keys[index].Reset();
    u32 last = self->count - 1;
    if (last != index) {
        MoveVector(&self->values[index], &self->values[last]);
        self->values[self->count - 1].Free();
        // The key moves bytewise (no alias info): the original reloads the pool, the count and the keys.
        CStrId* keys = self->keys;
        u32 last_key = self->count - 1;
        __builtin_memcpy(&keys[index].data, &keys[last_key].data, sizeof(UnkStringData*));
        UnkStringData* empty = &g_unk_7101d47108->empty;
        __builtin_memcpy(&keys[last_key].data, &empty, sizeof(empty));
        self->keys[self->count - 1].Reset();
    }
    --self->count;
    return &self->values[index];
}

// Shrinks both arrays to the count.
void fn_7100760bf0(UnkTileStatesMap_ovb0028* self) {
    if (self->count == self->capacity)
        return;
    auto* values = static_cast<UnkTileElemVector_ovb0028*>(
        self->allocator->vf_0x40((u64)self->count * sizeof(UnkTileElemVector_ovb0028), 8, self->flags));
    auto* keys = static_cast<CStrId*>(self->allocator->vf_0x40((u64)self->count * sizeof(CStrId), 8, self->flags));
    if (UnkTileElemVector_ovb0028* src = self->values) {
        UnkTileElemVector_ovb0028* dst = values;
        for (u32 i = 0; i < self->count; ++i, ++src, ++dst) {
            MoveVector(dst, src);
            src->Free();
        }
        self->allocator->vf_0x48(self->values);
    }
    if (CStrId* src = self->keys) {
        CStrId* dst = keys;
        for (u32 i = 0; i < self->count; ++i, ++src, ++dst) {
            new (dst) CStrId(*src);
            src->Reset();
        }
        self->allocator->vf_0x48(self->keys);
    }
    self->values = values;
    self->keys = keys;
    self->capacity = self->count;
}

// Copy constructor.
void fn_71007605c4(UnkTileStatesMap_ovb0028* self, const UnkTileStatesMap_ovb0028* other) {
    self->values = nullptr;
    self->keys = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->flags = other->flags;
    self->allocator = other->allocator;
    fn_7100760774(reinterpret_cast<UnkIdMap<UnkOwnedList>*>(self), other->count);
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i)
        CopyConstructVector(&self->values[i], &other->values[i]);
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i)
        CopyKey(&self->keys[i], &other->keys[i]);
}

// Appends a copy of (key, value), growing by the step when full; returns the new entry.
UnkTileElemVector_ovb0028* fn_7100760da8(UnkTileStatesMap_ovb0028* self, const CStrId* key,
                                         const UnkTileElemVector_ovb0028* value) {
    if (self->count == self->capacity)
        fn_7100760774(reinterpret_cast<UnkIdMap<UnkOwnedList>*>(self), self->count + self->step);
    CopyKey(&self->keys[self->count], key);
    CopyConstructVector(&self->values[self->count], value);
    ++self->count;
    return self->count ? &self->values[self->count] - 1 : self->values;
}

// The map's key/value pair as its reflected iterators give it: the key and value themselves (state 1)
// or pointers to the map's own (state 2).
struct UnkTileStatesPair_ovb0028 {
    struct Own {
        CStrId key;
        UnkTileElemVector_ovb0028 value;
    };
    struct Ref {
        CStrId* key;
        UnkTileElemVector_ovb0028* value;
    };
    union {
        Own own;  // state 1
        Ref ref;  // state 2
    };
    s32 state;  // 0x30

    CStrId* Key() { return state == 2 ? ref.key : &own.key; }
    UnkTileElemVector_ovb0028* Value() { return state == 2 ? ref.value : &own.value; }
};

static inline void ConstructPair(UnkTileStatesPair_ovb0028* self, CStrId* key, const UnkTileElemVector_ovb0028* value) {
    self->own.key.data = key->data;
    key->data = &g_unk_7101d47108->empty;
    CopyConstructVector(&self->own.value, value);
}

// Construction from another pair: the key is moved, the value copied; the result owns both.
void fn_710075fe30(UnkTileStatesPair_ovb0028* self, UnkTileStatesPair_ovb0028* other) {
    self->state = 0;
    if (other->state == 1) {
        self->state = 1;
        ConstructPair(self, &other->own.key, &other->own.value);
    } else if (other->state == 2) {
        self->state = 1;
        ConstructPair(self, other->ref.key, other->ref.value);
    }
}

// Copy assignment.
UnkTileStatesPair_ovb0028* fn_710075f8f0(UnkTileStatesPair_ovb0028* self, UnkTileStatesPair_ovb0028* other) {
    if (other != self) {
        const CStrId* key = other->Key();
        *self->Key() = *key;
        const UnkTileElemVector_ovb0028* value = other->Value();
        CopyAssignVector(self->Value(), value);
    }
    return self;
}

// The key move of the move assignment: the id is released like CStrId::ReleaseAndTake, then the other's
// is taken bytewise (no alias info: the original reloads the pair's state after it).
static inline void MoveKey(CStrId* dst, CStrId* src) {
    if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
        if (dst->data && dst->data != &pool->empty) {
            --dst->data->ref_count;
            if (dst->data->ref_count == 0) {
                fn_71000803a4(pool, dst->data);
                pool = g_unk_7101d47108;
            }
        }
        dst->data = &pool->empty;
    }
    __builtin_memcpy(&dst->data, &src->data, sizeof(src->data));
    src->data = &g_unk_7101d47108->empty;
}

// Move assignment.
UnkTileStatesPair_ovb0028* fn_710075faac(UnkTileStatesPair_ovb0028* self, UnkTileStatesPair_ovb0028* other) {
    if (other->state == 1) {
        MoveKey(self->Key(), &other->own.key);
        *self->Value() = static_cast<UnkTileElemVector_ovb0028&&>(other->own.value);
    } else if (other->state == 2) {
        CStrId* key = other->ref.key;
        MoveKey(self->Key(), key);
        *self->Value() = static_cast<UnkTileElemVector_ovb0028&&>(*other->ref.value);
    }
    return self;
}

// ---- Reflection ----

extern base::reflection::CType g_unk_7172795ca8;
void fn_710075d748(base::reflection::UnkReflFunction* fn);

// CreateNewTile's parameter types (uIdx, eTileType, then the rest from fn_710075d748).
void fn_710075d430(base::reflection::UnkReflFunction* fn) {
    fn->params.PushBack(fn_7100005178());
    fn->params.PushBack(&g_unk_7172795ca8);
    fn_710075d748(fn);
}
