// CGameBlackboard's unit (tools/tu_map.py): the game blackboard's two maps of string ids and their
// containers. After CBlackboard's 0xf0 bytes it keeps, at +0xf0, owned id sets keyed by section
// (each a hash set of ids; destroyed by fn_7100aef674 and freed) and, at +0x118, tables of floats
// keyed by ids (UnkKeyedStrIds' layout with the floats as its keys) keyed by ids. Here: the
// blackboard's destructor, its section removal and the "set unless already present" step, the
// handle assignment from an id, and the maps' Reserve, erase, push-back, copy constructor,
// set-by-key move and iterator-value move. Names and layouts are placeholders.
#include <new>

#include "base/UnkCloneHelpers.h"
#include "base/UnkCollectionIter.h"
#include "base/UnkKeyedStrIds.h"
#include "base/UnkParallelFind.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "types.h"

using namespace base::global;

void fn_7100aef674(void* self);
template <class Value>
struct UnkIdRecordTable;
template <class T, void (*Destroy)(T*)>
struct UnkOwned;
template <class V>
struct UnkIdMap;
void fn_7100aecdc4(UnkIdRecordTable<UnkOwned<void, fn_7100aef674>>* self);
void fn_7100aee918(UnkIdMap<UnkKeyedStrIds>* self, u32 capacity);
void fn_71000df22c(UnkKeyedStrIds* self, u32 capacity);
void fn_71000a1760(base::reflection::UnkTypedRef* out, const base::reflection::UnkTypedRef* in);
CBlackboard::CSection** fn_71010482d4(CBlackboard* self, CBlackboard::CSection** slot);
void fn_7101046ac0(CBlackboard* self);
extern void* g_unk_7101a978b8[];

// A string id whose destructor decrements and tests in two steps (the original reloads the id
// after the decrement). Its move stores the pointer bytewise, so the moved-from id's release
// reloads the pool.
struct UnkReleasedId_ovb0064 {
    UnkStringData* data;

    UnkReleasedId_ovb0064(UnkReleasedId_ovb0064&& other) {
        UnkStringData* d = other.data;
        __builtin_memcpy(&data, &d, sizeof(d));
        other.data = &g_unk_7101d47108->empty;
    }
    ~UnkReleasedId_ovb0064() {
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

// A new reference to an id, stored bytewise (no alias info: the original reloads after it).
static inline void CopyId(UnkStringData** dst, UnkStringData* const* src) {
    UnkStringData* d = *src;
    __builtin_memcpy(dst, &d, sizeof(d));
    if (&g_unk_7101d47108->empty != d)
        ++d->ref_count;
}

// Values at 0 and the ids that name them at 8, sharing a count, a capacity, a growth step, the
// allocation flags and the allocator.
template <class V>
struct UnkIdMap_ovb0064 {
    V* values;
    UnkReleasedId_ovb0064* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

// Grows both arrays, moving the values (destroying the old ones with Destroy) and the keys.
template <class V, void (*Destroy)(V*)>
static inline void Reserve(UnkIdMap_ovb0064<V>* self, u32 capacity) {
    if (self->capacity < capacity) {
        V* values = static_cast<V*>(self->allocator->vf_0x40((u64)capacity * sizeof(V), 8, self->flags));
        auto* keys = static_cast<UnkReleasedId_ovb0064*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(UnkReleasedId_ovb0064), 8, self->flags));
        if (V* src = self->values) {
            V* dst = values;
            for (u32 i = 0; i < self->count; ++i, ++src, ++dst) {
                new (dst) V(static_cast<V&&>(*src));
                Destroy(src);
            }
            self->allocator->vf_0x48(self->values);
        }
        if (UnkReleasedId_ovb0064* src = self->keys) {
            UnkReleasedId_ovb0064* dst = keys;
            for (u32 i = 0; i < self->count; ++i, ++src, ++dst) {
                new (dst) UnkReleasedId_ovb0064(static_cast<UnkReleasedId_ovb0064&&>(*src));
                src->~UnkReleasedId_ovb0064();
            }
            self->allocator->vf_0x48(self->keys);
        }
        self->values = values;
        self->keys = keys;
        self->capacity = capacity;
    }
}

// ---- The owned id sets (+0xf0) ----

// A chained hash set of ids: the bucket is picked by the id's hash halves.
struct UnkStrIdHash_ovb0064 {
    u8 pad_0x0[0x30];
    u32 hash_lo;
    u32 hash_hi;
};

struct UnkIdSetNode_ovb0064 {
    UnkStringData* key;
    u8 pad_0x8[0x18];
    UnkIdSetNode_ovb0064* next;
};

struct UnkIdSet_ovb0064 {
    u8 pad_0x0[0x10];
    UnkIdSetNode_ovb0064** buckets;
    u32 count;
    u32 modulus;

