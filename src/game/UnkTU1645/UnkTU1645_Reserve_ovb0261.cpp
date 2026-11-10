// Unit 1645 (tools/tu_map.py, no reliable name): the Reserve of two keyed maps of string ids: owned
// objects destroyed through their virtual slot 0x8, and owned tables of string ids destroyed with
// fn_7100ae2df8 and freed. The values and the ids move into the new storage. Names and layouts are
// placeholders.
#include <new>

#include "base/UnkKeyedStrIds.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using namespace base::global;

// A string id whose destructor decrements and tests in two steps (the original reloads the id
// after the decrement). Its move stores the pointer bytewise, so the moved-from id's release
// reloads the pool.
struct UnkReleasedId_b0261 {
    UnkStringData* data;

    UnkReleasedId_b0261(UnkReleasedId_b0261&& other) {
        UnkStringData* d = other.data;
        __builtin_memcpy(&data, &d, sizeof(d));
        other.data = &g_unk_7101d47108->empty;
    }
    ~UnkReleasedId_b0261() {
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

// An owned object destroyed through its virtual slot 0x8.
struct UnkOwnedObj_b0261 {
    virtual void vf_0x0();
    virtual void vf_0x8();
};

struct UnkOwnedSlot_b0261 {
    union {
        UnkOwnedObj_b0261* ptr;
    };
    UnkOwnedSlot_b0261(UnkOwnedSlot_b0261&& other) {
        UnkOwnedObj_b0261* p = other.ptr;
        other.ptr = nullptr;
        ptr = p;
    }
    ~UnkOwnedSlot_b0261() {
        UnkOwnedObj_b0261* old = ptr;
        ptr = nullptr;
        if (old)
            old->vf_0x8();
    }
};

struct UnkOwnedMap_b0261 {
    UnkOwnedSlot_b0261* values;
    UnkReleasedId_b0261* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_7100ae3834(UnkOwnedMap_b0261* self, u32 capacity) {
    if (self->capacity < capacity) {
        auto* values = static_cast<UnkOwnedSlot_b0261*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(UnkOwnedSlot_b0261), 8, self->flags));
        auto* keys = static_cast<UnkReleasedId_b0261*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(UnkReleasedId_b0261), 8, self->flags));
        if (UnkOwnedSlot_b0261* src = self->values) {
            UnkOwnedSlot_b0261* dst = values;
            for (u32 i = 0; i < self->count; ++i, ++src, ++dst) {
                new (dst) UnkOwnedSlot_b0261(static_cast<UnkOwnedSlot_b0261&&>(*src));
                src->~UnkOwnedSlot_b0261();
            }
            self->allocator->vf_0x48(self->values);
        }
        if (UnkReleasedId_b0261* src = self->keys) {
            UnkReleasedId_b0261* dst = keys;
            for (u32 i = 0; i < self->count; ++i, ++src, ++dst) {
                new (dst) UnkReleasedId_b0261(static_cast<UnkReleasedId_b0261&&>(*src));
                src->~UnkReleasedId_b0261();
            }
            self->allocator->vf_0x48(self->keys);
        }
        self->values = values;
        self->keys = keys;
        self->capacity = capacity;
    }
}

// An owned table of string ids, destroyed and freed with the slot.
struct UnkKeyedSlot_b0261 {
    union {
        UnkKeyedStrIds* ptr;
    };
    UnkKeyedSlot_b0261(UnkKeyedSlot_b0261&& other) {
        UnkKeyedStrIds* p = other.ptr;
        other.ptr = nullptr;
        ptr = p;
    }
    ~UnkKeyedSlot_b0261() {
        UnkKeyedStrIds* old = ptr;
        ptr = nullptr;
        if (old) {
            fn_7100ae2df8(old);
            fn_7100083c00(old);
        }
    }
};

struct UnkKeyedMap_b0261 {
    UnkKeyedSlot_b0261* values;
    UnkReleasedId_b0261* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_7100ae2ef4(UnkKeyedMap_b0261* self, u32 capacity) {
    if (self->capacity < capacity) {
        auto* values = static_cast<UnkKeyedSlot_b0261*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(UnkKeyedSlot_b0261), 8, self->flags));
        auto* keys = static_cast<UnkReleasedId_b0261*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(UnkReleasedId_b0261), 8, self->flags));
        if (UnkKeyedSlot_b0261* src = self->values) {
            UnkKeyedSlot_b0261* dst = values;
            for (u32 i = 0; i < self->count; ++i, ++src, ++dst) {
                new (dst) UnkKeyedSlot_b0261(static_cast<UnkKeyedSlot_b0261&&>(*src));
                src->~UnkKeyedSlot_b0261();
            }
            self->allocator->vf_0x48(self->values);
        }
        if (UnkReleasedId_b0261* src = self->keys) {
            UnkReleasedId_b0261* dst = keys;
            for (u32 i = 0; i < self->count; ++i, ++src, ++dst) {
                new (dst) UnkReleasedId_b0261(static_cast<UnkReleasedId_b0261&&>(*src));
                src->~UnkReleasedId_b0261();
            }
            self->allocator->vf_0x48(self->keys);
        }
        self->values = values;
        self->keys = keys;
        self->capacity = capacity;
    }
}
