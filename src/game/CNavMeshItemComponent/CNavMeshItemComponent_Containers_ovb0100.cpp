// CNavMeshItemComponent unit (tools/tu_map.py unit 1482): the component's constructor and start, its
// reflected source invokers, and the container code of its reflected collections. Names and layouts are
// placeholders.
// - The constructor (fn_71009bf7f0) and the start (fn_71009c1180): the link objects' names go to the
//   navigation-mesh manager, the source with the global descriptor's id is selected, and the listeners told.
// - Reflected invokers that select a source (fn_71009c2c2c, as fn_71009c2d74 in fn_71009c2d74.cpp), pass it
//   an argument (fn_71009c29f0) or set its side (fn_71009c2b04).
// - A pair of string ids handed back to a sink (fn_71009c189c).
// - A map of string ids to arrays of 8-byte elements (parallel arrays of arrays and keys), the same code as
//   TActorTileStatesMap's in src/game/CBreakableTileGroupComponent/CBreakableTileGroupComponent_Containers_ovb0028.cpp
//   with smaller elements: erase (fn_71009c09a8), shrink (fn_71009c45d4), copy construction (fn_71009c3fa8),
//   copy assignment (fn_71009c4308), insertion (fn_71009c478c) and equality (fn_71009c3d44). Its reserve is
//   fn_71009c4158 (src/base/tunable/CTunable/fn_71001358e0.cpp), its clear the clone fn_71009c44c8.
// - The variant of a string id and an array of pointers, held (state 1) or referenced (state 2), the same code
//   as src/game/CBTObserverComponent/CBTObserverComponent_Containers_ovb0055.cpp's: copy-assignment
//   (fn_71009c3134), construction from another (fn_71009c3748) and move-assignment (fn_71009c32f0).
#include <new>

#include "base/UnkCloneHelpers.h"
#include "base/UnkDeferredSet.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/CEnumType.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkNavMeshManager.h"
#include "game_s2/UnkOwnedList.h"
#include "types.h"

using base::UnkDeferredCallback;
using base::UnkDeferredSet;
using base::UnkStrIdMember;
using base::global::CStrId;
using base::global::UnkStringData;
using base::reflection::refl_ArgValue;
using base::reflection::refl_EmptyValue;
using base::reflection::UnkReflValue;

// ---- Construction and start ----

struct UnkStateObject;
void fn_71010c4148(UnkStateObject* self);
extern void* g_unk_7101a887d0[];  // the component's vtable (the second one at +0x198)

struct UnkNavMeshItemFields_ovb0100 {
    base::UnkVector<void*> field_0x48{0xf};
    base::UnkVector<void*> field_0x70{0xf};
    base::UnkVector<void*> field_0x98{0xf};
    bool field_0xc0 = false;
    base::UnkVector<void*> field_0xc8{0xf};
    base::UnkVector<void*> field_0xf0{0xf};
    base::UnkVector<void*> field_0x118{0xf};
    bool field_0x140 = false;
    base::UnkVector<void*> sources{0x57};  // 0x148
    bool field_0x170 = false;
    void* field_0x178 = nullptr;
    void* current = nullptr;  // 0x180
    base::UnkVector<void*> field_0x188{0xf};
    u64 field_0x1b0 = 0;
    bool field_0x1b8 = true;
};

struct UnkNavMeshItemComponent_ovb0100 {
    void** vtable;
    u8 pad_0x8[0x38];
    void** vtable_0x40;
    UnkNavMeshItemFields_ovb0100 fields;  // 0x48
};
static_assert(sizeof(UnkNavMeshItemComponent_ovb0100) == 0x1c0, "UnkNavMeshItemComponent_ovb0100 layout");

void fn_71009bf7f0(UnkNavMeshItemComponent_ovb0100* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101a887d0 + 2;
    self->vtable_0x40 = g_unk_7101a887d0 + 0x33;
    new (&self->fields) UnkNavMeshItemFields_ovb0100;
}

// ---- The sources ----

