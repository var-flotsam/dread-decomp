// Unit 2200's (no reliable name; hint CCutSceneDef) copy, move and destroy helpers: the 0x40-byte take
// record (a string id, a number, a path id, three string ids, a 64-bit number and a number), the 0xa0-byte
// record that holds two arrays and a map of ids, a vector of path ids, and the reflected copies of take
// records read from a direct or an indirect source. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "game/UnkTU2200.h"
#include "game/UnkTU83.h"
#include "types.h"

#include <new>

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkElem40_b42a3 {
    CStrId field_0x0;
    u32 field_0x8;
    CFilePathStrId field_0x10;
    CStrId field_0x18;
    CStrId field_0x20;
    CStrId field_0x28;
    u64 field_0x30;
    u32 field_0x38;
};

struct UnkElem40Array_b42a3 {
    UnkElem40_b42a3* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_7100c90fb0(void* dst, const void* src) {
    new (dst) UnkElem40_b42a3(*static_cast<const UnkElem40_b42a3*>(src));
}

void fn_7100c8b894(UnkElem40Array_b42a3* self, const UnkElem40_b42a3* value) {
    u32 n = self->count;
    u32 capacity = self->capacity;
    if (capacity <= n && self->step != 0)
        capacity = (n / self->step + 1) * self->step;
    if ((s32)n >= (s32)capacity)
        for (;;) {
        }
    fn_7100c8b758(self, capacity);
    new (&self->data[self->count]) UnkElem40_b42a3(*value);
    ++self->count;
}

static inline void ResetFilePath(CFilePathStrId& id) {
    if (base::global::UnkStringPool* pool = g_unk_7101d47200) {
        if (id.data && id.data != &pool->empty) {
            --id.data->ref_count;
            if (id.data->ref_count == 0) {
                fn_71000803a4(pool, id.data);
                pool = g_unk_7101d47200;
            }
        }
        id.data = &pool->empty;
    }
}

void fn_7100c8a490(void* p) {
    UnkElem40_b42a3* e = static_cast<UnkElem40_b42a3*>(p);
    e->field_0x28.Reset();
    e->field_0x20.Reset();
    e->field_0x18.Reset();
    ResetFilePath(e->field_0x10);
    e->field_0x0.Reset();
}

void* fn_7100c91450(void* dst, const void* src) {
    UnkElem40_b42a3* a = static_cast<UnkElem40_b42a3*>(dst);
    const UnkElem40_b42a3* b = static_cast<const UnkElem40_b42a3*>(src);
    a->field_0x0 = b->field_0x0;
    a->field_0x8 = b->field_0x8;
    if (a->field_0x10.data != b->field_0x10.data) {
        base::global::UnkStringData* d = b->field_0x10.data;
        base::global::UnkStringPool* pool = g_unk_7101d47200;
        if (d != &pool->empty)
            ++d->ref_count;
        if (a->field_0x10.data != &pool->empty) {
            --a->field_0x10.data->ref_count;
            if (a->field_0x10.data->ref_count == 0)
                fn_71000803a4(pool, a->field_0x10.data);
        }
        a->field_0x10.data = d;
    }
    a->field_0x18 = b->field_0x18;
    a->field_0x20 = b->field_0x20;
    a->field_0x28 = b->field_0x28;
    u64 x = b->field_0x30;
    u32 y = b->field_0x38;
    a->field_0x38 = y;
    a->field_0x30 = x;
    return dst;
}

static inline void TakeFilePath(CFilePathStrId& dst, CFilePathStrId& src) {
    ResetFilePath(dst);
    dst.data = src.data;
    src.data = &g_unk_7101d47200->empty;
}

void* fn_7100c9165c(void* dst, const void* src) {
    UnkElem40_b42a3* a = static_cast<UnkElem40_b42a3*>(dst);
    UnkElem40_b42a3* b = const_cast<UnkElem40_b42a3*>(static_cast<const UnkElem40_b42a3*>(src));
    a->field_0x0.ReleaseAndTake(b->field_0x0);
    a->field_0x8 = b->field_0x8;
    TakeFilePath(a->field_0x10, b->field_0x10);
    a->field_0x18.ReleaseAndTake(b->field_0x18);
    a->field_0x20.ReleaseAndTake(b->field_0x20);
    a->field_0x28.ReleaseAndTake(b->field_0x28);
    u64 x = b->field_0x30;
    u32 y = b->field_0x38;
    a->field_0x38 = y;
    a->field_0x30 = x;
    return dst;
}


struct UnkPathEntry_b239 {
    CFilePathStrId path;
    u64 value;
};

struct UnkPathArray_b239 {
    UnkPathEntry_b239* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 field_0x18;
    u64 field_0x20;
};

struct UnkFXPathEntry_b0068;

static inline __attribute__((always_inline)) void AppendAll_b239(UnkPathArray_b239* self, const UnkPathArray_b239* other) {
    for (const UnkPathEntry_b239 *it = other->data, *end = it + other->count; it != end; ++it) {
        u32 n = self->count;
        u32 capacity = self->capacity;
        if (capacity <= n && self->step != 0)
            capacity = (n / self->step + 1) * self->step;
        if ((s32)n >= (s32)capacity)
            for (;;) {
            }
        fn_7100c8c210(reinterpret_cast<base::UnkVector<UnkFXPathEntry_b0068>*>(self), capacity);
        UnkPathEntry_b239* d = &self->data[self->count];
        new (&d->path) CFilePathStrId(it->path);
        __builtin_memcpy(&d->value, &it->value, sizeof(u64));
        ++self->count;
    }
}

void fn_7100c8c108(UnkPathArray_b239* self, const UnkPathArray_b239* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->field_0x18 = other->field_0x18;
    self->field_0x20 = other->field_0x20;
    fn_7100c8c210(reinterpret_cast<base::UnkVector<UnkFXPathEntry_b0068>*>(self), other->capacity);
    AppendAll_b239(self, other);
}

UnkPathArray_b239* fn_7100c8cc78(UnkPathArray_b239* self, const UnkPathArray_b239* other) {
    if (self != other) {
        for (u64 i = 0; i < self->count; ++i)
            ResetFilePath(self->data[i].path);
        self->count = 0;
        fn_7100c8c210(reinterpret_cast<base::UnkVector<UnkFXPathEntry_b0068>*>(self), other->capacity);
        AppendAll_b239(self, other);
    }
    return self;
}

struct UnkStrIdPair_b239 {
    CStrId first;
    CStrId second;
    u32 value;
};

struct UnkStrIdMap_b239 {
    UnkStrIdPair_b239* values;
    CStrId* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

struct UnkIdArray_b239 {
    u32* numbers;
    CStrId* ids;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

struct UnkElemA0_b42a3 {
    CStrId field_0x0;
    CFilePathStrId field_0x8;
    CStrId field_0x10;
    CStrId field_0x18;
    u16 field_0x20;
    u32 field_0x24;
    UnkIdArray_b239 field_0x28;
    u64 field_0x50;
    CStrId field_0x58;
    CStrId field_0x60;
    u32 field_0x68;
    UnkStrIdMap_b239 field_0x70;
    CFilePathStrId field_0x98;
};
static_assert(sizeof(UnkElemA0_b42a3) == 0xa0, "UnkElemA0_b42a3 layout");

struct UnkElemA0Array_b42a3 {
    UnkElemA0_b42a3* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

struct UnkKeyedStrIds;
void fn_7100c8bd98(UnkStrIdMap_b239* self, const UnkStrIdMap_b239* other);

// Copies an id with an untyped store of the pointer (the caller's loop bound is reloaded after it).
static inline void CopyIdUntyped(CStrId* dst, const CStrId& src) {
    base::global::UnkStringData* d = src.data;
    __builtin_memcpy(&dst->data, &d, sizeof(d));
    if (&g_unk_7101d47108->empty != d)
        ++d->ref_count;
}

void fn_7100c8bba4(UnkElemA0_b42a3* self, const UnkElemA0_b42a3* other) {
    new (&self->field_0x0) CStrId(other->field_0x0);
    new (&self->field_0x8) CFilePathStrId(other->field_0x8);
    new (&self->field_0x10) CStrId(other->field_0x10);
    new (&self->field_0x18) CStrId(other->field_0x18);
    self->field_0x20 = other->field_0x20;
    self->field_0x24 = other->field_0x24;
    self->field_0x28.numbers = nullptr;
    self->field_0x28.ids = nullptr;
    self->field_0x28.count = 0;
    self->field_0x28.capacity = 0;
    self->field_0x28.step = other->field_0x28.step;
    self->field_0x28.flags = other->field_0x28.flags;
    self->field_0x28.allocator = other->field_0x28.allocator;
    fn_71000df22c(reinterpret_cast<UnkKeyedStrIds*>(&self->field_0x28), other->field_0x28.count);
    __builtin_memcpy(self->field_0x28.numbers, other->field_0x28.numbers, (u64)other->field_0x28.count * 4);
    self->field_0x28.count = other->field_0x28.count;
    self->field_0x28.count = other->field_0x28.count;
    for (u64 i = 0; i < self->field_0x28.count; ++i)
        CopyIdUntyped(&self->field_0x28.ids[i], other->field_0x28.ids[i]);
    self->field_0x50 = other->field_0x50;
    new (&self->field_0x58) CStrId(other->field_0x58);
    new (&self->field_0x60) CStrId(other->field_0x60);
    self->field_0x68 = other->field_0x68;
    fn_7100c8bd98(&self->field_0x70, &other->field_0x70);
    new (&self->field_0x98) CFilePathStrId(other->field_0x98);
}


struct UnkIdTriple_b239 {
    CStrId first;
    CStrId second;
    u32 value;
};

struct UnkIdRefs_b239 {
    CStrId* first;
    UnkIdTriple_b239* rest;
};

struct UnkIdSource_b239 {
    union {
        struct {
            CStrId first;
            CStrId second;
            CStrId third;
            u32 value;
        } direct;
        UnkIdRefs_b239 refs;
    };
    u32 kind;
};

struct UnkIdDest_b239 {
    CStrId first;
    CStrId second;
    CStrId third;
    u32 value;
    u32 pad_0x1c;
    u32 kind;
};

void fn_7100c902b4(void* dst_in, const void* src_in) {
    UnkIdDest_b239* dst = static_cast<UnkIdDest_b239*>(dst_in);
    UnkIdSource_b239* src = const_cast<UnkIdSource_b239*>(static_cast<const UnkIdSource_b239*>(src_in));
    dst->kind = 0;
    switch (src->kind) {
    case 1:
        dst->kind = 1;
        dst->first.data = src->direct.first.data;
        src->direct.first.data = &g_unk_7101d47108->empty;
        new (&dst->second) CStrId(src->direct.second);
        new (&dst->third) CStrId(src->direct.third);
        dst->value = src->direct.value;
        break;
    case 2: {
        dst->kind = 1;
        CStrId* first = src->refs.first;
        UnkIdTriple_b239* rest = src->refs.rest;
        dst->first.data = first->data;
        first->data = &g_unk_7101d47108->empty;
        new (&dst->second) CStrId(rest->first);
        new (&dst->third) CStrId(rest->second);
        dst->value = rest->value;
        break;
    }
    }
}

struct UnkTakeRecord_b239 {
    u32 first;
    UnkElem40_b42a3 rest;
    u32 kind;
};

struct UnkTakeRefs_b239 {
    u32* first;
    UnkElem40_b42a3* rest;
};

struct UnkTakeSource_b239 {
    union {
        struct {
            u32 first;
            UnkElem40_b42a3 rest;
        } direct;
        UnkTakeRefs_b239 refs;
    };
    u32 kind;
};

void fn_7100c91c58(void* dst_in, const void* src_in) {
    UnkTakeRecord_b239* dst = static_cast<UnkTakeRecord_b239*>(dst_in);
    const UnkTakeSource_b239* src = static_cast<const UnkTakeSource_b239*>(src_in);
    dst->kind = 0;
    switch (src->kind) {
    case 1:
        dst->kind = 1;
        dst->first = src->direct.first;
        new (&dst->rest) UnkElem40_b42a3(src->direct.rest);
        break;
    case 2:
        dst->kind = 1;
        UnkTakeRefs_b239 refs = src->refs;
        dst->first = *refs.first;
        new (&dst->rest) UnkElem40_b42a3(*refs.rest);
        break;
    }
}