    bool Contains(const CStrId& key) {
        if (!count)
            return false;
        const UnkStrIdHash_ovb0064* h = reinterpret_cast<const UnkStrIdHash_ovb0064*>(key.data);
        u32 i = (h->hash_lo ^ h->hash_hi) % modulus;
        UnkIdSetNode_ovb0064** bucket = buckets + i;
        if (!bucket)
            return false;
        for (UnkIdSetNode_ovb0064* n = *bucket; n; n = n->next)
            if (n->key == key.data)
                return true;
        return false;
    }
};

// The sets by section id, as base/UnkParallelFind.h sees them.
struct UnkIdSetMap_ovb0064 {
    UnkIdSet_ovb0064** values;
    CStrId* ids;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

// An owning pointer to a set; the move stores bytewise (the original reloads the moved-from slot).
struct UnkOwnedSet_ovb0064 {
    void* ptr;

    UnkOwnedSet_ovb0064(UnkOwnedSet_ovb0064&& other) {
        void* p = other.ptr;
        other.ptr = nullptr;
        __builtin_memcpy(&ptr, &p, sizeof(p));
    }
};

static void DestroyOwned(UnkOwnedSet_ovb0064* self) {
    void* p = self->ptr;
    self->ptr = nullptr;
    if (p) {
        fn_7100aef674(p);
        operator delete(p);
    }
}

// ---- The float tables (+0x118) ----

// Floats keyed by string ids (UnkKeyedStrIds' layout, the floats as its keys). The copy
// reserves through fn_71000df22c, copies the floats as a block and counts a use of each id.
struct UnkFloatsById_ovb0064 {
    f32* values;
    UnkStringData** ids;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkFloatsById_ovb0064(const UnkFloatsById_ovb0064& other) {
        ids = nullptr;
        count = 0;
        capacity = 0;
        values = nullptr;
        step = other.step;
        flags = other.flags;
        allocator = other.allocator;
        fn_71000df22c(reinterpret_cast<UnkKeyedStrIds*>(this), other.count);
        __builtin_memcpy(values, other.values, (u64)other.count * sizeof(f32));
        count = other.count;
        count = other.count;
        for (u64 i = 0; i < count; ++i)
            CopyId(&ids[i], &other.ids[i]);
    }
};

struct UnkFloatsByIdMap_ovb0064 {
    UnkFloatsById_ovb0064* values;
    UnkStringData** keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

struct UnkGameBlackboard_ovb0064 : CBlackboard {
    UnkIdSetMap_ovb0064 sets;                  // 0xf0
    UnkIdMap_ovb0064<UnkKeyedStrIds> tables;  // 0x118
};
static_assert(sizeof(UnkGameBlackboard_ovb0064) == 0x140, "UnkGameBlackboard_ovb0064 layout");

// Erases the set in `slot` (the last one moves into its place; fn_7100aef4ec, not matched yet).
UnkIdSet_ovb0064** fn_7100aef4ec(UnkIdSetMap_ovb0064* self, UnkIdSet_ovb0064** slot);

// The destructor: the float tables are cleared and freed, the sets cleared (fn_7100aecdc4) and
// freed, then CBlackboard's destructor runs.
void fn_7100aebde8(UnkGameBlackboard_ovb0064* self) {
    *reinterpret_cast<void***>(self) = &g_unk_7101a978b8[2];
    UnkIdMap_ovb0064<UnkKeyedStrIds>& t = self->tables;
    if (t.keys && t.count) {
        for (u64 i = 0; i < t.count; ++i)
            t.keys[i].~UnkReleasedId_ovb0064();
    }
    if (t.values && t.count) {
        for (u64 i = 0; i < t.count; ++i)
            fn_71000df388(&t.values[i]);
    }
    t.count = 0;
    if (t.keys)
        t.allocator->vf_0x48(t.keys);
    if (t.values)
        t.allocator->vf_0x48(t.values);
    UnkIdSetMap_ovb0064& s = self->sets;
    fn_7100aecdc4(reinterpret_cast<UnkIdRecordTable<UnkOwned<void, fn_7100aef674>>*>(&s));
    if (s.ids)
        s.allocator->vf_0x48(s.ids);
    if (s.values)
        s.allocator->vf_0x48(s.values);
    fn_7101046ac0(self);
}

// Removes the section in `slot`: its set is erased first, then CBlackboard's removal runs.
CBlackboard::CSection** fn_7100aebc34(UnkGameBlackboard_ovb0064* self, CBlackboard::CSection** slot) {
    UnkIdSet_ovb0064** set = UnkParallelFindSlot(&self->sets, (*slot)->key);
    if (set != &self->sets.values[self->sets.count])
        fn_7100aef4ec(&self->sets, set);
    return fn_71010482d4(self, slot);
}

// Sets `name` in the section `key` (fn_7100aeb78c) unless the section's set already holds it. The
// value is passed on by value (copied by fn_71000a1760); the callee does not read it.
void fn_7100aeb78c(UnkGameBlackboard_ovb0064* self, const CStrId& key, const CStrId& name);

void fn_7100aebccc(UnkGameBlackboard_ovb0064* self, const CStrId& key, const CStrId& name,
                   const base::reflection::UnkTypedRef* value) {
    UnkIdSet_ovb0064** set = UnkParallelFindSlot(&self->sets, key);
    if (set != &self->sets.values[self->sets.count] && (*set)->Contains(name))
        return;
    base::reflection::UnkTypedRef copy;
    fn_71000a1760(&copy, value);
    fn_7100aeb78c(self, key, name);
}

// Grows the owned sets' map.
void fn_7100aef764(UnkIdMap_ovb0064<UnkOwnedSet_ovb0064>* self, u32 capacity) {
    Reserve<UnkOwnedSet_ovb0064, DestroyOwned>(self, capacity);
}

// Erases the float table in `slot`: it is destroyed and its id released, then the last entry
// moves into its place. Returns the slot. The id moves bytewise (the original reloads after it).
UnkKeyedStrIds* fn_7100aef1d8(UnkIdMap_ovb0064<UnkKeyedStrIds>* self, UnkKeyedStrIds* slot) {
    u32 i = slot - self->values;
    fn_71000df388(&self->values[i]);
    self->keys[i].~UnkReleasedId_ovb0064();
    u32 last = self->count - 1;
    if (last != i) {
        new (&self->values[i]) UnkKeyedStrIds(static_cast<UnkKeyedStrIds&&>(self->values[last]));
        fn_71000df388(&self->values[self->count - 1]);
        UnkReleasedId_ovb0064* keys = self->keys;
        last = self->count - 1;
        __builtin_memcpy(&keys[i], &keys[last], sizeof(UnkReleasedId_ovb0064));
        UnkStringData* empty = &g_unk_7101d47108->empty;
        __builtin_memcpy(&keys[last], &empty, sizeof(empty));
        self->keys[self->count - 1].~UnkReleasedId_ovb0064();
    }
    --self->count;
    return &self->values[i];
}

// Adds a copy of `value` under `key` (growing by the step first when full); returns the new entry.
UnkFloatsById_ovb0064* fn_7100aef070(UnkFloatsByIdMap_ovb0064* self, const CStrId& key,
                                     const UnkFloatsById_ovb0064& value) {
    if (self->count == self->capacity)
        fn_7100aee918(reinterpret_cast<UnkIdMap<UnkKeyedStrIds>*>(self), self->count + self->step);
    {
        UnkStringData** dst = &self->keys[self->count];
        UnkStringData* d = key.data;
        __builtin_memcpy(dst, &d, sizeof(d));
        if (&g_unk_7101d47108->empty != d)
            ++d->ref_count;
    }
    new (&self->values[self->count]) UnkFloatsById_ovb0064(value);
    u32 n = ++self->count;
    return n ? self->values + n - 1 : self->values;
}

// The copy constructor of the map of float tables.
void fn_7100aee75c(UnkFloatsByIdMap_ovb0064* self, const UnkFloatsByIdMap_ovb0064* other) {
    self->keys = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->values = nullptr;
    self->step = other->step;
    self->flags = other->flags;
    self->allocator = other->allocator;
    fn_7100aee918(reinterpret_cast<UnkIdMap<UnkKeyedStrIds>*>(self), other->count);
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i)
        new (&self->values[i]) UnkFloatsById_ovb0064(other->values[i]);
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i)
        CopyId(&self->keys[i], &other->keys[i]);
}

// The float tables as their set-by-key move sees them: the entry's ids are released (storage kept),
// then it takes the given table's fields and leaves that one empty.
struct UnkFloatsByIdOwned_ovb0064 {
    f32* values;
    UnkReleasedId_ovb0064* ids;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

struct UnkFloatsByIdOwnedMap_ovb0064 {
    UnkFloatsByIdOwned_ovb0064* values;
    u64* keys;
    u32 count;