struct UnkSource;
struct UnkSourceList;
struct UnkOwner;
struct UnkSourceArg;
UnkSource*& fn_71009c0bf0(UnkSourceList* self, u32 index);
void fn_71009bfda8(UnkOwner* self, UnkSource* src);
void fn_71009c5ab8(UnkSource* src, UnkSourceArg* arg);

// CNavMeshItemStage::ESide's enum type (src/game/CElectricGeneratorComponent/fn_7100a7dd80.cpp).
struct UnkEnumType;
extern UnkEnumType g_unk_717273ef58;
s32 fn_710009976c(const base::reflection::CEnumType* self, const base::global::CStrId* name);

struct UnkNavSourceDesc_ovb0100 {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkNavSource_ovb0100 {
    u8 pad_0x0[0x40];
    UnkNavSourceDesc_ovb0100* desc;
    s32 side;  // 0x48
};

struct UnkNavSourceList_ovb0100 {
    u8 pad_0x0[0x8];
    u32 count;
};

struct UnkNavOwner_ovb0100 {
    u8 pad_0x0[0x148];
    UnkNavSourceList_ovb0100 sources;
    u8 pad_0x154[0x2c];
    UnkNavSource_ovb0100* current;  // 0x180

    UnkNavSource_ovb0100* SourceAt(u32 i) {
        return reinterpret_cast<UnkNavSource_ovb0100*>(fn_71009c0bf0(reinterpret_cast<UnkSourceList*>(&sources), i));
    }
};

// Selects the source the descriptor argument names: the current one if it already has its id, else the
// first listed source with it (none for id -1 or when no source has it).
UnkReflValue fn_71009c2c2c(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* arg = args[0];
    UnkNavOwner_ovb0100* self = *reinterpret_cast<UnkNavOwner_ovb0100**>(obj + 8);
    u64 id = (*refl_ArgValue<UnkNavSourceDesc_ovb0100*>(arg))->id;
    if (!self->current || self->current->desc->id != id) {
        UnkNavSource_ovb0100* found = nullptr;
        if (id != u64(-1)) {
            for (u32 i = 0; i < self->sources.count; ++i) {
                UnkNavSource_ovb0100* src = self->SourceAt(i);
                if (src && src->desc->id == id) {
                    found = src;
                    break;
                }
            }
        }
        fn_71009bfda8(reinterpret_cast<UnkOwner*>(self), reinterpret_cast<UnkSource*>(found));
    }
    return refl_EmptyValue();
}

// Passes the second argument (null when it has no type) to the first listed source with the id of the
// descriptor the first one gives.
UnkReflValue fn_71009c29f0(void*, char* obj, UnkReflValue* const* values) {
    UnkReflValue* args = *values;
    UnkNavOwner_ovb0100* self = *reinterpret_cast<UnkNavOwner_ovb0100**>(obj + 8);
    UnkNavSourceDesc_ovb0100* const* desc =
        args[0].type ? refl_ArgValue<UnkNavSourceDesc_ovb0100*>(&args[0]) : nullptr;
    UnkSourceArg* value = args[1].type ? const_cast<UnkSourceArg*>(refl_ArgValue<UnkSourceArg>(&args[1])) : nullptr;
    u64 id = (*desc)->id;
    if (id != u64(-1)) {
        for (u32 i = 0; i < self->sources.count; ++i) {
            UnkNavSource_ovb0100* src = self->SourceAt(i);
            if (src && src->desc->id == id) {
                fn_71009c5ab8(reinterpret_cast<UnkSource*>(src), value);
                break;
            }
        }
    }
    return refl_EmptyValue();
}

// Sets the side (CNavMeshItemStage::ESide, by name) of the first listed source with the id of the descriptor the
// first argument gives.
UnkReflValue fn_71009c2b04(void*, char* obj, UnkReflValue* const* values) {
    UnkReflValue* args = *values;
    UnkNavOwner_ovb0100* self = *reinterpret_cast<UnkNavOwner_ovb0100**>(obj + 8);
    UnkNavSourceDesc_ovb0100* const* desc =
        args[0].type ? refl_ArgValue<UnkNavSourceDesc_ovb0100*>(&args[0]) : nullptr;
    const base::global::CStrId* name = args[1].type ? refl_ArgValue<base::global::CStrId>(&args[1]) : nullptr;
    u64 id = (*desc)->id;
    if (id != u64(-1)) {
        for (u32 i = 0; i < self->sources.count; ++i) {
            UnkNavSource_ovb0100* src = self->SourceAt(i);
            if (src && src->desc->id == id) {
                src->side =
                    fn_710009976c(reinterpret_cast<base::reflection::CEnumType*>(&g_unk_717273ef58), name);
                break;
            }
        }
    }
    return refl_EmptyValue();
}

void fn_71009c5d6c(UnkSource* src);
struct UnkZiplineLinkTarget_b49;
struct UnkZiplineLink_b49;
UnkZiplineLinkTarget_b49** fn_71003b0c4c(UnkZiplineLink_b49* self);

void fn_7100ec1278(UnkNavMeshManager* self, const base::global::CStrId* name);
void fn_71009c4aa0(UnkDeferredSet<UnkDeferredCallback<u64>>* s, const u64* arg);

// The global descriptor (as src/game/CNavMeshItemComponent/CNavMeshItemComponent_b36.cpp declares it).
struct UnkSourceDesc;
extern __attribute__((visibility("hidden"))) UnkSourceDesc* g_unk_717273ee10;

struct UnkNavLinkObject_ovb0100 {
    u8 pad_0x0[0x30];
    base::global::CStrId name;
};

struct UnkNavLinkTarget_ovb0100 {
    u8 pad_0x0[0x10];
    UnkNavLinkObject_ovb0100* object;
};

struct UnkNavLink_ovb0100 {
    u8 data[0x40];
};

struct UnkNavMeshItemUpdate_ovb0100 {
    u8 pad_0x0[0x48];
    UnkDeferredSet<UnkDeferredCallback<u64>> listeners;  // 0x48
    u8 pad_0x48[0x148 - 0x48 - sizeof(UnkDeferredSet<UnkDeferredCallback<u64>>)];
    UnkNavSourceList_ovb0100 sources;  // 0x148
    u8 pad_0x154[0x1c];
    bool started;                    // 0x170
    UnkNavSource_ovb0100* selected;  // 0x178
    UnkNavSource_ovb0100* current;   // 0x180
    base::UnkVector<UnkNavLink_ovb0100> links;  // 0x188

