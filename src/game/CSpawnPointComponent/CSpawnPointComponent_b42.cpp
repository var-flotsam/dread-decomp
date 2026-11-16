// CSpawnPointComponent unit (tools/tu_map.py, unit 2704): the copy constructor of a base::global::CRntVector of
// 0x40-byte entries, the same code as fn_7100246a94 (src/game/CActor/CActor_VectorCopy_b41.cpp) with
// this unit's own entry move (fn_710111fd80) and append (fn_7101131b98): reset, copy the step, flags
// and allocator, reserve the other's capacity (entries moved, the old ones destroyed through their
// virtual destructor), then append a copy of each entry. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/CSpawnGroupComponent.h"
#include "game/UnkSpawnHolder.h"
#include "game/UnkTU2704.h"
#include "types.h"

void fn_7101131a84(base::global::CRntVector<UnkSpawnHolder>* self, const base::global::CRntVector<UnkSpawnHolder>* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    u32 n = other->capacity;
    if (self->capacity < n) {
        UnkSpawnHolder* p = static_cast<UnkSpawnHolder*>(self->allocator->vf_0x40((u64)n * sizeof(UnkSpawnHolder), 8, self->flags));
        if (UnkSpawnHolder* src = self->data) {
            UnkSpawnHolder* dst = p;
            for (u32 i = 0; i < self->count; ++i) {
                fn_710111fd80(dst, src);
                (src++)->~UnkSpawnHolder();
                ++dst;
            }
            self->allocator->vf_0x48(self->data);
        }
        self->data = p;
        self->capacity = n;
    }
    for (const UnkSpawnHolder *it = other->data, *end = other->data + other->count; it != end; ++it)
        fn_7101131b98(self, it);
}

// The copy assignment beside it: destroy the entries, reserve the other's capacity (entries moved
// with fn_710111fd80), then append a copy of each entry (fn_7101131b98).
base::global::CRntVector<UnkSpawnHolder>* fn_7101131d60(base::global::CRntVector<UnkSpawnHolder>* self,
                                               const base::global::CRntVector<UnkSpawnHolder>* other) {
    if (self == other)
        return self;
    for (u64 i = 0; i < self->count; ++i)
        self->data[i].~UnkSpawnHolder();
    self->count = 0;
    u32 n = other->capacity;
    if (self->capacity < n) {
        UnkSpawnHolder* p = static_cast<UnkSpawnHolder*>(self->allocator->vf_0x40((u64)n * sizeof(UnkSpawnHolder), 8, self->flags));
        if (UnkSpawnHolder* src = self->data) {
            UnkSpawnHolder* dst = p;
            for (u32 i = 0; i < self->count; ++i) {
                fn_710111fd80(dst, src);
                (src++)->~UnkSpawnHolder();
                ++dst;
            }
            self->allocator->vf_0x48(self->data);
        }
        self->data = p;
        self->capacity = n;
    }
    for (const UnkSpawnHolder *it = other->data, *end = other->data + other->count; it != end; ++it)
        fn_7101131b98(self, it);
    return self;
}