    UnkFloatsByIdOwned_ovb0064* End() { return &values[count]; }
    UnkFloatsByIdOwned_ovb0064* Find(const u64* key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == *key)
                return values + i;
        return values + count;
    }
};

void fn_7100aee2c0(UnkFloatsByIdOwnedMap_ovb0064* self, const u64* key, UnkFloatsByIdOwned_ovb0064* value) {
    UnkFloatsByIdOwned_ovb0064* dst = self->Find(key);
    if (dst != self->End()) {
        if (dst->ids)
            for (u64 i = 0; i < dst->count; ++i)
                dst->ids[i].~UnkReleasedId_ovb0064();
        dst->count = 0;
        dst->allocator = value->allocator;
        dst->ids = value->ids;
        dst->values = value->values;
        dst->capacity = value->capacity;
        dst->count = value->count;
        dst->flags = value->flags;
        dst->step = value->step;
        value->count = 0;
        value->values = nullptr;
        value->ids = nullptr;
    }
}

// A string id whose move leaves the pool's empty string behind.
struct UnkMovedId_ovb0064 {
    UnkStringData* data;

    UnkMovedId_ovb0064(UnkMovedId_ovb0064&& other) : data(other.data) { other.data = &g_unk_7101d47108->empty; }
};

using UnkFloatsByIdValue_ovb0064 = base::UnkIterValue<UnkFloatsById_ovb0064, UnkMovedId_ovb0064>;

// The move constructor of the map's cached iterator value: the id is moved, the table copied.
void fn_7100aedd04(UnkFloatsByIdValue_ovb0064* self, UnkFloatsByIdValue_ovb0064* other) {
    using Pair = UnkFloatsByIdValue_ovb0064::Value;
    self->state = 0;
    if (other->state == 1) {
        self->state = 1;
        new (&self->value) Pair{static_cast<UnkMovedId_ovb0064&&>(other->value.index), other->value.element};
    } else if (other->state == 2) {
        self->state = 1;
        UnkFloatsByIdValue_ovb0064::Ref r = other->ref;
        new (&self->value)
            Pair{static_cast<UnkMovedId_ovb0064&&>(*const_cast<UnkMovedId_ovb0064*>(r.index)), *r.element};
    }
}

// ---- A handle (an id and the object a registry has for it) ----

struct UnkStrIdHandle {
    CStrId id;
    void* field_0x8;
};

void fn_7100ae986c(__attribute__((noescape)) UnkStrIdHandle* self, const CStrId& id);

// The handle fn_7100ae986c builds in place, destroyed by hand.
union UnkStrIdHandleTemp_ovb0064 {
    UnkStrIdHandle h;
    UnkStrIdHandleTemp_ovb0064(const CStrId& id) { fn_7100ae986c(&h, id); }
    ~UnkStrIdHandleTemp_ovb0064() { h.id.~CStrId(); }
};

// Assigns the handle for `id` (built, copied, then destroyed).
UnkStrIdHandle* fn_7100aec000(UnkStrIdHandle* self, const CStrId& id) {
    UnkStrIdHandleTemp_ovb0064 t(id);
    self->id = t.h.id;
    self->field_0x8 = t.h.field_0x8;
    return self;
}