    UnkNavSource_ovb0100* SourceAt(u32 i) {
        return reinterpret_cast<UnkNavSource_ovb0100*>(fn_71009c0bf0(reinterpret_cast<UnkSourceList*>(&sources), i));
    }
};
static_assert(__builtin_offsetof(UnkNavMeshItemUpdate_ovb0100, links) == 0x188, "UnkNavMeshItemUpdate_ovb0100 layout");

static inline UnkNavMeshManager* NavMeshManager() {
    UnkNavMeshManager* manager = g_unk_71726a4510;
    if (!manager) {
        manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(sizeof(UnkNavMeshManager), 0x3f));
        fn_7100ebf278(manager);
        g_unk_71726a4510 = manager;
    }
    return manager;
}

// Start: registers each linked object's name with the navigation-mesh manager, selects the source with the
// global descriptor's id, refreshes the selected and current sources and tells the listeners.
void fn_71009c1180(UnkNavMeshItemUpdate_ovb0100* self) {
    self->started = true;
    for (UnkNavLink_ovb0100 *it = self->links.data, *end = self->links.data + self->links.count; it != end; ++it) {
        auto* target =
            reinterpret_cast<UnkNavLinkTarget_ovb0100*>(*fn_71003b0c4c(reinterpret_cast<UnkZiplineLink_b49*>(it)));
        if (!target)
            continue;
        if (UnkNavLinkObject_ovb0100* object = target->object)
            fn_7100ec1278(NavMeshManager(), &object->name);
    }
    UnkNavSource_ovb0100* found = nullptr;
    u64 id = reinterpret_cast<UnkNavSourceDesc_ovb0100*>(g_unk_717273ee10)->id;
    if (id != u64(-1)) {
        for (u32 i = 0; i < self->sources.count; ++i) {
            UnkNavSource_ovb0100* src = self->SourceAt(i);
            if (src && src->desc->id == id) {
                found = src;
                break;
            }
        }
    }
    self->selected = found;
    if (found)
        fn_71009c5d6c(reinterpret_cast<UnkSource*>(found));
    if (self->current)
        fn_71009c5d6c(reinterpret_cast<UnkSource*>(self->current));
    u64 arg = reinterpret_cast<u64>(self);
    fn_71009c4aa0(&self->listeners, &arg);
}

