// CPerceptionComponent's unit (tools/tu_map.py): the component's perceiver map at +0x48, two parallel
// arrays of 8-byte elements (owned perceivers and their string ids) sharing a count, a capacity, a step,
// allocator flags and an allocator. fn_71009c81cc is the component's destructor (the vtable, the map, then
// the component destructor fn_71010c4194); fn_71009c8338 runs the base component's step (fn_71010c41bc), caches
// the owner (+0x10) cast to CCharClassPerceptionComponent at +0x40 and fills the map with a perceiver created by
// each of that class's entries (+0x58, count at +0x60) under the entry's name (copied as raw bytes, then
// referenced); fn_71009c86a0 grows the map, moving both arrays into new storage.
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/global/CStrId.h"
#include "game/UnkComponent.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

typedef u32 __attribute__((may_alias)) a_u32;

struct UnkPerceiver_ovb0126 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void Delete();  // 0x18
};

// An owned perceiver pointer.
struct UnkPerceiverPtr_ovb0126 {
    UnkPerceiver_ovb0126* ptr;

    void Reset() {
        UnkPerceiver_ovb0126* p = ptr;
        __builtin_memset(&ptr, 0, sizeof(ptr));
        if (p)
            p->Delete();
    }
};

// A string id whose release decrements and tests the count in two steps.
struct UnkPerceptionStrId_ovb0126 {
    UnkStringData* data;

    void Release() {
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

struct UnkPerceptionMap_ovb0126 {
    UnkPerceiverPtr_ovb0126* values;
    UnkPerceptionStrId_ovb0126* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) void Destroy() {
        if (keys) {
            for (u32 i = 0; i < count; i++)
                keys[i].Release();
        }
        if (values) {
            for (u64 i = 0; i < *reinterpret_cast<a_u32*>(&count); i++) {
                UnkPerceiver_ovb0126* p = values[i].ptr;
                values[i].ptr = nullptr;
                if (p)
                    p->Delete();
            }
        }
        count = 0;
        if (keys)
            allocator->vf_0x48(keys);
        if (values)
            allocator->vf_0x48(values);
    }
};

void fn_71009c86a0(UnkPerceptionMap_ovb0126* self, u32 n) {
    if (self->capacity >= n)
        return;
    u64 size = (u64)n * 8;
    auto* new_values = static_cast<UnkPerceiverPtr_ovb0126*>(self->allocator->vf_0x40(size, 8, self->flags));
    auto* new_keys = static_cast<UnkPerceptionStrId_ovb0126*>(self->allocator->vf_0x40(size, 8, self->flags));
    if (UnkPerceiverPtr_ovb0126* old = self->values) {
        // A 64-bit index for the new array beside the 32-bit counter: the end test compares 32 bits.
        u64 j = 0;
        for (u32 i = 0; i < self->count; ++i, ++j, ++old) {
            UnkPerceiver_ovb0126* p = old->ptr;
            old->ptr = nullptr;
            new_values[j].ptr = p;
            old->Reset();
        }
        self->allocator->vf_0x48(self->values);
    }
    if (UnkPerceptionStrId_ovb0126* old = self->keys) {
        u64 j = 0;
        for (u32 i = 0; i < self->count; ++i, ++j, ++old) {
            __builtin_memcpy(&new_keys[j], old, sizeof(*old));
            old->data = &g_unk_7101d47108->empty;
            old->Release();
        }
        self->allocator->vf_0x48(self->keys);
    }
    self->values = new_values;
    self->keys = new_keys;
    self->capacity = n;
}

extern char g_unk_7101a88cd8[];  // the component's vtable

struct UnkPerceptionEntry_ovb0126 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual UnkPerceiver_ovb0126* Create();  // 0x10
    UnkPerceptionStrId_ovb0126 name;
};

struct UnkCharClassPerception_ovb0126 {
    u8 pad_0x0[0x58];
    UnkPerceptionEntry_ovb0126** entries;
    u32 count;
};

struct UnkPerceptionComponent_ovb0126 {
    char* vtable;
    u8 pad_0x8[0x8];
    base::reflection::UnkReflObject* owner;  // 0x10
    u8 pad_0x18[0x28];
    UnkCharClassPerception_ovb0126* char_class;  // 0x40
    UnkPerceptionMap_ovb0126 map;  // 0x48
};

bool fn_71009c8338(UnkPerceptionComponent_ovb0126* self, void* arg) {
    bool r = fn_71010c41bc(reinterpret_cast<UnkBaseComponent*>(self), arg);
    base::reflection::UnkReflObject* o = self->owner;
    auto* cls = reinterpret_cast<UnkCharClassPerception_ovb0126*>(
        o && fn_710009d50c(o->vf_0x0(), fn_71009c77f0()) ? o : nullptr);
    self->char_class = cls;
    for (UnkPerceptionEntry_ovb0126 **it = cls->entries, **end = it + cls->count; it != end; ++it) {
        UnkPerceptionEntry_ovb0126* entry = *it;
        UnkPerceiver_ovb0126* perceiver = entry->Create();
        UnkPerceptionMap_ovb0126* map = &self->map;
        if (map->count == map->capacity)
            fn_71009c86a0(map, map->count + map->step);
        UnkPerceptionStrId_ovb0126* k = &map->keys[map->count];
        __builtin_memcpy(k, &entry->name, sizeof(*k));
        if (&g_unk_7101d47108->empty != k->data)
            ++k->data->ref_count;
        __builtin_memcpy(&map->values[map->count], &perceiver, sizeof(perceiver));
        ++map->count;
    }
    return r;
}

void fn_71009c81cc(UnkPerceptionComponent_ovb0126* self) {
    self->vtable = g_unk_7101a88cd8 + 0x10;
    self->map.Destroy();
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}