// ---- A pair of string ids ----

struct UnkNavIdPair_ovb0100 {
    CStrId first;
    CStrId second;
};

struct UnkNavPairSink_ovb0100 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48(UnkNavIdPair_ovb0100* pair);
};

struct UnkNavPairOwner_ovb0100 {
    u8 pad_0x0[0x38];
    UnkNavPairSink_ovb0100* sink;  // 0x38
    u8 pad_0x40[0x8];
    s32 released;  // 0x48
};

void fn_71009c189c(UnkNavPairOwner_ovb0100* self, UnkNavIdPair_ovb0100* pair) {
    if (!pair)
        return;
    pair->second.Reset();
    pair->first.Reset();
    self->sink->vf_0x48(pair);
    ++self->released;
}

// ---- The map of arrays ----

using UnkNavElemVector_ovb0100 = base::UnkVector<u64>;

struct UnkNavElemMap_ovb0100 {
    UnkNavElemVector_ovb0100* values;  // 0x0
    CStrId* keys;                      // 0x8
    u32 count;                         // 0x10
    u32 capacity;                      // 0x14
    u32 step;                          // 0x18
    u32 flags;                         // 0x1c
    base::UnkAllocator* allocator;     // 0x20

    const UnkNavElemVector_ovb0100* Find(const CStrId& key) const {
        const UnkNavElemVector_ovb0100* v = values;
        const CStrId* k = keys;
        for (u64 i = 0; i < count; ++i) {
            if (k[i].data == key.data)
                return &v[(u32)i];
        }
        return &v[count];
    }
};

// Releases the keys, frees the arrays and empties the map (a clone of fn_710027e494).
void fn_71009c44c8(UnkNavElemMap_ovb0100* self);

template <class V>
struct UnkIdMap;
void fn_71009c4158(UnkIdMap<UnkOwnedList>* self, u32 capacity);

static inline void MoveVector(UnkNavElemVector_ovb0100* dst, UnkNavElemVector_ovb0100* src) {
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

static inline void CopyConstructVector(UnkNavElemVector_ovb0100* dst, const UnkNavElemVector_ovb0100* src) {
    dst->data = nullptr;
    dst->count = 0;
    dst->capacity = 0;
    dst->step = src->step;
    dst->field_0x14 = src->field_0x14;
    dst->flags = src->flags;
    dst->allocator = src->allocator;
    dst->Reserve(src->capacity);
    __builtin_memcpy(dst->data, src->data, (u64)src->count * sizeof(u64));
    dst->count = src->count;
}

// A new reference to a key, stored bytewise (no alias info: the original reloads the count after it).
static inline void CopyKey(CStrId* dst, const CStrId* src) {
    UnkStringData* d = src->data;
    __builtin_memcpy(&dst->data, &d, sizeof(d));
    if (&g_unk_7101d47108->empty != d)
        ++d->ref_count;
}

// Erase at an entry: the last entry moves into the hole. Returns the entry now at that position.
UnkNavElemVector_ovb0100* fn_71009c09a8(UnkNavElemMap_ovb0100* self, UnkNavElemVector_ovb0100* it) {
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
__attribute__((noinline)) void fn_71009c45d4(UnkNavElemMap_ovb0100* self) {
    if (self->count == self->capacity)
        return;
    auto* values = static_cast<UnkNavElemVector_ovb0100*>(
        self->allocator->vf_0x40((u64)self->count * sizeof(UnkNavElemVector_ovb0100), 8, self->flags));
    auto* keys = static_cast<CStrId*>(self->allocator->vf_0x40((u64)self->count * sizeof(CStrId), 8, self->flags));
    if (UnkNavElemVector_ovb0100* src = self->values) {
        UnkNavElemVector_ovb0100* dst = values;
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
void fn_71009c3fa8(UnkNavElemMap_ovb0100* self, const UnkNavElemMap_ovb0100* other) {
    self->values = nullptr;
    self->keys = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->flags = other->flags;
    self->allocator = other->allocator;
    fn_71009c4158(reinterpret_cast<UnkIdMap<UnkOwnedList>*>(self), other->count);
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i)
        CopyConstructVector(&self->values[i], &other->values[i]);
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i)
        CopyKey(&self->keys[i], &other->keys[i]);
}

// Appends a copy of (key, value), growing by the step when full; returns the new entry.
UnkNavElemVector_ovb0100* fn_71009c478c(UnkNavElemMap_ovb0100* self, const CStrId* key,
                                        const UnkNavElemVector_ovb0100* value) {
    if (self->count == self->capacity)
        fn_71009c4158(reinterpret_cast<UnkIdMap<UnkOwnedList>*>(self), self->count + self->step);
    CopyKey(&self->keys[self->count], key);
    CopyConstructVector(&self->values[self->count], value);
    ++self->count;
    return self->count ? &self->values[self->count] - 1 : self->values;
}

// Copy assignment: cleared, shrunk and regrown to the other's count when the capacity differs, then copied.
UnkNavElemMap_ovb0100* fn_71009c4308(UnkNavElemMap_ovb0100* self, const UnkNavElemMap_ovb0100* other) {
    if (self == other)
        return self;
    fn_71009c44c8(self);
    if (other->count != self->capacity) {
        fn_71009c45d4(self);
        fn_71009c4158(reinterpret_cast<UnkIdMap<UnkOwnedList>*>(self), other->count);
    }
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i)
        CopyConstructVector(&self->values[i], &other->values[i]);
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i)
        CopyKey(&self->keys[i], &other->keys[i]);
    return self;
}

// Equal when both have the same keys with equal arrays.
static inline bool Equal(const UnkNavElemVector_ovb0100& a, const UnkNavElemVector_ovb0100& b) {
    if (b.count != a.count)
        return false;
    for (u64 i = 0, n = b.count; i < n; ++i) {
        if (a.data[i] != b.data[i])
            return false;
    }
    return true;
}

bool fn_71009c3d44(const UnkNavElemMap_ovb0100* self, const UnkNavElemMap_ovb0100* other) {
    if (self->count != other->count)
        return false;
    for (u32 i = 0; i < self->count; ++i) {
        const UnkNavElemVector_ovb0100* v = other->Find(self->keys[i]);
        if (v == &other->values[other->count])
            return false;
        if (!Equal(self->values[i], *v))
            return false;
    }
    return true;
}

// ---- The variant ----

struct UnkNavPtrList_ovb0100 {
    void** data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkNavPtrList_ovb0100(const UnkNavPtrList_ovb0100& other)
        : data(nullptr), count(0), capacity(0), step(other.step), field_0x14(other.field_0x14), flags(other.flags),
          allocator(other.allocator) {
        void** p = nullptr;
        if (u32 n = other.capacity) {
            p = static_cast<void**>(allocator->vf_0x40((u64)n * sizeof(void*), 8, flags));
            if (data) {
                __builtin_memcpy(p, data, (u64)count * sizeof(void*));
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = n;
        }
        __builtin_memcpy(p, other.data, (u64)other.count * sizeof(void*));
        count = other.count;
    }

    UnkNavPtrList_ovb0100& operator=(const UnkNavPtrList_ovb0100& other) {
        if (this != &other) {
            count = 0;
            u32 n = other.capacity;
            void** p;
            if (capacity < n) {
                p = static_cast<void**>(allocator->vf_0x40((u64)n * sizeof(void*), 8, flags));
                if (data) {
                    __builtin_memcpy(p, data, (u64)count * sizeof(void*));
                    allocator->vf_0x48(data);
                }
                data = p;
                capacity = n;
            } else {
                p = data;
            }
            __builtin_memcpy(p, other.data, (u64)other.count * sizeof(void*));
            count = other.count;
        }
        return *this;
    }
};

struct UnkNavIdListVariant_ovb0100 {
    struct Named {
        UnkStrIdMember id;
        UnkNavPtrList_ovb0100 list;
    };
    union {
        Named named;
        struct {
            UnkStrIdMember* id;
            UnkNavPtrList_ovb0100* list;
        } refs;
    };
    s32 state;  // 0x30

    UnkStrIdMember& Id() { return state == 2 ? *refs.id : named.id; }
    const UnkStrIdMember& Id() const { return state == 2 ? *refs.id : named.id; }
    UnkNavPtrList_ovb0100& List() { return state == 2 ? *refs.list : named.list; }
    const UnkNavPtrList_ovb0100& List() const { return state == 2 ? *refs.list : named.list; }
};

UnkNavIdListVariant_ovb0100* fn_71009c3134(UnkNavIdListVariant_ovb0100* self, const UnkNavIdListVariant_ovb0100* other) {
    if (other != self) {
        self->Id() = other->Id();
        self->List() = other->List();
    }
    return self;
}

// Takes the other variant's id and copies its list.
void fn_71009c3748(UnkNavIdListVariant_ovb0100* self, UnkNavIdListVariant_ovb0100* other) {
    self->state = 0;
    switch (other->state) {
    case 1:
        self->state = 1;
        new (&self->named.id) UnkStrIdMember(static_cast<UnkStrIdMember&&>(other->named.id));
        new (&self->named.list) UnkNavPtrList_ovb0100(other->named.list);
        break;
    case 2: {
        self->state = 1;
        UnkStrIdMember* id = other->refs.id;
        UnkNavPtrList_ovb0100* list = other->refs.list;
        new (&self->named.id) UnkStrIdMember(static_cast<UnkStrIdMember&&>(*id));
        new (&self->named.list) UnkNavPtrList_ovb0100(*list);
        break;
    }
    }
}

// The same variant over the engine's vector, for move-assignment.
struct UnkNavIdVecVariant_ovb0100 {
    union {
        struct {
            UnkStrIdMember id;
            base::UnkVector<void*> vec;
        } named;
        struct {
            UnkStrIdMember* id;
            base::UnkVector<void*>* vec;
        } refs;
    };
    s32 state;  // 0x30

    UnkNavIdVecVariant_ovb0100() {}
    ~UnkNavIdVecVariant_ovb0100() {}

    UnkStrIdMember& Id() { return state == 2 ? *refs.id : named.id; }
    base::UnkVector<void*>& Vec() { return state == 2 ? *refs.vec : named.vec; }
};

// Releases `to`, then takes `from`'s id, leaving the empty string (stored untyped: the original
// reloads the variant's state afterwards).
static inline void MoveId(UnkStrIdMember& to, UnkStrIdMember& from) {
    to.~UnkStrIdMemberOf();
    to.data = from.data;
    base::global::UnkStringData* empty = &g_unk_7101d47108->empty;
    __builtin_memcpy(&from.data, &empty, sizeof(empty));
}

UnkNavIdVecVariant_ovb0100* fn_71009c32f0(UnkNavIdVecVariant_ovb0100* self, UnkNavIdVecVariant_ovb0100* other) {
    switch (other->state) {
    case 1:
        MoveId(self->Id(), other->named.id);
        self->Vec() = static_cast<base::UnkVector<void*>&&>(other->named.vec);
        break;
    case 2: {
        UnkStrIdMember* id = other->refs.id;
        MoveId(self->Id(), *id);
        self->Vec() = static_cast<base::UnkVector<void*>&&>(*other->refs.vec);
        break;
    }
    }
    return self;
}
